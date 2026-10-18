SHELL := /bin/sh
.PHONY: all clean test inspect

# Original UEFI application + freestanding kernel. No Linux kernel or rootfs.
all:
	$(MAKE) -C milestone1 all

test:
	$(MAKE) -C milestone1 test

clean:
	$(MAKE) -C milestone1 clean

inspect:
	@git ls-files
	@printf '\nTracked file count: '
	@git ls-files | wc -l
