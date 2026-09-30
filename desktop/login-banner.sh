#!/bin/sh
printf '\033[1;36m'
cat /usr/local/share/custom-os/boot-banner.txt 2>/dev/null || true
printf '\033[0m'
