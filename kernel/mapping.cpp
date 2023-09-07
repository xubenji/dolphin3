// 这个文件主要是写一个类，然后这个类将物理地址和虚拟地址映射起来，类

#include "mapping.hpp"
#include "debug.hpp"
#include "handler.hpp"
#include "phyVirAddrTransfer.hpp"
#include "printk.hpp"
#include "uart.hpp"
#include <stddef.h>
#include <stdint.h>

class Mapping
{
public:
    Mapping(uint64_t startAddr)
    {
        this->startAddr = startAddr;
    }
    void *operator new(size_t size)
    {
        // std::cout << "operator new" << std::endl;
        return nullptr;
    }
    uint64_t startAddr;
    uint64_t endAddr;
    uint64_t virtualAddr;
};
