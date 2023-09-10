/*
 * @Author: xubenji 459547070@qq.com
 * @Date: 2023-02-04 17:07:57
 * @LastEditors: xubenji 459547070@qq.com
 * @LastEditTime: 2023-02-04 23:50:31
 * @FilePath: /dolphin3/kernel/kernelMemory.cpp
 * @Description: 初始化kernel的内存管理，默认虚拟机模拟树莓派3B，所以内存容量设定为1GB
 * 我们似乎需要对几个重点变量了解清楚，内核的代码被加载在0x80000(512kb)的物理地址上，
 * 但是我们需要让内核能够访问所有的虚拟地址空间，在运行完init_kernel_memory()函数后，
 * 内核可以访问所有物理地址和虚拟地址。kernelOccupiedMemSize变量是bootloader传递给内核的，
 * 表示内核将会占用的物理内存大小，这里我们假设内核占用4MB的物理内存。
 * 及0x400000（五个0, 4MB）. 2MB4KB~2MB28KB将会用于内核重载new操作，
 * 2MB4kb~2MB12KB block_size = 64字节，64字节转化为16进制是0x40，2mb12kb转化为
 * 2MB12KB~2MB20KB block_size = 128字节，128字节转化为16进制是0x80，
 * 2MB20KB~2MB28KB block_size = 256字节，256字节转化为16进制是0x100，
 * 也就是说，我们将会在这个区域内实现自己的C++new内存分配逻辑。
 * 内核本身代码可能只有十几KB，为什么内核要占用这么多内存？因为页目录表和页表表项都需要占用内存。
 * 其中最大的页目录表地址，也就是TTBR1_EL1寄存器的值，是0x82000（520kb）。这里使用pageDir0Addr表示，
 * pageDir1Addr地址是0x8300（524kb），pageDir2Addr地址是0x200000（2MB）。
 * pageDir2Addr是页表的表项，每一项8个字节，总共有512项，占用4KB内存。所以2MB~2MB4KB将会用于页表。
 * 低于2MB我干脆不用，以免麻烦。
 * 所以你看，低于4MB的内存地址区域全部都是内核很重要的数据结构，所以这里不会给应用程序使用。
 * 因为new操作需要返回一个内存地址，所以，我们必须占用一部分内存。但是每一个类的大小是一个未知数，我们有几点实现的方法。
 * 1. 我们可以在编译时就知道每一个类的大小，然后在物理内存中分配一块内存，
 *    然后在new操作中返回这块内存的地址。这样操作很难管理内存。
 * 2. 我们固定每一个类的大小，然后在内核中分配一块内存，然后在new操作中返回这块内存的地址。但是这会产生浪费。
 * 我们初步的想法是固定一个足够大的内存块，比如说64字节，然后在内核中分配一块内存，然后在new操作中返回这块内存的地址。
 * 如果类的大小超过了这个内存块，我们就分配一个新的内存块。虽然这样会有一些浪费，但是好处是便于管理.
 */

#include "kernelMemory.hpp"
#include "debug.hpp"
#include "newOverload.hpp"
#include "phyVirAddrTransfer.hpp"
#include "printk.hpp"
#include <stddef.h>
#include <stdint.h>

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

// 内核占用的物理内存大小4MB
uint64_t kernelOccupiedMemSize = 0x400000;

void init_kernel_memory()
{
    printk("\n");

    uint64_t pageDir0Addr = get_ttbr1_el1();
    uint64_t *p = pageDir0Addr;
    uint64_t pageDir1Addr = (*p >> 8) << 8;
    p = pageDir1Addr;
    uint64_t pageDir2Addr = (*p >> 8) << 8;
    p = pageDir2Addr + 16;
    map_kernel_page_dir_virtual_addr(p);

    // 测试代码
    // uint64_t *pp = P2V(0x400400);
    //  *pp = 0x4578;
    // int aa = 3;
}

/**
     * function:map_kernel_page_dir_virtual_addr
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