// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0
/*
 * ferromem_metrics.c — sysfs metrics exporter
 *
 * Copyright 2026 abyo-software K.K.
 * Phase 2 m3 plan: expose /sys/kernel/ferromem/{compression_ratio,
 *                                                throughput,queue_depth,errors}.
 */

#include <linux/kernel.h>

/* TODO Phase 2 m3:
 *  - kobject under /sys/kernel/ferromem/
 *  - read-only attributes backed by atomic counters in ferromem_xrt.c
 *  - Prometheus-friendly format (one value per file)
 */
