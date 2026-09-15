# Security Policy

## Supported Versions

minil follows a rolling-release development model.

| Version | Supported          |
| ------- | ------------------ |
| rolling | :white_check_mark: |

## Reporting a Vulnerability

If you discover a security issue in minil, please open a GitHub issue.

For sensitive reports, contact the maintainer before public disclosure.

Please include:

- affected architecture (x86_64, i386, AArch64, RISC-V)
- steps to reproduce
- expected behavior
- actual behavior
- proof of concept (if available)

The maintainer will review the report and determine whether the issue
is a valid security vulnerability or a regular bug.

Because minil is a low-level freestanding runtime that interacts directly
with the Linux ABI and system calls, vulnerabilities related to process
startup, syscall wrappers, ABI handling, or memory safety are considered
high priority.

minil is experimental software distributed under CC0 1.0.
