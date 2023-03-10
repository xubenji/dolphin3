#pragma once
#include "stdint.h"
#define PAGE_SIZE 0x200000

namespace kernelMemory
{
#define MEMORY_SIZE 1024
void init_kernel_memory();
void map_kernel_page_dir_virtual_addr(uint64_t startAddr);
} // namespace kernelMemory
