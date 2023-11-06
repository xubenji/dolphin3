#include "readELF.hpp"
#include "file.hpp"
#include "memoryK.hpp"
#include "printK.hpp"
#include "printK.hpp" // 假设printk函数定义在此头文件中
#include "uart.hpp"
#include <stdint.h>

namespace Kernel
{

// 构造函数
ELF::ELF()
{
    fs = new FAT16();
    memory = nullptr;
}

// 析构函数
ELF::~ELF()
{
    if (memory)
    {
        // 假设有一个用于释放内存的函数
        freek(reinterpret_cast<uint64_t>(memory));
        memory = nullptr;
    }
    delete fs;
}

// 加载ELF文件到内存
bool ELF::load(char *filename)
{
    // 假设 FAT16::load_file 成功时返回0
    this->memory = (void *)Kernel::mallock(); // 分配内存
    if (!memory)
    {
        printk("Failed to allocate memory for ELF file.\n");
        return false;
    }

    if (fs->load_file(filename, reinterpret_cast<uint64_t>(memory)) != 0)
    {
        printk("Failed to load file: %s\n", filename);
        return false;
    }
    return true;
}

char *ELF::get_class_str(unsigned char eiClass)
{
    switch (eiClass)
    {
    case ELFCLASS32:
        return "ELF32";
    case ELFCLASS64:
        return "ELF64";
    default:
        return "Unknown";
    }
}

char *ELF::get_data_str(unsigned char eiData)
{
    switch (eiData)
    {
    case ELFDATA2LSB:
        return "2's complement, little endian";
    case ELFDATA2MSB:
        return "2's complement, big endian";
    default:
        return "Unknown";
    }
}

char *ELF::get_type_str(unsigned short eType)
{
    switch (eType)
    {
    case ET_NONE:
        return "NONE (No file type)";
    case ET_REL:
        return "REL (Relocatable file)";
    case ET_EXEC:
        return "EXEC (Executable file)";
    case ET_DYN:
        return "DYN (Shared object file)";
    case ET_CORE:
        return "CORE (Core file)";
    // There are other types like ET_LOOS/ET_HIOS, ET_LOPROC/ET_HIPROC, but they are less common
    default:
        return "Unknown";
    }
}

char *ELF::get_machine_str(unsigned short eMachine)
{
    switch (eMachine)
    {
    case EM_NONE:
        return "None";
    case EM_386:
        return "Intel 80386";
    case EM_X86_64:
        return "AMD x86-64";
    case EM_AARCH64:
        return "AArch64";
    // There are many possible values for e_machine, these are just a few examples
    case EM_ARM:
        return "ARM";
    case EM_MIPS:
        return "MIPS";
    case EM_PPC:
        return "PowerPC";
    case EM_PPC64:
        return "PowerPC 64-bit";
    case EM_S390:
        return "IBM S390";
    // ... Add more machine types as needed ...
    default:
        return "Unknown";
    }
}

bool ELF::is_strings_equal(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        ++str1;
        ++str2;
    }
    return *str1 == *str2;
}

// 打印ELF头部信息
void ELF::print_header()
{
    if (memory == nullptr)
    {
        printk("Memory not initialized.\n");
        return;
    }

    Elf64_Ehdr *hdr = reinterpret_cast<Elf64_Ehdr *>(memory);

    printk("ELF Header:\n");
    printk("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++)
    {
        printk("%x ", hdr->e_ident[i]);
    }
    printk("\n");
    printk("  Class:                             %s\n", get_class_str(hdr->e_ident[EI_CLASS]));
    printk("  Data:                              %s\n", get_data_str(hdr->e_ident[EI_DATA]));
    printk("  Version:                           %u\n", hdr->e_ident[EI_VERSION]);
    printk("  OS/ABI:                            %u\n", hdr->e_ident[EI_OSABI]);
    printk("  ABI Version:                       %u\n", hdr->e_ident[EI_ABIVERSION]);
    printk("  Type:                              %s\n", get_type_str(hdr->e_type));
    printk("  Machine:                           %s\n", get_machine_str(hdr->e_machine));
    printk("  Version:                           0x%x\n", hdr->e_version);
    printk("  Entry point address:               0x%x\n", (unsigned long)hdr->e_entry);
    printk("  Start of program headers:          %u (bytes into file)\n", (unsigned long)hdr->e_phoff);
    printk("  Start of section headers:          %u (bytes into file)\n", (unsigned long)hdr->e_shoff);
    printk("  Flags:                             0x%x\n", hdr->e_flags);
    printk("  Size of this header:               %u (bytes)\n", hdr->e_ehsize);
    printk("  Size of program headers:           %u (bytes)\n", hdr->e_phentsize);
    printk("  Number of program headers:         %u\n", hdr->e_phnum);
    printk("  Size of section headers:           %u (bytes)\n", hdr->e_shentsize);
    printk("  Number of section headers:         %u\n", hdr->e_shnum);
    printk("  Section header string table index: %u\n", hdr->e_shstrndx);
}

void ELF::print_section(const char *sectionName)
{
    if (memory == nullptr)
    {
        printk("Memory not initialized.\n");
        return;
    }

    Elf64_Ehdr *hdr = reinterpret_cast<Elf64_Ehdr *>(memory);
    Elf64_Shdr *shdrs = reinterpret_cast<Elf64_Shdr *>(reinterpret_cast<uint64_t>(memory) + hdr->e_shoff);
    const char *stringTable = reinterpret_cast<const char *>(memory) + shdrs[hdr->e_shstrndx].sh_offset;

    for (int i = 0; i < hdr->e_shnum; i++)
    {
        const char *currentSectionName = stringTable + shdrs[i].sh_name;
        if (is_strings_equal(currentSectionName, sectionName) == false)
        {
            printk("Section '%s' content:\n", sectionName);
            const unsigned char *section_content = reinterpret_cast<const unsigned char *>(memory) + shdrs[i].sh_offset;
            for (uint64_t j = 0; j < shdrs[i].sh_size; j++)
            {
                printk("%x ", section_content[j]);
                if ((j + 1) % 16 == 0)
                    printk("\n");
            }
            printk("\n");
            return;
        }
    }
    printk("Section '%s' not found.\n", sectionName);
}

} // namespace Kernel
