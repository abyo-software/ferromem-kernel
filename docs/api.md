# API Reference

> Stable in v0.1.0 onward. The skeleton release (v0.0.1) implements only the module load/unload entry points.

## Module parameters (insmod)

| Parameter | Type | Default | Description |
|---|---|---|---|
| `algorithm` | string (charp) | `"lz4"` | Compression algorithm: `lz4`, `zstd`, `snappy` |
| `target_gb` | int | `256` | Target zram backend size in GB |

```bash
sudo insmod ferromem_zram.ko algorithm=lz4 target_gb=256
```

## sysfs (read-only, v0.1.0+)

Path: `/sys/kernel/ferromem/`

| File | Type | Description |
|---|---|---|
| `compression_ratio` | float | Current compression ratio (compressed bytes / original bytes) |
| `effective_memory_bytes` | u64 | Physical RAM + compressed zram (estimated) |
| `cpu_cycles_saved_total` | u64 | Cumulative CPU cycles avoided vs CPU `lz4` |
| `throughput_bytes_per_second` | u64 | Current compression throughput |
| `queue_depth` | u32 | Current XRT command queue depth |
| `errors` | u64 | Cumulative error count |
| `afi_loaded` | u32 | 1 if AFI bitstream loaded, 0 otherwise |
| `algorithm` | string | Currently selected algorithm |

## char device + ioctl (v0.1.0+)

Path: `/dev/ferromem-zram`

### `FERROMEM_IOC_GET_STATS`

Read-only ioctl returning a `struct ferromem_stats`. See `src/include/ferromem.h`.

```c
struct ferromem_stats stats;
int fd = open("/dev/ferromem-zram", O_RDONLY);
ioctl(fd, FERROMEM_IOC_GET_STATS, &stats);
printf("bytes_in=%llu bytes_out=%llu ratio=%.2f\n",
       stats.bytes_in, stats.bytes_out,
       (double)stats.bytes_in / stats.bytes_out);
close(fd);
```

### `FERROMEM_IOC_SET_ALG`

Switch algorithm at runtime. Argument: `int` matching `enum ferromem_algorithm`.

```c
int alg = FERROMEM_ALG_ZSTD;
ioctl(fd, FERROMEM_IOC_SET_ALG, &alg);
```

Returns `-EBUSY` if active compression is in flight; retry after `cat /sys/kernel/ferromem/queue_depth` returns 0.

### `FERROMEM_IOC_RESET`

Reset stats counters. No argument.

```c
ioctl(fd, FERROMEM_IOC_RESET);
```

## zram integration

After load, the backend is selectable from zram's standard sysfs:

```bash
echo ferromem > /sys/block/zram0/comp_algorithm
```

To verify selection:

```bash
cat /sys/block/zram0/comp_algorithm
# expected output contains: [ferromem] lzo lz4 zstd ...
```

## Return codes

The kernel module follows standard Linux `errno` conventions:

| errno | Meaning |
|---|---|
| `0` | Success |
| `-EOPNOTSUPP` | AFI bitstream not loaded (skeleton mode or AFI failure) |
| `-EBUSY` | XRT command queue full |
| `-ETIMEDOUT` | FPGA command exceeded timeout (default 10 ms) |
| `-EINVAL` | Invalid parameter (algorithm name, etc.) |
| `-ENOMEM` | Out of memory |
| `-EIO` | XRT / FPGA hardware error |

## C ABI for user-space library (Apache 2.0, `lib/`)

`lib/ferromem-compress.so` exposes a simple C interface:

```c
int ferromem_compress(const void *src, size_t src_len,
                      void *dst, size_t *dst_len,
                      const char *algorithm);

int ferromem_decompress(const void *src, size_t src_len,
                        void *dst, size_t dst_len,
                        const char *algorithm);
```

Implemented in v0.2.0+; the kernel module is sufficient for zram integration.
