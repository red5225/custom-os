BUILD=build
CORE_URL=http://repo.tinycorelinux.net/17.x/x86/release/Core-17.1.iso
CORE_SHA256=8fe45bbda0e9b52e5874dd6e9733aac5051e6311282c1e056c852fb1fd721b08

all: $(BUILD)/os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/Core-17.1.iso: | $(BUILD)
	wget -c --tries=20 --timeout=20 --waitretry=2 $(CORE_URL) -O $(BUILD)/Core-17.1.iso
	printf '%s  %s
' '$(CORE_SHA256)' '$(BUILD)/Core-17.1.iso' | sha256sum -c -

$(BUILD)/os.iso: $(BUILD)/Core-17.1.iso
	cp $(BUILD)/Core-17.1.iso $(BUILD)/os.iso

clean:
	rm -rf $(BUILD)
