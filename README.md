# Custom OS

A tiny x86 operating system built for UTM SE. GitHub Actions builds a bootable ISO automatically.

## Build
Push to `main` or run the **Build OS** workflow manually. The ISO is uploaded as a workflow artifact.

## Current UI
A 320x200 VGA-mode desktop with a dark background, top bar, launcher, clock placeholder, and simple cards. No userspace or Linux dependency.

## UTM SE
Create an x86 PC VM and attach the generated ISO as the optical disk. Boot from the ISO.
