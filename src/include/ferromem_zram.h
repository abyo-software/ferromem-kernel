/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0 */
/* Copyright 2026 abyo-software G.K. */

#ifndef FERROMEM_ZRAM_H
#define FERROMEM_ZRAM_H

#include "ferromem.h"

/* Hooks invoked by the Linux zram subsystem (drivers/block/zram/zcomp.c).
 *
 * NOTE: These stubs currently return -EOPNOTSUPP. Real implementation lands
 *       in Phase 2 month 3 (~2026-08). See docs/architecture.md.
 */

int  ferromem_zram_register(void);
void ferromem_zram_unregister(void);

#endif /* FERROMEM_ZRAM_H */
