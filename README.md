Device libraries for the Tillitis TKey

- C runtime: libcrt0.
- Common C function: libcommon.
- Protocol functions: libproto.
- Cryptographic functions: libmonocypher.
  Based on monocypher version 3.1.3.
  https://github.com/LoupVaillant/Monocypher

## Building

To build you need at least `make`, `clang`, `llvm`, and `lld` packages installed. 

Version 15 or later of LLVM/Clang is required for the RV32IC\_Zmmul
architecture we use. Ubuntu 22.10 (Kinetic) is known to work. Please
see
[toolchain_setup.md](https://github.com/tillitis/tillitis-key1/blob/main/doc/toolchain_setup.md)
(in the tillitis-key1 repository) for detailed information on the
currently supported build and development environment.

## Building using podman

You can also build the libraries with our OCI image
`ghcr.io/tillitis/tkey-builder`. 

The easiest way to build this is if you have `make` installed:

```
make podman
```

Or use podman directly:

```
podman run --rm --mount type=bind,source=.,target=/src -w /src -it ghcr.io/tillitis/tkey-builder:1 make -j
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
  -L ../tkey-libs/libcrt0/ -lcrt0 \
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
and friends (see `libcommon/lib.[ch]`) use this debug port to print
stuff.

`libcommon` is compiled with no debug output by default. Rebuild
`libcommon` without `-DNODEBUG` to get the debug output.
