#include <efi.h>
#include <efilib.h>
#include <elf.h>

#define BOOT_INFO_MAGIC 0x4E4F4C424F4F5431ULL
#define KERNEL_PATH L"\\kernel.elf"
#define STACK_PAGES 16

static EFI_GUID FileInfoGuid = { 0x09576e92, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } };

struct boot_info {
    UINT64 magic;
    UINT64 memory_map_size;
    UINT64 memory_map_key;
    UINT64 descriptor_size;
    UINT32 descriptor_version;
    UINT32 reserved;
    VOID *memory_map;
};

static VOID fail(EFI_STATUS status, CHAR16 *message) {
    Print(L"NOL OS loader error: %s (status 0x%lx)\r\n", message, status);
    for (;;) __asm__ __volatile__("cli; hlt");
}

static EFI_STATUS read_file(EFI_HANDLE image, CHAR16 *path, VOID **buffer, UINTN *size) {
    EFI_LOADED_IMAGE *loaded = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs = NULL;
    EFI_FILE_HANDLE root = NULL, file = NULL;
    EFI_FILE_INFO *info = NULL;
    EFI_STATUS status;
    UINTN info_size = SIZE_OF_EFI_FILE_INFO;

    status = uefi_call_wrapper(BS->HandleProtocol, 3, image, &LoadedImageProtocol, (VOID **)&loaded);
    if (EFI_ERROR(status)) return status;
    status = uefi_call_wrapper(BS->HandleProtocol, 3, loaded->DeviceHandle, &FileSystemProtocol, (VOID **)&fs);
    if (EFI_ERROR(status)) return status;
    status = uefi_call_wrapper(fs->OpenVolume, 2, fs, &root);
    if (EFI_ERROR(status)) return status;
    status = uefi_call_wrapper(root->Open, 5, root, &file, path, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) { uefi_call_wrapper(root->Close, 1, root); return status; }
    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, info_size, (VOID **)&info);
    if (EFI_ERROR(status)) goto done;
    status = uefi_call_wrapper(file->GetInfo, 4, file, &FileInfoGuid, &info_size, info);
    if (status == EFI_BUFFER_TOO_SMALL) {
        uefi_call_wrapper(BS->FreePool, 1, info);
        info = NULL;
        status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, info_size, (VOID **)&info);
        if (EFI_ERROR(status)) goto done;
        status = uefi_call_wrapper(file->GetInfo, 4, file, &FileInfoGuid, &info_size, info);
    }
    if (EFI_ERROR(status)) goto done;
    if (info->FileSize == 0 || info->FileSize > 64 * 1024 * 1024) { status = EFI_LOAD_ERROR; goto done; }
    *size = (UINTN)info->FileSize;
    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, *size, buffer);
    if (EFI_ERROR(status)) goto done;
    status = uefi_call_wrapper(file->Read, 3, file, size, *buffer);
    if (EFI_ERROR(status)) uefi_call_wrapper(BS->FreePool, 1, *buffer);
done:
    if (info) uefi_call_wrapper(BS->FreePool, 1, info);
    uefi_call_wrapper(file->Close, 1, file);
    uefi_call_wrapper(root->Close, 1, root);
    return status;
}

static EFI_STATUS load_elf(VOID *data, UINTN size, EFI_PHYSICAL_ADDRESS *entry_out) {
    Elf64_Ehdr *eh;
    Elf64_Phdr *ph;
    UINTN i;
    if (size < sizeof(Elf64_Ehdr)) return EFI_LOAD_ERROR;
    eh = (Elf64_Ehdr *)data;
    if (eh->e_ident[EI_MAG0] != ELFMAG0 || eh->e_ident[EI_MAG1] != ELFMAG1 ||
        eh->e_ident[EI_MAG2] != ELFMAG2 || eh->e_ident[EI_MAG3] != ELFMAG3 ||
        eh->e_ident[EI_CLASS] != ELFCLASS64 || eh->e_ident[EI_DATA] != ELFDATA2LSB ||
        eh->e_machine != EM_X86_64 || eh->e_phentsize != sizeof(Elf64_Phdr) ||
        eh->e_phoff > size || eh->e_phnum > (size - eh->e_phoff) / sizeof(Elf64_Phdr)) return EFI_LOAD_ERROR;
    ph = (Elf64_Phdr *)((UINT8 *)data + eh->e_phoff);
    for (i = 0; i < eh->e_phnum; ++i) {
        EFI_PHYSICAL_ADDRESS address;
        UINTN pages;
        EFI_STATUS status;
        if (ph[i].p_type != PT_LOAD || ph[i].p_memsz == 0) continue;
        if (ph[i].p_filesz > ph[i].p_memsz || ph[i].p_offset > size || ph[i].p_filesz > size - ph[i].p_offset || ph[i].p_paddr > 0xFFFFFFFFULL || ph[i].p_memsz > 64 * 1024 * 1024) return EFI_LOAD_ERROR;
        address = (EFI_PHYSICAL_ADDRESS)(ph[i].p_paddr & ~0xFFFULL);
        pages = (UINTN)((ph[i].p_paddr + ph[i].p_memsz - address + 0xFFF) / 0x1000);
        status = uefi_call_wrapper(BS->AllocatePages, 4, AllocateAddress, EfiLoaderData, pages, &address);
        if (EFI_ERROR(status)) return status;
        CopyMem((VOID *)(UINTN)ph[i].p_paddr, (UINT8 *)data + ph[i].p_offset, (UINTN)ph[i].p_filesz);
        if (ph[i].p_memsz > ph[i].p_filesz) SetMem((VOID *)(UINTN)(ph[i].p_paddr + ph[i].p_filesz), (UINTN)(ph[i].p_memsz - ph[i].p_filesz), 0);
    }
    if (eh->e_entry == 0 || eh->e_entry > 0xFFFFFFFFULL) return EFI_LOAD_ERROR;
    *entry_out = (EFI_PHYSICAL_ADDRESS)eh->e_entry;
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *system_table) {
    VOID *kernel_data = NULL, *map = NULL;
    UINTN kernel_size = 0, map_size = 0, map_key = 0, descriptor_size = 0;
    UINT32 descriptor_version = 0;
    EFI_PHYSICAL_ADDRESS entry = 0, stack_base = 0;
    struct boot_info *boot = NULL;
    EFI_STATUS status;
    InitializeLib(image, system_table);
    Print(L"NOL OS: native UEFI loader\r\n");
    status = read_file(image, KERNEL_PATH, &kernel_data, &kernel_size);
    if (EFI_ERROR(status)) fail(status, L"cannot read \\kernel.elf; ensure the file is beside EFI/BOOT/BOOTX64.EFI");
    status = load_elf(kernel_data, kernel_size, &entry);
    uefi_call_wrapper(BS->FreePool, 1, kernel_data);
    if (EFI_ERROR(status)) fail(status, L"invalid ELF kernel or unable to allocate its load segments");
    status = uefi_call_wrapper(BS->AllocatePages, 4, AllocateAnyPages, EfiLoaderData, STACK_PAGES, &stack_base);
    if (EFI_ERROR(status)) fail(status, L"cannot allocate kernel stack");
    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, sizeof(*boot), (VOID **)&boot);
    if (EFI_ERROR(status)) fail(status, L"cannot allocate boot info");
    boot->magic = BOOT_INFO_MAGIC;
    status = uefi_call_wrapper(BS->GetMemoryMap, 5, &map_size, map, &map_key, &descriptor_size, &descriptor_version);
    if (status != EFI_BUFFER_TOO_SMALL) fail(status, L"cannot size UEFI memory map");
    map_size += 2 * descriptor_size + 4096;
    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, map_size, &map);
    if (EFI_ERROR(status)) fail(status, L"cannot allocate UEFI memory map buffer");
    for (;;) {
        UINTN current_size = map_size;
        status = uefi_call_wrapper(BS->GetMemoryMap, 5, &current_size, map, &map_key, &descriptor_size, &descriptor_version);
        if (EFI_ERROR(status)) fail(status, L"cannot retrieve UEFI memory map");
        boot->memory_map_size = current_size;
        boot->memory_map_key = map_key;
        boot->descriptor_size = descriptor_size;
        boot->descriptor_version = descriptor_version;
        boot->memory_map = map;
        status = uefi_call_wrapper(BS->ExitBootServices, 2, image, map_key);
        if (!EFI_ERROR(status)) break;
        if (status != EFI_INVALID_PARAMETER) fail(status, L"ExitBootServices failed");
    }
    __asm__ __volatile__("mov %0, %%rsp\n\tand $-16, %%rsp\n\txor %%rbp, %%rbp\n\tmov %1, %%rdi\n\tjmp *%2"
                         : : "r"((UINTN)stack_base + STACK_PAGES * 4096 - 16), "r"(boot), "r"((UINTN)entry) : "memory");
    __builtin_unreachable();
}
