// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0
/*
 * ferromem_xrt.c — XRT command queue (skeleton)
 *
 * Copyright 2026 abyo-software G.K.
 * Without the proprietary AFI bitstream from the AWS Marketplace AMI
 * (or a fork-built equivalent), all operations return -EOPNOTSUPP.
 */

#include <linux/kernel.h>
#include <linux/errno.h>

#include "include/ferromem.h"
#include "include/ferromem_xrt.h"

static bool g_afi_loaded;

int ferromem_xrt_init(void)
{
	/* TODO Phase 2 m3:
	 *  - probe /dev/xclmgmt* via fpga_mgr API or open() + ioctl()
	 *  - verify AFI signature against built-in public key
	 *  - set g_afi_loaded = true on success
	 */
	g_afi_loaded = false;
	pr_info("ferromem_xrt: init (skeleton — AFI not loaded)\n");
	return 0;
}

void ferromem_xrt_exit(void)
{
	g_afi_loaded = false;
}

bool ferromem_xrt_afi_loaded(void)
{
	return g_afi_loaded;
}

int ferromem_xrt_compress(const void *src, size_t src_len,
			  void *dst, size_t *dst_len,
			  enum ferromem_algorithm alg)
{
	(void)src; (void)src_len; (void)dst; (void)dst_len; (void)alg;
	if (!g_afi_loaded)
		return -EOPNOTSUPP;
	/* TODO Phase 2 m3: enqueue compress command on XRT command queue */
	return -EOPNOTSUPP;
}

int ferromem_xrt_decompress(const void *src, size_t src_len,
			    void *dst, size_t dst_len,
			    enum ferromem_algorithm alg)
{
	(void)src; (void)src_len; (void)dst; (void)dst_len; (void)alg;
	if (!g_afi_loaded)
		return -EOPNOTSUPP;
	/* TODO Phase 2 m3: enqueue decompress command on XRT command queue */
	return -EOPNOTSUPP;
}
