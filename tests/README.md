# Tests

> Skeleton release (v0.0.1) — test scaffolding only. Real tests land with v0.1.0.

## Layout

```
tests/
├── README.md          ← this file
├── unit/              ← KUnit-based kernel tests (run inside kernel test runner)
└── integration/       ← user-space tests requiring an F2 instance + loaded AFI
```

## Unit tests (`tests/unit/`)

KUnit framework. Build with `make tests/unit` (TBD in v0.1.0), run via the standard KUnit runner:

```bash
./tools/testing/kunit/kunit.py run --kunitconfig=tests/unit/.kunitconfig
```

Planned cases:

- Backend register / unregister
- Module parameter parsing (algorithm, target_gb)
- Stub `-EOPNOTSUPP` paths when AFI not loaded
- sysfs attribute exposure
- ioctl plumbing (FERROMEM_IOC_GET_STATS / SET_ALG / RESET)

## Integration tests (`tests/integration/`)

Require an actual AWS F2 instance with a FerroMem AFI loaded (or equivalent FPGA setup on another cloud / onprem).

Planned scripts:

- `001_basic_load.sh` — load module, verify dmesg, unload cleanly
- `002_zram_register.sh` — `echo ferromem > /sys/block/zram0/comp_algorithm` and verify
- `003_roundtrip.sh` — write 100k random pages, read back, verify byte-equal
- `004_compression_ratio.sh` — write a realistic corpus (Linux memory dump), measure ratio ≥ 1.75×
- `005_throughput.sh` — bulk benchmark, expect ≥ 2 GB/s
- `006_chaos.sh` — kill AFI mid-flight, verify graceful degradation

## Soak (24 h)

Planned for v0.2.0: continuous zram swap-in/out for 24 hours, monitoring `/proc/meminfo`, `/proc/slabinfo`, and `/sys/kernel/ferromem/errors`. Goal: zero kmemleak hits and `errors == 0`.
