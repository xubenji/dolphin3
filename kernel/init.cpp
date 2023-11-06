/**
 * Name:
 * Author: Benji Xu <benjixu2020@gmail.com>
 * Date: 2021-11-04 03:56:41
 * LastEditTime: 2022-03-30 23:56:16
 * LastEditors: Benji Xu
 * FilePath: /dolphin2/kernel/kernel.c
 * Description:
 * 描述: 无论是x86还是arm cpu执行完引导区的代码以后就开始执行这里的代码。
 */
#include "debug.hpp"
#include "file.hpp"
#include "handler.hpp"
#include "memoryK.hpp"
#include "readELF.hpp"
#include <stdarg.h>
#include <stdint.h>


// 可以使用这个获取二进制文件的各种地址
// extern "C" char bss_start, bss_end;

#ifdef __cplusplus
extern "C"
{
#endif
#include "lib.hpp"
#include "printK.hpp"
#include "uart.hpp"
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    void init_kernel(void);
    void enable_irq(void);

#ifdef __cplusplus
}
#endif

void init_kernel(void)
{

    init_uart();
    printk("\nOS info:\n");
    printk("We current at level: %d\n", (uint64_t)get_el());
    printk("It is a new kernel running in ARM64 with C++ code!!!");

    Kernel::init_kernel_memory();

    /******************************/
    // 未来实现用户进程要用到
    // uint64_t ttbr0_el1 = 0x190000;
    // uint64_t *pp = (uint64_t *)ttbr0_el1;
    // *pp = 0x191000 + 0x447;
    // pp = 0x191000;
    // *pp = 0x192000 + 0x447;
    // pp = 0x192000 + 0x8 + 0x8;
    // *pp = 0x445 + 0x400000;
    // uint32_t *ins = 0x400000;
    // *ins = 0xD503201F;
    // *(ins+1) = 0xCA000000;
    // *(ins+2) = 0xD503201F;
    // set_ttbr0_el1(ttbr0_el1);
    // access_el0();
    /******************************/

    void *p = (void *)Kernel::mallock();

    Kernel::FAT16 *fs = new Kernel::FAT16();
    fs->list_file();
    // if (fs->load_file("HELLO.ELF", (uint64_t)p) == 0)
    // {
    //     printk("File data: %s\r\n", p);
    // }

    Kernel::ELF *elf = new Kernel::ELF();
    if (elf->load("HELLO.ELF"))
    {
        elf->print_header();
        // 打印对应的section还存在问题
        elf->print_section(".text");
    }
    else
    {
        printk("Failed to load ELF file.\n");
    }

    //  enable_irq();
    //  init_timer();
    printk("\n");
    while (1)
    {
    }
}
