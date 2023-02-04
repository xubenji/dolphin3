#pragma once

#include "stdint.h"

void delay(uint64_t value);
void out_word(uint64_t addr, uint32_t value);
uint32_t in_word(uint64_t addr);

unsigned char get_el(void);

