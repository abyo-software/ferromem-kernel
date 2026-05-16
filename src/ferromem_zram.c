// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0
/*
 * ferromem_zram.c — zram backend (zcomp_backend) implementation
 *
 * Copyright 2026 abyo-software G.K.
 * Skeleton — returns -EOPNOTSUPP until v0.1.0 (Phase 2 month 3).
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>

#include "include/ferromem.h"
#include "include/ferromem_zram.h"
#include "include/ferromem_xrt.h"

int ferromem_zram_register(void)
{
	pr_info("ferromem_zram: register (skeleton — backend not yet active)\n");
	/* TODO Phase 2 m3:
	 *  - zcomp_register(&ferromem_backend);
	 *  - struct zcomp_backend ferromem_backend with .compress / .decompress
	 *    forwarding to ferromem_xrt_compress / ferromem_xrt_decompress.
	 */
	return 0;
}

void ferromem_zram_unregister(void)
{
	pr_info("ferromem_zram: unregister\n");
	/* TODO Phase 2 m3: zcomp_unregister(&ferromem_backend); */
}
