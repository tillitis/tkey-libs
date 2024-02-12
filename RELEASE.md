# Release notes


## v0.1.0

This release contains some changes that forces applications that use
tkey-libs to be updated to work with this release.

Note: It is highly likely that this update will affect the CDI of the
TKey. It is advised to always verify this for each specific app, for
every update. If the CDI changes, and it is applicable, it should be
stated clearly to end users to avoid unknowingly changing the TKey
identity.

Breaking changes:
- Check destination buffer's size for read(). To prevent writing
  outside of destination buffer.
- Renaming LED-functions to follow led_*().

Changes:
- New function, secure_wipe(), to clean memory of secret data.
- New function, touch_wait(). Waits for a touch by the user, with
  selectable timeout.
- New function, led_get(). Get the value of the applied LED color.
- Upgraded Monocypher to 4.0.2.
- Add variable AR in Makefile to enabling passing llvm-ar from command
  line.
- Update example app to use led.h.
- Don't have QEMU debug enabled by default.
- Minor tweaks and formatting.

Full changelog:
[v0.0.2...v0.1.0](https://github.com/tillitis/tkey-libs/compare/v0.0.2...v0.1.0)

## v0.0.2

This release contains some changes that forces applications that use
tkey-libs to be updated to work with this release.

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
