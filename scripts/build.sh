#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
VERSION="2026.08"
mkdir -p "$ROOT/dl" "$ROOT/out"
chmod +x "$ROOT/buildroot-external/board/customos/post-build.sh" "$ROOT/buildroot-external/board/customos/overlay/etc/init.d/S99customos" "$ROOT/buildroot-external/board/customos/overlay/usr/bin/customos" "$ROOT/buildroot-external/board/customos/overlay/usr/bin/customos-integrity" "$ROOT/buildroot-external/board/customos/overlay/usr/bin/customos-app"
cd "$ROOT"
if [ ! -d "buildroot-$VERSION" ]; then
  curl -fL "https://buildroot.org/downloads/buildroot-$VERSION.tar.xz" -o "dl/buildroot-$VERSION.tar.xz"
  tar -C "$ROOT" -xf "dl/buildroot-$VERSION.tar.xz"
fi
make -C "buildroot-$VERSION" O="$ROOT/out" BR2_EXTERNAL="$ROOT/buildroot-external" customos_x86_64_defconfig
make -C "buildroot-$VERSION" O="$ROOT/out" BR2_EXTERNAL="$ROOT/buildroot-external" -j"$(getconf _NPROCESSORS_ONLN)"
