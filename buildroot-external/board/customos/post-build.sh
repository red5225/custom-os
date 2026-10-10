#!/bin/sh
set -eu
TARGET_DIR="$1"
install -d "$TARGET_DIR/etc/customos" "$TARGET_DIR/opt/customos" "$TARGET_DIR/var/lib/customos" "$TARGET_DIR/var/log/customos"
printf '%s\n' 'CustomOS managed system files.' > "$TARGET_DIR/etc/customos/README"
chmod 0755 "$TARGET_DIR/etc/init.d/S99customos" "$TARGET_DIR/usr/bin/customos" "$TARGET_DIR/usr/bin/customos-integrity" "$TARGET_DIR/usr/bin/customos-app"
