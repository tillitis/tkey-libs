Device libraries for the Tillitis TKey

- C runtime: libcrt0.
- Common C function: libcommon.
- Protocol functions: libproto.
- Cryptographic functions: libmonocypher.
  Based on monocypher version 3.1.3.
  https://github.com/LoupVaillant/Monocypher

## Debug output

If you're running the device app on our qemu emulator we have added a
debug port on 0xfe00\_1000 (`TK1_MMIO_QEMU_DEBUG`). Anything written
there will be printed as a character by qemu on the console.

`qemu_putchar()`, `qemu_puts()`, `qemu_putinthex()`, `qemu_hexdump()`
and friends (see `libcommon/lib.[ch]`) use this debug port to print
stuff.

`libcommon` is compiled with no debug output by default. Rebuild
`libcommon` without `-DNODEBUG` to get the debug output.
