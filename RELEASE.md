# Release notes

## v0.0.2

This release contains some changes that forces applications that use
TKey-libs to be updated to work with this release.

Breaking changes:
- Introducing include hierarchy to make it less generic, e.g.,
  `#include <tkey/led.h>`.
- Use stdint.h/stddef.h infavor of types.h.
- Library .a files built on top level to simplify inclusion.
- Upgraded Monocypher to 4.0.1.
- QEMU debug behaviour changed, instead of defining `NODEBUG` to
  disable debug, one has to enable it by defining `QEMU_DEBUG`.

Changes:
- Introduce functions to control the LED, led.h and led.c.
- New function, assert() to make an illegal instruction and forcing
  the CPU to halt.
- Add functions memcpy_s(), wordcpy_s(), memeq() from firmware
- Adding `const` to MMIO variables and qemu_* functions.
- Minor tweaks, clean up and bugfixes.

Full changelog:
[v0.0.1...v0.0.2](https://github.com/tillitis/tkey-libs/compare/v0.0.1...v0.0.2)


## v0.0.1

Just ripped from

https://github.com/tillitis/tillitis-key1-apps

No semantic changes.
