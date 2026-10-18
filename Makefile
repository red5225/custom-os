SHELL := /bin/sh
.PHONY: all clean test inspect xc-test

all:
	$(MAKE) -C boot/uefi all

test:
	$(MAKE) -C boot/uefi test

clean:
	$(MAKE) -C boot/uefi clean

xc-test:
	g++ -std=c++17 -Wall -Wextra -Werror tools/xc/xc.cpp -o /tmp/customos-xc
	@set -eu; for f in tests/xc/*.xc; do /tmp/customos-xc "$$f" >/dev/null; done

inspect:
	@git ls-files
	@printf '\nTracked file count: '
	@git ls-files | wc -l
