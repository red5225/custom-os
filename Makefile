BUILD=build
CORE_URL=http://repo.tinycorelinux.net/17.x/x86/release/Core-17.1.iso
CORE_SHA256=8fe45bbda0e9b52e5874dd6e9733aac5051e6311282c1e056c852fb1fd721b08
TCZ_BASE=http://repo.tinycorelinux.net/17.x/x86/tcz
PYTHON_TCZ=python3.14.tcz

all: $(BUILD)/os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/Core-17.1.iso: | $(BUILD)
	wget -c --tries=20 --timeout=20 --waitretry=2 $(CORE_URL) -O $(BUILD)/Core-17.1.iso
	printf '%s  %s\n' '$(CORE_SHA256)' '$(BUILD)/Core-17.1.iso' | sha256sum -c -

$(BUILD)/os.iso: $(BUILD)/Core-17.1.iso
	rm -rf $(BUILD)/iso-root $(BUILD)/tcz
	mkdir -p $(BUILD)/iso-root $(BUILD)/tcz
	xorriso -osirrox on -indev $(BUILD)/Core-17.1.iso -extract / $(BUILD)/iso-root
	cd $(BUILD)/tcz && wget -c --tries=20 --timeout=20 --waitretry=2 $(TCZ_BASE)/$(PYTHON_TCZ) $(TCZ_BASE)/$(PYTHON_TCZ).dep
	@set -e; \
	cd $(BUILD)/tcz; \
	download_deps() { \
	  f="$$1"; \
	  [ -f "$$f.dep" ] || return 0; \
	  while read -r dep; do \
	    [ -z "$$dep" ] && continue; \
	    if [ ! -f "$$dep" ]; then wget -q --tries=20 --timeout=20 --waitretry=2 "$(TCZ_BASE)/$$dep"; fi; \
	    if [ ! -f "$$dep.dep" ]; then wget -q --tries=20 --timeout=20 --waitretry=2 "$(TCZ_BASE)/$$dep.dep" || true; fi; \
	    download_deps "$$dep"; \
	  done < "$$f.dep"; \
	}; \
	download_deps "$(PYTHON_TCZ)"
	mkdir -p $(BUILD)/iso-root/tce/optional
	cp $(BUILD)/tcz/*.tcz $(BUILD)/iso-root/tce/optional/
	printf '%s\n' $$(cd $(BUILD)/tcz && ls -1 *.tcz) > $(BUILD)/iso-root/tce/onboot.lst
	@set -e; \
	for cfg in $(BUILD)/iso-root/boot/isolinux/isolinux.cfg $(BUILD)/iso-root/boot/isolinux/*.cfg; do \
	  [ -f "$$cfg" ] || continue; \
	  sed -i 's/append /append tinycore tce=sr0 /' "$$cfg"; \
	done
	rm -f $(BUILD)/os.iso
	xorriso -as mkisofs -l -J -R -V CUSTOMOS -no-emul-boot -boot-load-size 4 -boot-info-table -b boot/isolinux/isolinux.bin -c boot/isolinux/boot.cat -o $(BUILD)/os.iso $(BUILD)/iso-root

clean:
	rm -rf $(BUILD)
