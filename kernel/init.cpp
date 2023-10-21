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
#include "stdarg.h"
#include <stdint.h>

extern "C" char bss_start, bss_end;

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

    // printk("\nbss_start: %x, bss_end: %x\n", (uint64_t)&bss_start, (uint64_t)&bss_end);

    Kernel::init_kernel_memory();
    Kernel::init_fs();

    // 假设我们读取一个文件，假设我们分配一个内存地址(3MB)给p
    //void *p = 0x300000;

    void *p = Kernel::mallock();


    if (Kernel::load_file("TEST.BIN", (uint64_t)p) == 0)
    {
        printk("File data: %s\r\n", p);
    }
    printk("All files in root dir: %s\r\n", p);
    Kernel::list_file();

    //  enable_irq();
    //  init_timer();
    printk("\n");
    while (1)
    {
    }
}
