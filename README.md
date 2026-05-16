# ferromem-kernel

> Linux zram backend kernel module for FPGA-accelerated memory compression.
>
> Open-source companion to **FerroMem**, an AWS F2 Marketplace AMI that offloads zram compression to FPGA hardware.

[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![Status](https://img.shields.io/badge/status-skeleton-yellow.svg)]()

## What is FerroMem?

FerroMem accelerates Linux `zram` (and `zswap`) by offloading LZ4 / Zstd / Snappy compression to an FPGA. On AWS EC2 F2 instances this cuts effective memory cost by **~50%** vs equivalent r7i memory-optimized instances.

| Component | License | Distribution |
|---|---|---|
| **`ferromem-zram.ko`** (this repo) | Apache 2.0 (dual-licensed GPLv2 for kernel loading) | GitHub source / DKMS package |
| `ferromem-compress.so` (user-space lib) | Apache 2.0 | GitHub source |
| FPGA bitstream (`.awsxclbin`) | Proprietary | AWS Marketplace AMI bundle only |
| Daemon / UI / metering | Proprietary | AWS Marketplace AMI bundle only |

The kernel module is freely usable, but **without the proprietary FPGA bitstream** delivered through the AWS Marketplace AMI, `/dev/ferromem-zram` returns `EOPNOTSUPP` on every operation. Fork-friendly, lockin-free: any party can re-implement the bitstream side and continue using the OSS kernel layer.

## Status

> **2026-05-16: Skeleton release (v0.0.1)** — design documents and stubs only. Working implementation is targeted for Phase 2 month 3 (~3 months from now). See [docs/architecture.md](docs/architecture.md) and the parent project's `08-aws-integration/xrt-zram-driver-design.md`.

## Quick start (once implemented)

### Via AWS Marketplace (recommended for AWS F2 users)

```bash
# Subscribe to FerroMem AMI from AWS Marketplace, launch f2.6xlarge
# Module loads automatically; just enable zram with the ferromem backend:
echo ferromem > /sys/block/zram0/comp_algorithm
mkswap /dev/zram0
swapon /dev/zram0
```

### Build from source (other clouds / on-prem)

```bash
# Requires Linux kernel 6.8+ headers, XRT, and an FPGA bitstream
git clone https://github.com/abyo-software/ferromem-kernel.git
cd ferromem-kernel
make
sudo insmod src/ferromem_zram.ko algorithm=lz4 target_gb=256
echo ferromem > /sys/block/zram0/comp_algorithm
```

The `make` step builds against your running kernel by default; cross-build via `KERNELDIR=...`.

## DKMS

```bash
sudo dkms add .
sudo dkms install ferromem/0.0.1
```

Tested on Ubuntu 24.04 LTS (kernel 6.8.x). See [docs/installation.md](docs/installation.md) for details once implemented.

## Architecture

```
┌──────────────────────────────────────────────────────────┐
│   Linux kernel (zram / zswap subsystem)                  │
└──────────┬───────────────────────────────────────────────┘
           │ zcomp_backend interface
┌──────────▼───────────────────────────────────────────────┐
│   ferromem-zram.ko       (this repo, Apache 2.0)         │
└──────────┬───────────────────────────────────────────────┘
           │ XRT command queue / netlink to user-space
┌──────────▼───────────────────────────────────────────────┐
│   ferromem-daemon        (proprietary, Marketplace AMI)  │
└──────────┬───────────────────────────────────────────────┘
           │ XRT runtime
┌──────────▼───────────────────────────────────────────────┐
│   FPGA bitstream         (proprietary, Marketplace AMI)  │
│      LZ4 / Zstd / Snappy encoder + decoder               │
└──────────────────────────────────────────────────────────┘
```

See [docs/architecture.md](docs/architecture.md) for the full design (4 KB page flow, batched mode, error paths).

## Repository layout

```
ferromem-kernel/
├── README.md              ← this file
├── LICENSE                ← Apache 2.0
├── NOTICE                 ← third-party attribution
├── DCO.md                 ← Developer Certificate of Origin (sign-off required)
├── CONTRIBUTING.md
├── CODE_OF_CONDUCT.md
├── SECURITY.md            ← security@ferromem.io
├── Makefile               ← kbuild driver
├── dkms.conf              ← DKMS package metadata
├── src/                   ← kernel module source
│   ├── ferromem_main.c
│   ├── ferromem_zram.c
│   ├── ferromem_xrt.c
│   ├── ferromem_metrics.c
│   ├── ferromem_ioctl.c
│   └── include/
├── lib/                   ← user-space library source (Apache 2.0)
├── tests/
│   ├── unit/              ← kunit
│   └── integration/       ← zram round-trip
├── docs/                  ← architecture / installation / API / benchmarks
└── .github/workflows/     ← CI (build + lint)
```

## Roadmap

| Milestone | Target | Status |
|---|---|---|
| v0.0.1 — skeleton release (design docs + stubs) | 2026-05 | ✅ Done |
| v0.1.0 — working build with AWS F2 (loadable, zram backend registered) | Phase 2 month 3 (2026-08) | 🚧 |
| v0.2.0 — Marketplace launch sync, DKMS package, Ubuntu 24.04 verified | Phase 3 month 1 (2026-09) | 📋 |
| v0.3.0 — batched-mode compress (32–128 pages) | Phase 3 month 4 | 📋 |
| v1.0.0 — production grade, 5+ external contributors | Phase 4 (2027 Q1) | 📋 |
| upstream RFC | Phase 5+ (community traction dependent) | 📋 |

## Contributing

1. Read [CONTRIBUTING.md](CONTRIBUTING.md).
2. Sign off your commits per [DCO.md](DCO.md) (`git commit -s`).
3. Open a PR against `main`.

Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before participating.

## Reporting security issues

Please email **security@ferromem.io** rather than filing a public issue. See [SECURITY.md](SECURITY.md).

## License

Apache License, Version 2.0. See [LICENSE](LICENSE).

The kernel module is dual-licensed under GPLv2 *for the purpose of loading into the Linux kernel only* (per `MODULE_LICENSE("GPL")`); source distribution remains Apache 2.0.

`ferromem-zram.ko` does not include or link against the proprietary AWS Marketplace AMI components. Those components have their own EULA available on the Marketplace listing.

## Trademarks

"FerroMem" is a trademark of **abyo-software G.K.** (合同会社 abyo-software, a Japanese Limited Liability Company / Godo Kaisha). This Apache 2.0 license does not grant trademark rights. Forks may use the source but must rebrand. See [LICENSE](LICENSE) §6 and the `NOTICE` file.

## Links

- AWS Marketplace listing: *coming Phase 3*
- Documentation: [docs/](docs/)
- Issues: https://github.com/abyo-software/ferromem-kernel/issues
- Discussions: https://github.com/abyo-software/ferromem-kernel/discussions
