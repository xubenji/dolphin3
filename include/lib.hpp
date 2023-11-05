#pragma once

#include <stdint.h>

void delay(uint64_t value);
void out_word(uint64_t addr, uint32_t value);
uint32_t in_word(uint64_t addr);
void access_el0(void);
void set_ttbr0_el1(uint64_t ttbr0_el1);

unsigned char get_el(void);
//ttbr1_el1是内核态下管理页目录表的寄存器
//ttbr0_el1是用户态下管理页目录表的寄存器
//这和x86_64的不一样，在x86平台下都是cr3寄存器管理页目录表基地址
uint64_t get_ttbr1_el1(void);
uint64_t get_ttbr0_el1(void);
