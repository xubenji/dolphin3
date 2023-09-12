#ifndef MEMORY_K_HPP
#define MEMORY_K_HPP
#include <cstddef>
#include <stdint.h>
#define PAGE_SIZE 0x200000

namespace Kernel
{
#define MEMORY_SIZE 1024
void init_kernel_memory();
void map_kernel_page_dir_virtual_addr(uint64_t startAddr);
uint64_t mallock(void);
bool freek(uint64_t addr);
} // namespace Kernel

#endif