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
#include "handler.hpp"
#include "stdarg.h"
#include "stdint.h"
#include "kernelMemory.hpp"
#ifdef __cplusplus
extern "C"
{
#endif
#include "lib.hpp"
#include "printk.hpp"
#include "uart.hpp"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    void kernel_init(void);
    void enable_irq(void);

#ifdef __cplusplus
}
#endif

void kernel_init(void)
{
    
    init_uart();

    printk("test!!!!!");
    printk("We current at level: %d\n", (uint64_t)get_el());
    printk("It is a new kernel running in ARM64 with C++ code!!!");

    kernelMemory::init_kernel_memory();
    
    enable_irq();
    init_timer();
    printk("\n");
    while (1)
    {
    }
}
