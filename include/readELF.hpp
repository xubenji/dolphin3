#pragma once

#include "file.hpp"    // 包含FAT16类的定义
#include "memoryK.hpp" // 包含内存管理相关函数
#include "overloadNewK.hpp"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <elf.h>
#ifdef __cplusplus
}
#endif

namespace Kernel
{
class ELF : public NewK
{
private:
    Kernel::FAT16 *fs;
    void *memory; // 用来存储ELF文件内容的内存指针
    char *get_class_str(unsigned char ei_class);
    char *get_data_str(unsigned char ei_data);
    char *get_type_str(unsigned short e_type);
    char *get_machine_str(unsigned short e_machine);
    bool is_strings_equal(const char *str1, const char *str2);

public:
    ELF();                     // 构造函数
    ~ELF();                    // 析构函数
    bool load(char *filename); // 加载ELF文件
    void print_header();       // 打印ELF文件头部信息
    void print_section(const char *section_name); // 打印ELF文件中指定段的信息
};
} // namespace Kernel
