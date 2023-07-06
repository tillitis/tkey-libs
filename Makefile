OBJCOPY ?= llvm-objcopy

CC = clang

INCLUDE=include

# If you want libcommon's qemu_puts() et cetera to output something on our QEMU
# debug port, remove -DNODEBUG below
CFLAGS = -target riscv32-unknown-none-elf -march=rv32iczmmul -mabi=ilp32 -mcmodel=medany \
   -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf \
   -fno-builtin-putchar -nostdlib -mno-relax -flto \
   -Wall -Werror=implicit-function-declaration \
   -I $(INCLUDE) -I . \
   -DNODEBUG
ifneq ($(TKEY_SIGNER_APP_NO_TOUCH),)
CFLAGS := $(CFLAGS) -DTKEY_SIGNER_APP_NO_TOUCH
endif

AS = clang
ASFLAGS = -target riscv32-unknown-none-elf -march=rv32iczmmul -mabi=ilp32 -mcmodel=medany -mno-relax

LDFLAGS=-T app.lds -L libcommon/ -lcommon -L libcrt0/ -lcrt0


.PHONY: all
all: libcrt0.a libcommon.a libmonocypher.a

podman:
	podman run --rm --mount type=bind,source=$(CURDIR),target=/src -w /src -it ghcr.io/tillitis/tkey-builder:2 make -j

.PHONY: check
check:
	clang-tidy -header-filter=.* -checks=cert-* libcommon/*.c -- $(CFLAGS)

# C runtime library
libcrt0.a: libcrt0/crt0.o
	llvm-ar -qc $@ libcrt0/crt0.o

# Common C functions
LIBOBJS=libcommon/assert.o libcommon/blake2s.o libcommon/led.o libcommon/lib.o libcommon/proto.o libcommon/qemu_debug.o
libcommon.a: $(LIBOBJS)
	llvm-ar -qc $@ $(LIBOBJS)
$(LIBOBJS): include/blake2s.h include/types.h include/tk1_mem.h include/lib.h include/proto.h include/led.h include/assert.h include/qemu_debug.h

# Monocypher
MONOOBJS=monocypher/monocypher.o monocypher/monocypher-ed25519.o
libmonocypher.a: $(MONOOBJS)
	llvm-ar -qc $@ $(MONOOBJS)
$MONOOBJS: monocypher/monocypher-ed25519.h monocypher/monocypher.h

LIBS=libcrt0.a libcommon.a

.PHONY: clean
clean:
	rm -f $(LIBS) $(LIBOBJS) libcrt0/crt0.o
	rm -f libmonocypher.a $(MONOOBJS)

# Uses ../.clang-format
FMTFILES=include/*.h libcommon/*.c
.PHONY: fmt
fmt:
	clang-format --dry-run --ferror-limit=0 $(FMTFILES)
	clang-format --verbose -i $(FMTFILES)
.PHONY: checkfmt
checkfmt:
	clang-format --dry-run --ferror-limit=0 --Werror $(FMTFILES)

.PHONY: update-mem-include
update-mem-include:
	cp -af ../tillitis-key1/hw/application_fpga/fw/tk1_mem.h include/tk1_mem.h
