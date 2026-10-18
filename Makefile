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
	test "$$(/tmp/customos-xc tests/xc/condition.xc)" = "condition true"
	@set -eu; for f in tests/xc/regression_*.xc apps/examples/*.xc; do /tmp/customos-xc "$$f" >/dev/null; done
	@if /tmp/customos-xc tests/xc/invalid.xc >/dev/null 2>&1; then echo 'invalid XC program was accepted'; exit 1; fi
	@if /tmp/customos-xc tests/xc/bad_value.xc >/dev/null 2>&1; then echo 'invalid XC value was accepted'; exit 1; fi

inspect:
	@git ls-files
	@printf '\nTracked file count: '
	@git ls-files | wc -l
