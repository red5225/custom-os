#!/usr/bin/env python3
import os
import sys
import urllib.request
from collections import deque

BASE = sys.argv[1].rstrip("/")
ROOT = sys.argv[2]
OUT = sys.argv[3]
os.makedirs(OUT, exist_ok=True)

seen = set()
queue = deque([ROOT])

def download(name):
    path = os.path.join(OUT, name)
    if os.path.exists(path):
        return
    url = f"{BASE}/{name}"
    print(f"Downloading {name}", flush=True)
    urllib.request.urlretrieve(url, path)

while queue:
    name = queue.popleft()
    if not name or name.startswith("#") or name in seen:
        continue
    if not name.endswith(".tcz"):
        name += ".tcz"
    seen.add(name)
    download(name)
    dep = name + ".dep"
    try:
        download(dep)
    except Exception:
        continue
    with open(os.path.join(OUT, dep), "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            dep_name = line.strip()
            if dep_name and not dep_name.startswith("#") and dep_name not in seen:
                queue.append(dep_name)

print(f"Downloaded {len(seen)} TCZ packages.", flush=True)
