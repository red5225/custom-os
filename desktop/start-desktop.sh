#!/bin/sh
export DISPLAY=:0
export XDG_CURRENT_DESKTOP=CustomOS
export HOME=/home/tc
mkdir -p "$HOME/.jwm"
if ! xdpyinfo >/dev/null 2>&1; then
  if command -v Xfbdev >/dev/null 2>&1; then
    Xfbdev :0 -screen 0 1024x768x32 -nolisten tcp >/tmp/custom-os-x.log 2>&1 &
  elif command -v Xorg >/dev/null 2>&1; then
    Xorg :0 -nolisten tcp >/tmp/custom-os-x.log 2>&1 &
  else
    exit 1
  fi
  sleep 2
fi
jwm >/tmp/custom-os-jwm.log 2>&1 &
