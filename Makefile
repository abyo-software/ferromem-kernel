# ferromem-kernel — Linux kernel module build
#
# Usage:
#   make              build against the running kernel
#   make KERNELDIR=/path/to/kernel/build   build against a specific kernel
#   make clean        remove build artifacts
#
# Module: ferromem_zram.ko
# License: Apache-2.0 (this source); GPL (loadable into Linux kernel)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

obj-m := ferromem_zram.o
ferromem_zram-y := \
	src/ferromem_main.o \
	src/ferromem_zram.o \
	src/ferromem_xrt.o \
	src/ferromem_metrics.o \
	src/ferromem_ioctl.o

ccflags-y := -I$(PWD)/src/include -DFERROMEM_VERSION=\"0.0.1\"

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f src/*.o src/*.ko src/*.mod* src/.*.cmd src/.*.o.cmd
	rm -rf .tmp_versions
	rm -f Module.symvers modules.order

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	depmod -a

# Run kernel-style lint (requires kernel checkpatch.pl in $PATH or $KERNELDIR/scripts)
lint:
	@if command -v checkpatch.pl >/dev/null 2>&1; then \
		find src -name '*.c' -o -name '*.h' | xargs checkpatch.pl --no-tree --file --strict; \
	elif [ -x $(KERNELDIR)/scripts/checkpatch.pl ]; then \
		find src -name '*.c' -o -name '*.h' | xargs $(KERNELDIR)/scripts/checkpatch.pl --no-tree --file --strict; \
	else \
		echo "checkpatch.pl not found; install linux-headers and retry"; \
		exit 1; \
	fi

.PHONY: all clean install lint
