// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0
/*
 * ferromem_main.c — kernel module init / exit
 *
 * Copyright 2026 abyo-software G.K.
 * Skeleton (v0.0.1) — working implementation lands Phase 2 month 3.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "include/ferromem.h"
#include "include/ferromem_zram.h"
#include "include/ferromem_xrt.h"

/* Module params (insmod algorithm=lz4 target_gb=256 ...) */
static char *algorithm = FERROMEM_DEFAULT_ALGORITHM;
module_param(algorithm, charp, 0444);
MODULE_PARM_DESC(algorithm, "Compression algorithm: lz4 | zstd | snappy");

static int target_gb = FERROMEM_DEFAULT_TARGET_GB;
module_param(target_gb, int, 0444);
MODULE_PARM_DESC(target_gb, "Target zram backend size in GB");

static int __init ferromem_init(void)
{
	int ret;

	pr_info("ferromem: v%s init (algorithm=%s target_gb=%d)\n",
		FERROMEM_VERSION, algorithm, target_gb);

	ret = ferromem_xrt_init();
	if (ret) {
		pr_warn("ferromem: XRT init failed (%d), running in stub mode\n", ret);
		/* Continue: skeleton mode still registers the zcomp backend */
	}

	ret = ferromem_zram_register();
	if (ret) {
		pr_err("ferromem: zram register failed (%d)\n", ret);
		ferromem_xrt_exit();
		return ret;
	}

	pr_info("ferromem: ready (AFI=%s)\n",
		ferromem_xrt_afi_loaded() ? "loaded" : "absent — operations return -EOPNOTSUPP");
	return 0;
}

static void __exit ferromem_exit(void)
{
	ferromem_zram_unregister();
	ferromem_xrt_exit();
	pr_info("ferromem: unloaded\n");
}

module_init(ferromem_init);
module_exit(ferromem_exit);

MODULE_LICENSE("GPL");                /* loadable into Linux kernel; source dual-licensed Apache-2.0 */
MODULE_AUTHOR("abyo-software G.K.");
MODULE_DESCRIPTION("Linux zram backend for FPGA-accelerated memory compression (FerroMem)");
MODULE_VERSION("0.0.1");
