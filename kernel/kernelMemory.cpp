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
#include "addrTransfer.hpp"
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
    uint64_t *p = pageDir0Addr;
    uint64_t pageDir1Addr = (*p >> 8) << 8;
    p = pageDir1Addr;
    uint64_t pageDir2Addr = (*p >> 8) << 8;
    p = pageDir2Addr + 16;
    map_kernel_page_dir_virtual_addr(p);

    //测试代码
    //uint64_t *pp = P2V(0x400400);
    // *pp = 0x4578;
    //int aa = 3;
}

/** 
 * function: 
 * param [uint64_t] startAddr: 开始映射的起始地址
 * return [void]
 * description: 让内核完全访问虚拟地址，如果不执行这个函数，内核无法访问所有的虚拟地址。
 * 超过4mb的虚拟地址无法访问，例如： P2V(0x400400) 无法访问。
 * 在mmu.S中已经映射了4mb的虚拟地址，所以这里只需要映射接下来的1GB-4MB虚拟地址，所以页表需要512-2项
 */
void map_kernel_page_dir_virtual_addr(uint64_t startAddr)
{
    uint64_t *p = startAddr;
    uint64_t pageAddr = 0x400000;
    pageAddr = (pageAddr | 1 << 10 | 1 << 2 | 1 << 0);
    for (int i = 0; i < 512 - 2; i++)
    {
        *(p + i) = pageAddr;
        pageAddr = pageAddr + PAGE_SIZE;
    }
}

} // namespace kernelMemory
