/*
 * @Author: xubenji 459547070@qq.com
 * @Date: 2023-09-10 17:07:57
 * @LastEditors: xubenji 459547070@qq.com
 * @LastEditTime: 2023-09-10 23:50:31
 * @FilePath: /dolphin3/kernel/overloadKNew.cpp
 * @Description: 所有的在kernel空间中new的内存都会从这里分配，所以所有class必须继承Kernel::KNew
*/

#include "overloadKNew.hpp"
#include <cstddef>
#include <cstdint>

namespace Kernel
{
//  * 2MB4kb~2MB12KB block_size = 64字节，64字节转化为16进制是0x40，2mb12kb转化为
//  * 2MB12KB~2MB20KB block_size = 128字节，128字节转化为16进制是0x80，
//  * 2MB20KB~2MB28KB block_size = 256字节，256字节转化为16进制是0x100，
int KNew::_blockBitMap64[0x2000 / 0x40] = {0};
int KNew::_blockBitMap128[0x2000 / 0x80] = {0};
int KNew::_blockBitMap256[0x2000 / 0x100] = {0};

void *KNew::operator new(size_t size)
{
    uint64_t addr = allocate_mem(size);
    if (addr < static_cast<uint64_t>(0))
    {
        printk("allocate_mem error\n");
        return nullptr;
    }
    else
    {
        // printk("allocate_mem success\n");
        return reinterpret_cast<void *>(addr);
    }
    // printk("\nnew the size is: %x\n", size);
    return nullptr; // This line may not be necessary since we have conditions covering all outcomes above.
}

void KNew::operator delete(void *p)
{
    if (free_mem(reinterpret_cast<uint64_t>(p)))
    {
        printk("free_mem succeeded\n");
    }
}

// 起始地址是0x201000，结束地址是0x207000
int KNew::allocate_mem(size_t size)
{
    if (size <= 0x40)
    {
        for (size_t i = 0; i < 0x2000 / 0x40; i++)
        {
            if (_blockBitMap64[i] == 0)
            {
                _blockBitMap64[i] = 1;
                return 0x201000 + i * 0x40;
            }
        }
    }
    else if (size <= 0x80)
    {
        for (size_t i = 0; i < 0x1000 / 0x80; i++)
        {
            if (_blockBitMap128[i] == 0)
            {
                _blockBitMap128[i] = 1;
                return 0x203000 + i * 0x80;
            }
        }
    }
    else if (size <= 0x100)
    {
        for (size_t i = 0; i < 0x1000 / 0x100; i++)
        {
            if (_blockBitMap256[i] == 0)
            {
                _blockBitMap256[i] = 1;
                return 0x205000 + i * 0x100;
            }
        }
    }
    return -1;
}

int KNew::free_mem(uint64_t addr)
{
    // 起始地址是0x201000，结束地址是0x207000
    if (addr >= 0x201000 && addr < 0x203000)
    {
        int index = (addr - 0x201000) / 0x40;
        _blockBitMap64[index] = 0;
        return true;
    }
    else if (addr >= 0x203000 && addr < 0x205000)
    {
        int index = (addr - 0x203000) / 0x80;
        _blockBitMap128[index] = 0;
        return true;
    }
    else if (addr >= 0x205000 && addr < 0x207000)
    {
        int index = (addr - 0x205000) / 0x100;
        _blockBitMap256[index] = 0;
        return true;
    }
    else
    {
        return -1;
    }
}

} // namespace Kernel
