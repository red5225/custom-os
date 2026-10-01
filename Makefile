BUILD=build
CORE_URL=http://repo.tinycorelinux.net/17.x/x86/release/Core-17.1.iso
CORE_SHA256=8fe45bbda0e9b52e5874dd6e9733aac5051e6311282c1e056c852bf1fd721b08
TCZ_BASE=http://repo.tinycorelinux.net/17.x/x86/tcz
PYTHON_TCZ=python3.14.tcz
DESKTOP_PACKAGES=Xvesa.tcz jwm.tcz aterm.tcz

all: $(BUILD)/os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/Core-17.1.iso: | $(BUILD)
	wget -c --tries=20 --timeout=20 --waitretry=2 $(CORE_URL) -O $@
	printf '%s  %s\n' '$(CORE_SHA256)' '$@' | sha256sum -c -

$(BUILD)/os.iso: $(BUILD)/Core-17.1.iso scripts/fetch_tcz.py scripts/make_wallpaper.py
	rm -rf $(BUILD)/iso-root $(BUILD)/tcz $(BUILD)/initrd-root
	mkdir -p $(BUILD)/iso-root $(BUILD)/tcz $(BUILD)/initrd-root
	xorriso -osirrox on -indev $(BUILD)/Core-17.1.iso -extract / $(BUILD)/iso-root
	chmod -R u+rwX $(BUILD)/iso-root
	python3 scripts/fetch_tcz.py $(TCZ_BASE) $(PYTHON_TCZ) $(BUILD)/tcz
	for pkg in $(DESKTOP_PACKAGES); do python3 scripts/fetch_tcz.py $(TCZ_BASE) $$pkg $(BUILD)/tcz; done
	mkdir -p $(BUILD)/iso-root/tce/optional
	cp $(BUILD)/tcz/*.tcz $(BUILD)/iso-root/tce/optional/
	printf '%s\n' Xvesa.tcz jwm.tcz aterm.tcz > $(BUILD)/iso-root/tce/onboot.lst
	mkdir -p $(BUILD)/initrd-root
	gunzip -c $(BUILD)/iso-root/boot/core.gz | (cd $(BUILD)/initrd-root && sudo cpio -idm --quiet)
	sudo chown -R $$(id -u):$$(id -g) $(BUILD)/initrd-root
	chmod -R u+rwX $(BUILD)/initrd-root
	cp -a board/custom/rootfs_overlay/. $(BUILD)/initrd-root/
	mkdir -p $(BUILD)/initrd-root/usr/local/bin $(BUILD)/initrd-root/usr/local/share/custom-os $(BUILD)/initrd-root/opt/backgrounds $(BUILD)/initrd-root/home/tc/.X.d
	cp desktop/start-desktop.sh $(BUILD)/initrd-root/usr/local/bin/start-desktop
	cp desktop/desktop-launcher.sh $(BUILD)/initrd-root/usr/local/bin/custom-launcher
	cp desktop/jwmrc $(BUILD)/initrd-root/home/tc/.jwmrc
	cp desktop/boot-banner.txt $(BUILD)/initrd-root/usr/local/share/custom-os/boot-banner.txt
	cp desktop/login-banner.sh $(BUILD)/initrd-root/usr/local/bin/custom-login-banner
	cp desktop/xinit-custom $(BUILD)/initrd-root/home/tc/.X.d/custom-os
	chmod +x $(BUILD)/initrd-root/usr/local/bin/start-desktop $(BUILD)/initrd-root/usr/local/bin/custom-launcher $(BUILD)/initrd-root/usr/local/bin/custom-login-banner $(BUILD)/initrd-root/home/tc/.X.d/custom-os
	python3 scripts/make_wallpaper.py $(BUILD)/initrd-root/opt/backgrounds/custom-os.png
	cd $(BUILD)/initrd-root && find . -print | cpio -o -H newc --quiet | gzip -9 > ../iso-root/boot/core.gz
	for cfg in $(BUILD)/iso-root/boot/isolinux/isolinux.cfg $(BUILD)/iso-root/boot/isolinux/*.cfg; do [ -f "$$cfg" ] || continue; sed -i 's#tce=sr0#tce=sr0 desktop=custom-os#g' "$$cfg"; done
	rm -f $@
	xorriso -as mkisofs -l -J -R -V CUSTOMOS -no-emul-boot -boot-load-size 4 -boot-info-table -b boot/isolinux/isolinux.bin -c boot/isolinux/boot.cat -o $@ $(BUILD)/iso-root

clean:
	rm -rf $(BUILD)
