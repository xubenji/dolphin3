/*
 * @Author: xubenji 459547070@qq.com
 * @Date: 2023-02-04 17:07:57
 * @LastEditors: xubenji 459547070@qq.com
 * @LastEditTime: 2023-02-04 23:50:31
 * @FilePath: /dolphin3/kernel/kernelMemory.cpp
 * @Description: 初始化kernel的内存管理，默认虚拟机模拟树莓派3B，所以内存容量设定为1GB
 */

#include "kernelMemory.hpp"
#include "debug.hpp"
#include "printk.hpp"
#include "stddef.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "lib.hpp"
#ifdef __cplusplus
}
#endif

namespace kernelMemory
{
uint64_t kernelStartMemory = 0x400000;

void init_kernel_memory()
{
    uint64_t pageDir0Addr = get_ttbr1_el1();
    int aa = 3;
}

void check_memory_info()
{
    uint64_t pages = MEMORY_SIZE / 2 - 2;
    ASSERT(pages > 0, "init_page(): memory is too small (memory must bigger than 4 MB)");
    uint64_t address = kernelStartMemory;
}

} 
