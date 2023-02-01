#include "stdint.h"
#ifdef __cplusplus
extern "C"{
#endif
#include "lib.h"
#include "printk.h"
#include "irq.h"
#include "uart.h"
#ifdef __cplusplus
}
#endif





//struct task_list tasks[tasksNumber];
struct task_list *p;
struct task_list *tHead;



extern "C"{
void enable_timer(void);
uint32_t read_timer_status(void);
uint32_t read_timer_freq(void);
void set_timer_interval(uint32_t value);
void set_elr_el(uint64_t elr);
void disable_timer(void);
void irq_return(uint64_t sp);
void set_spsr_el(uint64_t spsr_el);
void set_sp_el1(uint64_t sp);
void set_sp_el0(uint64_t sp);
void set_ttbr0(uint64_t dir0);
void handler(uint64_t numid, uint64_t esr, uint64_t elr, uint64_t sp, uint64_t spsr, uint64_t ttbr0_el1, uint64_t ttbr1_el1);
}

static uint32_t timer_interval = 0;
static uint64_t ticks = 0;

void init_timer(void)
{
    timer_interval = read_timer_freq() / 100;
    enable_timer();
    out_word(CNTP_EL0, (1 << 1));
}

static void timer_interrupt_handler(uint64_t esr, uint64_t elr, uint64_t sp, uint64_t spsr, uint64_t ttbr0_el1, uint64_t ttbr1_el1)
{
    uint32_t status = read_timer_status();
    //disable_timer();
    if (status & (1 << 2))
    {
        printk("t");
    }
}

void handler(uint64_t numid, uint64_t esr, uint64_t elr, uint64_t sp, uint64_t spsr, uint64_t ttbr0_el1, uint64_t ttbr1_el1)
{
    uint32_t irq;

    switch (numid)
    {
    case 1:
        printk("sync error at %x: %x\r\n", elr, esr);
        while (1)
        {
        }

    case 2:
        irq = in_word(CNTP_STATUS_EL0);
        if (irq & (1 << 1))
        {
            timer_interrupt_handler(esr, elr, sp, spsr, ttbr0_el1, ttbr1_el1);
        }
        else
        {
            printk("unknown irq \r\n");
            while (1)
            {
            }
        }
        break;

    default:
        printk("unknown exception\r\n");
        while (1)
        {
        }
    }
}