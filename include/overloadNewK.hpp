#pragma once
#include "PVAddrTransfer.hpp"
#include "debug.hpp"
#include "handler.hpp"
#include "mapping.hpp"
#include "memoryK.hpp"
#include "printK.hpp"
#include "uart.hpp"
#include <cstddef>
#include <cstdint>
#include <stddef.h>
#include <stdint.h>

namespace Kernel
{

class NewK
{
public:
    void *operator new(size_t size);
    void operator delete(void *p);

private:
    static int _blockBitMap64[0x2000 / 0x40];
    static int _blockBitMap128[0x2000 / 0x80];
    static int _blockBitMap256[0x2000 / 0x100];

    static int allocate_mem(size_t size);
    static int free_mem(uint64_t addr);

    uint64_t _endAddr = 0x207000;
    uint64_t _memBlockSize64 = 0x40;
    uint64_t _memBlockSize128 = 0x80;
    uint64_t _memBlockSize256 = 0x100;
};

} // namespace Kernel
