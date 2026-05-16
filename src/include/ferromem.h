/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0 */
/*
 * ferromem.h — common definitions for the FerroMem kernel module
 *
 * Copyright 2026 abyo-software K.K.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * For loading into the Linux kernel, this file is additionally available
 * under the GPLv2 (per MODULE_LICENSE("GPL") in ferromem_main.c).
 */

#ifndef FERROMEM_H
#define FERROMEM_H

#include <linux/types.h>

#define FERROMEM_DRV_NAME    "ferromem"
#define FERROMEM_DEVICE_NAME "ferromem-zram"

/* Module parameters (defaults; overridable at insmod) */
#define FERROMEM_DEFAULT_ALGORITHM "lz4"
#define FERROMEM_DEFAULT_TARGET_GB 256

/* Supported algorithms */
enum ferromem_algorithm {
	FERROMEM_ALG_LZ4 = 0,
	FERROMEM_ALG_ZSTD,
	FERROMEM_ALG_SNAPPY,
	FERROMEM_ALG_MAX,
};

/* ioctl numbers */
#define FERROMEM_IOC_MAGIC      'F'
#define FERROMEM_IOC_GET_STATS  _IOR(FERROMEM_IOC_MAGIC, 1, struct ferromem_stats)
#define FERROMEM_IOC_SET_ALG    _IOW(FERROMEM_IOC_MAGIC, 2, int)
#define FERROMEM_IOC_RESET      _IO(FERROMEM_IOC_MAGIC,  3)

struct ferromem_stats {
	__u64 bytes_in;
	__u64 bytes_out;
	__u64 compressions;
	__u64 decompressions;
	__u64 errors;
	__u32 queue_depth;
	__u32 algorithm;          /* enum ferromem_algorithm */
	__u32 afi_loaded;         /* 0 = no AFI, 1 = AFI ready */
	__u32 reserved;
};

#endif /* FERROMEM_H */
