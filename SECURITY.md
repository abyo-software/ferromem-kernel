# Security Policy

## Reporting a vulnerability

If you discover a security vulnerability in ferromem-kernel, **please do not file a public GitHub issue**. Instead:

1. Email **security@ferromem.io** with details:
   - Affected component (kernel module, user-space lib, build tooling)
   - Affected version(s)
   - Reproduction steps or proof-of-concept
   - Your suggested severity (informational / low / medium / high / critical)
   - Whether you would like credit on the security advisory (and what name to use)

2. We will acknowledge receipt within **5 business days**.

3. We aim to publish a fix and coordinated security advisory within **90 days** of the report, sooner for high/critical severity.

## Scope

This policy covers:

- `src/` — the OSS kernel module
- `lib/` — the OSS user-space library
- `Makefile`, `dkms.conf`, build / packaging tooling

This policy **does not** cover:

- The proprietary AWS Marketplace AMI components (daemon, AFI bitstream, dashboard). For those, please use the AMI's support channel listed on the Marketplace listing.
- The Linux kernel itself or upstream zram/zswap. Report kernel vulnerabilities to security@kernel.org and the relevant maintainers.
- AWS infrastructure issues. Report to AWS Security at https://aws.amazon.com/security/vulnerability-reporting/.
- XRT (Xilinx Runtime). Report via the upstream XRT project.

## Supported versions

| Version | Supported |
|---|---|
| v0.0.x (skeleton) | ⚠️ Best-effort only |
| v0.1.x and later | ✅ |

We commit to supporting the latest minor release and the previous minor release with security fixes. Older releases may receive critical-only fixes.

## Disclosure

Once a fix is available, we will publish a security advisory on GitHub (`Security` tab → `Advisories`) and a CVE if applicable. Reporters who would like credit will be acknowledged in the advisory.
