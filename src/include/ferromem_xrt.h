/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0 */
/* Copyright 2026 abyo-software G.K. */

#ifndef FERROMEM_XRT_H
#define FERROMEM_XRT_H

#include "ferromem.h"

/* XRT command queue interface to the proprietary daemon / AFI bitstream.
 *
 * Phase 2 month 3 decision: kernel-direct (/dev/xclmgmt ioctl) vs
 * user-space-daemon (netlink). See docs/architecture.md §3.
 *
 * Skeleton stubs return -EOPNOTSUPP until AFI bitstream is loaded.
 */

int  ferromem_xrt_init(void);
void ferromem_xrt_exit(void);

int  ferromem_xrt_compress(const void *src, size_t src_len,
			   void *dst, size_t *dst_len,
			   enum ferromem_algorithm alg);

int  ferromem_xrt_decompress(const void *src, size_t src_len,
			     void *dst, size_t dst_len,
			     enum ferromem_algorithm alg);

bool ferromem_xrt_afi_loaded(void);

#endif /* FERROMEM_XRT_H */
