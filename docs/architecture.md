# ferromem-kernel — Architecture

Status: **skeleton v0.0.1, design only**. The working implementation lands in Phase 2 month 3 (~2026-08).

This document is the design reference. The companion design in the parent project (private) lives at `08-aws-integration/xrt-zram-driver-design.md`; this public version captures the OSS-relevant subset.

## 1. Goals

1. **OSS-only loadable kernel module** that registers a new `zcomp_backend` named `ferromem` for the Linux zram subsystem.
2. **Hardware-independent layer**: the kernel module is the only piece in the data path that talks to the Linux kernel. All FPGA-specific glue is below the `ferromem_xrt_*` interface.
3. **Portable**: kernel module can be loaded on any Linux 6.8+ host; without an FPGA bitstream it stays inert (`-EOPNOTSUPP`). With the AWS Marketplace AMI's proprietary bitstream, it accelerates compression in hardware. The same kernel layer can be paired with an alternative bitstream (e.g., onprem Alveo, Azure Stratix 10).
4. **Production-grade**: clean kernel coding style, KUnit tests, GitHub Actions CI, DKMS packaging.

## 2. Data flow

```
┌────────────────────────────────────────────────────────────┐
│   user space: any swap-out / zswap consumer                │
└─────────────────────────────┬──────────────────────────────┘
                              │ writes to /dev/zram0 (or zswap pool)
┌─────────────────────────────▼──────────────────────────────┐
│   Linux zram subsystem  (drivers/block/zram/)              │
│   ── selects backend via /sys/block/zram0/comp_algorithm   │
└─────────────────────────────┬──────────────────────────────┘
                              │ struct zcomp_backend → "ferromem"
┌─────────────────────────────▼──────────────────────────────┐
│   ferromem_zram.c   (THIS REPO, Apache 2.0)                │
│   .compress / .decompress per 4 KB page                    │
└─────────────────────────────┬──────────────────────────────┘
                              │ ferromem_xrt_compress / _decompress
┌─────────────────────────────▼──────────────────────────────┐
│   ferromem_xrt.c    (THIS REPO, Apache 2.0)                │
│   Either kernel-direct via /dev/xclmgmt* ioctl             │
│        OR netlink to user-space daemon                     │
└─────────────────────────────┬──────────────────────────────┘
                              │ XRT command queue
┌─────────────────────────────▼──────────────────────────────┐
│   AWS F2 FPGA + AFI bitstream  (PROPRIETARY, AMI bundle)   │
└────────────────────────────────────────────────────────────┘
```

The boundary between "this repo" and "Marketplace AMI" is at the `ferromem_xrt_*` interface. The interface is stable C; replacing the implementation underneath swaps the backend hardware.

## 3. Open decision: kernel-direct vs user-space daemon (Phase 2 m3)

### Option A: kernel-direct (`/dev/xclmgmt*` ioctl from inside `ferromem_xrt.c`)

| Pro | Con |
|---|---|
| Lower latency (~500 ns less per page) | Tighter coupling to XRT kernel ABI |
| No additional process to manage | XRT ABI changes require module rebuild |
| Single trust boundary | Harder to swap proprietary AFI loader |

### Option B: user-space daemon (`ferromem-daemon` proprietary, netlink to kernel module)

| Pro | Con |
|---|---|
| Decouples kernel module from XRT ABI | +500 ns/page syscall overhead |
| AFI verification / signing can stay user-space | Two processes in critical path |
| Easier to substitute alternative bitstream backends | More moving parts |

**Provisional choice**: **Option B for v0.1.0** (simpler, faster to ship), revisit Option A for v0.3.0 once batched-mode compress is in place and 500 ns is meaningful relative to total per-page time.

## 4. Performance targets (4 KB page)

| Metric | v0.1.0 target | v0.3.0 (batched mode) target |
|---|---|---|
| Compress latency p50 | ≤ 1.5 μs | ≤ 800 ns |
| Decompress latency p50 | ≤ 1.0 μs | ≤ 500 ns |
| Throughput (32-page batch) | ≥ 2 GB/s | ≥ 4 GB/s |
| Compression ratio (zram realistic) | ≥ 1.75× | ≥ 1.85× |

Reference: CPU `lz4` does a 4 KB page in ~150 ns. FerroMem is slower per-page but frees a CPU core entirely, which is the ROI for memory-pressured workloads.

## 5. zcomp backend interface

The Linux kernel's `drivers/block/zram/zcomp.c` exposes a `struct zcomp_backend` registration point. We implement:

```c
static struct zcomp_strm *ferromem_create(void);
static void ferromem_destroy(struct zcomp_strm *zstrm);
static int  ferromem_compress(struct zcomp_strm *zstrm,
                              const void *src, size_t src_len,
                              void *dst, size_t *dst_len);
static int  ferromem_decompress(struct zcomp_strm *zstrm,
                                const void *src, size_t src_len,
                                void *dst, size_t dst_len);

static struct zcomp_backend ferromem_backend = {
    .name        = "ferromem",
    .create      = ferromem_create,
    .destroy     = ferromem_destroy,
    .compress    = ferromem_compress,
    .decompress  = ferromem_decompress,
    .set_max_streams = ferromem_set_max_streams,
};
```

Once registered, users select the backend with `echo ferromem > /sys/block/zram0/comp_algorithm`.

## 6. Batched-mode (v0.3.0)

The default zram path is per-page. For HBM-backed FPGA acceleration the per-page PCIe roundtrip is the dominant cost. v0.3.0 will add a batched API that accepts 32–128 pages in one submission, amortising the roundtrip overhead.

This mirrors the Alibaba Anolis `batched_compress` patches under upstream review for IAA. We aim to keep our ABI compatible so that if those patches land, our module slots in directly.

## 7. Test plan

- **Unit (KUnit)**: backend register/unregister, parameter parsing, ioctl plumbing, stub `-EOPNOTSUPP` paths.
- **Integration (F2 instance)**: load module → load AFI → enable zram with ferromem backend → write/read random 4 KB pages → verify byte-equal and compression ratio.
- **Soak**: 24 h continuous zram swap-in/out, monitor RSS for leaks, error counters for graceful degradation.
- **Chaos**: AFI load fail → module stays loaded but reports `-EOPNOTSUPP`; zram falls back to CPU `lz4`.

## 8. Linking with the proprietary AMI

The Marketplace AMI ships `ferromem_zram.ko` pre-built in `/opt/ferromem/kernel/`. Users may either:

- Use the pre-built `.ko` (Marketplace path, recommended for AWS F2 customers)
- Build from this repo against the running kernel (DKMS path, recommended for cross-cloud or onprem)

Both produce binary-compatible modules. The proprietary daemon and AFI bitstream are *not* in this repository; they remain in the Marketplace AMI bundle.

## 9. References

- Linux kernel `drivers/block/zram/zcomp.c` — zcomp backend registration
- XRT documentation — https://github.com/Xilinx/XRT
- AWS F2 documentation — https://awsdocs-fpga-f2.readthedocs-hosted.com/
- Meta TMO (ASPLOS 2022) — production zswap deployment, compression ratio reference
- Alibaba Anolis IAA + zstd batching — upstream patches, batched API shape
