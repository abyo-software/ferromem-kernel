# Installation

> v0.0.1 (skeleton): the module builds and loads but every operation returns `-EOPNOTSUPP`. Useful for design review and CI scaffolding only. The first working release is v0.1.0 (~2026-08).

## Prerequisites

- Linux kernel **6.8+** (Ubuntu 24.04 LTS default; Rocky 9.x via backport)
- `linux-headers-$(uname -r)` package
- `make`, `gcc` matching kernel ABI
- (Optional, runtime) XRT runtime and an FPGA bitstream — needed for actual hardware acceleration

For an AWS F2 instance launched from the FerroMem Marketplace AMI, the module is **pre-installed** at `/opt/ferromem/kernel/ferromem_zram.ko` together with the proprietary AFI bitstream. You don't need to build from source.

## Build from source

```bash
git clone https://github.com/abyo-software/ferromem-kernel.git
cd ferromem-kernel
make
```

Build artifact: `ferromem_zram.ko` at the top of the repo.

To target a specific kernel:

```bash
make KERNELDIR=/path/to/kernel/build
```

## DKMS

```bash
sudo cp -r . /usr/src/ferromem-0.0.1
sudo dkms add -m ferromem -v 0.0.1
sudo dkms install -m ferromem -v 0.0.1
```

## Load

```bash
sudo insmod ferromem_zram.ko algorithm=lz4 target_gb=256
dmesg | tail
```

Expected output:

```
ferromem: v0.0.1 init (algorithm=lz4 target_gb=256)
ferromem_xrt: init (skeleton — AFI not loaded)
ferromem_zram: register (skeleton — backend not yet active)
ferromem: ready (AFI=absent — operations return -EOPNOTSUPP)
```

The `AFI=absent` line is expected in v0.0.1 — there's no working backend yet.

## Enable as zram backend (v0.1.0+)

Once v0.1.0 lands and an AFI bitstream is loaded:

```bash
echo ferromem > /sys/block/zram0/comp_algorithm
echo $((256 * 1024 * 1024 * 1024)) > /sys/block/zram0/disksize
mkswap /dev/zram0
swapon /dev/zram0
```

## Unload

```bash
sudo rmmod ferromem_zram
```

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| `insmod: No such file or directory` | Module not built for running kernel | `make KERNELDIR=$(uname -r) clean && make` |
| `insmod: Operation not permitted` | Not root | Use `sudo` |
| `ferromem: XRT init failed` in dmesg | XRT not installed / FPGA absent | Expected on non-FPGA hosts; module stays in stub mode |
| Operations return `-EOPNOTSUPP` | No AFI loaded | Load AFI via Marketplace AMI tooling or build your own bitstream |

## Removal

```bash
sudo dkms remove -m ferromem -v 0.0.1 --all
sudo rm -rf /usr/src/ferromem-0.0.1
```
