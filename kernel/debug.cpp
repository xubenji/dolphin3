#include "debug.hpp"
#include <stdint.h>
extern "C"
{
#include "printK.hpp"
}

void error_check(uint8_t *file, uint64_t line, uint8_t *infor)
{
    printk("\n-------------------------------\n");
    printk("         EEROR_CHECK\n");
    printk("-------------------------------\n");
    printk("Assertion Failed:[%s:%u]\n", file, line);
    printk("error information: %s", infor);
    while (1)
    {
    }

    //__asm__ __volatile__("1: jmp 1b");
}
