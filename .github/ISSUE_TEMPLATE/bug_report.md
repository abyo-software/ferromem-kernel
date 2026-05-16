---
name: Bug report
about: Report a bug in the OSS kernel module or build tooling
title: '[BUG] '
labels: bug
---

> **NOTE**: For security vulnerabilities, please email **security@ferromem.io** instead of using this form. See [SECURITY.md](../../SECURITY.md).
>
> For issues with the proprietary AWS Marketplace AMI components (daemon, AFI, dashboard), please use the AMI's support channel listed on the Marketplace listing. This issue tracker covers the OSS kernel module only.

## Summary

A clear and concise description of the bug.

## Environment

- ferromem-kernel version (`modinfo ferromem_zram | grep version`):
- Kernel version (`uname -r`):
- Distribution (Ubuntu / RHEL / Rocky / etc.):
- FPGA platform (AWS F2 / onprem Alveo / other / N/A):
- AFI loaded (yes / no / N/A):

## Steps to reproduce

1.
2.
3.

## Expected behavior

What you expected to happen.

## Actual behavior

What actually happened.

## Logs

```
# paste dmesg output around the failure, redact any sensitive info
```

## Additional context

Anything else that might help us reproduce or diagnose.
