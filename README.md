[![ci](https://github.com/tillitis/tkey-libs/actions/workflows/ci.yaml/badge.svg?branch=main&event=push)](https://github.com/tillitis/tkey-libs/actions/workflows/ci.yaml)

# Device libraries for the Tillitis TKey

- C runtime: libcrt0.
- Common C functions including protocol calls: libcommon.
- Cryptographic functions: libmonocypher.
  Based on monocypher version 4.0.2
  https://github.com/LoupVaillant/Monocypher

Relase notes in [RELEASE.md](RELEASE.md).

## Licenses and SPDX tags

Unless otherwise noted, the project sources are licensed under the
terms and conditions of the "GNU General Public License v2.0 only":

> Copyright Tillitis AB.
>
> These programs are free software: you can redistribute it and/or
> modify it under the terms of the GNU General Public License as
> published by the Free Software Foundation, version 2 only.
>
> These programs are distributed in the hope that they will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
> General Public License for more details.

> You should have received a copy of the GNU General Public License
> along with this program. If not, see:
>
> https://www.gnu.org/licenses

See [LICENSE](LICENSE) for the full GPLv2-only license text.

External source code we have imported are isolated in their own
directories. They may be released under other licenses. This is noted
with a similar `LICENSE` file in every directory containing imported
sources.

The project uses single-line references to Unique License Identifiers
as defined by the Linux Foundation's [SPDX project](https://spdx.org/)
on its own source files, but not necessarily imported files. The line
in each individual source file identifies the license applicable to
that file.

The current set of valid, predefined SPDX identifiers can be found on
the SPDX License List at:

https://spdx.org/licenses/

## Building
In order to build, you must have the `make`, `clang`, `llvm`, and
`lld` packages installed.

Version 15 or higher of LLVM/Clang is necessary for the RV32IC\_Zmmul
architecture we are using. For more detailed information on the
supported build and development environment, please refer to the
[Developer Handbook](https://dev.tillitis.se/).
## Building using Podman

You can also build the libraries with our OCI image
`ghcr.io/tillitis/tkey-builder`.

The easiest way to build this is if you have `make` installed:

```
make podman
```

You can also specify a different image by using
`IMAGE=localhost/tkey-builder-local`.

Or use Podman directly:

```
podman run --rm --mount type=bind,source=.,target=/src -w /src -it ghcr.io/tillitis/tkey-builder:4 make -j
```

## Minimal application build

You will typically need to link at least the `libcrt0` C runtime
otherwise your program won't even reach `main()`.

We provide a linker script in `apps.lds` which shows the linker the
memory layout.

Minimal compilation would look something like:

```
clang -target riscv32-unknown-none-elf -march=rv32iczmmul -mabi=ilp32 \
  -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common \
  -fno-builtin-printf -fno-builtin-putchar -nostdlib -mno-relax -flto \
  -Wall -Werror=implicit-function-declaration \
  -I ../tkey-libs/include \
  -I ../tkey-libs -c -o foo.o foo.c

clang -target riscv32-unknown-none-elf -march=rv32iczmmul -mabi=ilp32 \
  -mcmodel=medany -static -ffast-math -fno-common -nostdlib \
  -T ../tkey-libs/app.lds \
  -L ../tkey-libs -lcrt0 \
  -I ../tkey-libs -o foo.elf foo.o

```

## Makefile example

See `example-app/Makefile` for an example Makefile for a simple device
application.

## Debug output

If you're running the device app on our qemu emulator we have added a
debug port on 0xfe00\_1000 (`TK1_MMIO_QEMU_DEBUG`). Anything written
there will be printed as a character by qemu on the console.

`qemu_putchar()`, `qemu_puts()`, `qemu_putinthex()`, `qemu_hexdump()`
and friends (see `include/tkey/qemu_debug.h` for list of functions)
use this debug port to print stuff.

If you want to use these, define `QEMU_DEBUG` when compiling your
program, othwerwise all `qemu_*()` functions in your program are
removed by the C pre-processor.

`tkey-libs` own use of `qemu_*()` is limited to output from
`assert()`. This means that `QEMU_DEBUG` should be defined when
compiling `tkey-libs` itself!
