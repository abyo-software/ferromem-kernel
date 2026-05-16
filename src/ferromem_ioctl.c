// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0
/*
 * ferromem_ioctl.c — char-device + ioctl interface for ferromem-cli
 *
 * Copyright 2026 abyo-software K.K.
 * Phase 2 m3 plan: /dev/ferromem-zram char device, FERROMEM_IOC_GET_STATS /
 *                  FERROMEM_IOC_SET_ALG / FERROMEM_IOC_RESET.
 */

#include <linux/kernel.h>

/* TODO Phase 2 m3:
 *  - misc_register() exposing /dev/ferromem-zram
 *  - file_operations.unlocked_ioctl dispatching FERROMEM_IOC_* commands
 *  - copy_to_user / copy_from_user for struct ferromem_stats
 */
