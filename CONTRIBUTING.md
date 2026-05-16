# Contributing to ferromem-kernel

Thank you for considering a contribution to ferromem-kernel. This document explains how to propose changes.

## Project status

This repository is in **skeleton stage** (v0.0.1). Working implementation lands in Phase 2 month 3 (~2026-08). Until then, contributions are most useful as:

- design review comments on `docs/architecture.md`
- additional benchmark / corpus suggestions in `tests/` planning
- documentation improvements

Code contributions to `src/` are very welcome once the v0.1.0 release lands; please watch the repo or open an Issue if you want a heads-up.

## Getting started

1. Fork the repository on GitHub.
2. Clone your fork locally:
   ```
   git clone https://github.com/<your-username>/ferromem-kernel.git
   cd ferromem-kernel
   ```
3. Create a branch for your change:
   ```
   git checkout -b my-improvement
   ```
4. Make your change with clear, focused commits.
5. **Sign off your commits** per [DCO.md](DCO.md):
   ```
   git commit -s -m "Subject of change"
   ```
6. Push and open a pull request to `main`.

## Coding standards

### Kernel module (`src/`)

- Follow Linux kernel coding style (`scripts/checkpatch.pl --strict`).
- Prefer existing kernel APIs over re-implementations.
- Use `pr_*()` / `dev_*()` for logging; no `printk` direct.
- All public functions get kernel-doc comments.
- New ABI (ioctl, sysfs, module params) requires a doc entry under `docs/api.md`.

### Tests (`tests/`)

- Unit tests use KUnit framework, in `tests/unit/`.
- Integration tests assume an F2 instance with AFI loaded; place under `tests/integration/`.
- Each test should be self-contained and clean up after itself.

### Documentation (`docs/`)

- Markdown; line-wrap ~80 cols where practical.
- Architecture decisions go into `docs/architecture.md` ADR-style section.

## CI

GitHub Actions runs on every pull request:

- `kernel-build.yml` — compile against linux kernel 6.8 and 6.10 headers
- `lint.yml` — checkpatch.pl + sparse + clang-format diff check

PRs must be green before merge.

## License of contributions

By contributing, you agree that your contribution is licensed under the Apache License, Version 2.0 (see [LICENSE](LICENSE)). For Linux kernel loading, the module is dual-licensed under GPLv2 (per `MODULE_LICENSE("GPL")`); source distribution remains Apache 2.0.

## Reporting bugs

- For bugs in the public OSS layer (kernel module, user-space library): open an Issue with reproduction steps, kernel version, and `dmesg` output around the failure.
- For security vulnerabilities: please email **security@ferromem.io** instead of opening a public issue. See [SECURITY.md](SECURITY.md).
- For issues in the proprietary AWS Marketplace AMI components (daemon, AFI, dashboard): use the AMI's support channel, not this repository.

## Questions

- General usage: GitHub Discussions
- Design / architecture: open an Issue tagged `design`
- Roadmap / planning: see `README.md` Roadmap table, comment on the linked Issues

Thank you for helping make FerroMem better.
