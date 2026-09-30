BUILD=build
CORE_URL=https://mirrors.sau.edu.cn/tinycorelinux/17.x/x86/release/Core-17.1.iso
CORE_SHA256=8fe45bbda0e9b52e5874dd6e9733aac5051e6311282c1e056c852fb1fd721b08

all: $(BUILD)/os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/Core-17.1.iso: | $(BUILD)
	curl -L --fail --retry 3 --connect-timeout 15 $(CORE_URL) -o $(BUILD)/Core-17.1.iso
	printf '%s  %s\n' '$(CORE_SHA256)' '$(BUILD)/Core-17.1.iso' | sha256sum -c -

$(BUILD)/os.iso: $(BUILD)/Core-17.1.iso
	cp $(BUILD)/Core-17.1.iso $(BUILD)/os.iso

clean:
	rm -rf $(BUILD)
