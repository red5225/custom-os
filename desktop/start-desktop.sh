#!/bin/sh
export DISPLAY=:0
export XDG_CURRENT_DESKTOP=CustomOS
export HOME=/home/tc
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin

mkdir -p /tmp/.X11-unix "$HOME"

# Tiny Core 17.x x86: Xvesa is the most compatible framebuffer/X server.
if ! command -v Xvesa >/dev/null 2>&1; then
  command -v tce-load >/dev/null 2>&1 && tce-load -i /etc/sysconfig/tcedir/optional/Xvesa.tcz >/dev/null 2>&1 || true
fi
if ! command -v jwm >/dev/null 2>&1; then
  command -v tce-load >/dev/null 2>&1 && tce-load -i /etc/sysconfig/tcedir/optional/jwm.tcz >/dev/null 2>&1 || true
fi

if command -v Xvesa >/dev/null 2>&1; then
  Xvesa :0 -screen 1024x768x32 -shadow -nolisten tcp >/tmp/custom-os-x.log 2>&1 &
else
  echo "Custom OS: Xvesa is unavailable."
  exec /bin/sh
fi

for i in 1 2 3 4 5 6 7 8 9 10; do
  [ -S /tmp/.X11-unix/X0 ] && break
  sleep 1
done

if [ ! -S /tmp/.X11-unix/X0 ]; then
  echo "Custom OS: Xvesa failed to start."
  cat /tmp/custom-os-x.log
  exec /bin/sh
fi

export DISPLAY=:0
if command -v jwm >/dev/null 2>&1; then
  exec jwm -f "$HOME/.jwmrc"
fi

echo "Custom OS: JWM is unavailable."
exec /bin/sh
