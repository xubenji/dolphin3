#ifdef __cplusplus
extern "C" {
#endif
    #include "uart.h"
    #include "lib.h"
    #include "printk.h"
    #include "handler.h"
#ifdef __cplusplus
}
#endif

#include "stdint.h"

void init_all()
{
    init_uart();
    printk("test!!!!!");

    printk("We current at level: %d\n", (uint64_t)get_el());


    enable_irq();

    
    while (1)
    {
        
    }
}
