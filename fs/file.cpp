#include "file.hpp"
#include "PVAddrTransfer.hpp"
#include "debug.hpp"
#include "overloadNewK.hpp"
#include "printK.hpp"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib.hpp"
#ifdef __cplusplus
}
#endif

static struct BPB *get_fs_bpb(void)
{
    uint32_t lba = *(uint32_t *)(FS_BASE + 0x1be + 8);

    return (struct BPB *)(FS_BASE + lba * 512);
}

void init_fs(void)
{
    uint8_t *p = (uint8_t *)get_fs_bpb();

    if (p[0x1fe] != 0x55 || p[0x1ff] != 0xaa)
    {
        printk("invalid signature\n");
        ASSERT(0, -1);
    }
    else
    {
        printk("file system is loaded\r\n");
    }
}
