# Benchmarks

> Placeholder for benchmark data. Populated from v0.1.0 onwards.

## Methodology

All benchmarks are run on:

- AWS EC2 `f2.6xlarge` (1 × VU47P, 24 vCPU EPYC Milan, 256 GiB RAM, Ubuntu 24.04 LTS, kernel 6.8)
- FerroMem AMI v0.1.0+ with proprietary AFI bitstream loaded
- zram device size 256 GB
- 4 KB page size
- Algorithm: `lz4` unless noted otherwise

## Microbenchmark (planned for v0.1.0)

| Test | Metric | Target | Result |
|---|---|---|---|
| 4 KB random page compress | latency p50 | ≤ 1.5 μs | TBD |
| 4 KB random page compress | latency p99 | ≤ 3 μs | TBD |
| 4 KB random page decompress | latency p50 | ≤ 1.0 μs | TBD |
| 32-page batch compress | throughput | ≥ 2 GB/s | TBD |
| zram realistic workload | compression ratio | ≥ 1.75× | TBD |

## Workload comparison (planned for v0.2.0)

Compare FerroMem against CPU `lz4` and CPU `zstd-1` under representative zram workloads:

| Workload | CPU lz4 | CPU zstd-1 | FerroMem (v0.1.0) | FerroMem (v0.3.0 batched) |
|---|---|---|---|---|
| TMO production trace | TBD | TBD | TBD | TBD |
| Linux process memory | TBD | TBD | TBD | TBD |
| Random pages | TBD | TBD | TBD | TBD |
| Log replay | TBD | TBD | TBD | TBD |

CPU cycles saved (and thus user workload throughput uplift) will be the primary metric.

## Reference baselines

For context until our v0.1.0 numbers land:

| Implementation | 4 KB page (compress) | Source |
|---|---|---|
| CPU `lz4` (single core) | ~150 ns | mainline Linux kernel benchmarks |
| CPU `zstd-1` (single core) | ~500 ns | upstream zstd benchmarks |
| Intel IAA (DEFLATE) | ~700 ns | Alibaba Anolis IAA blog |
| **FerroMem v0.1.0 (target)** | **~1.5 μs** | This project (4 KB per page incl PCIe roundtrip) |
| **FerroMem v0.3.0 (target, batched)** | **~800 ns** | This project (amortized over 32-page batch) |

FerroMem is slower per-page than CPU `lz4` but frees a CPU core for user workload. The ROI is significant when memory pressure dominates total cost.
