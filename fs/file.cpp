/** 
 * Name: 
 * Author: Benji Xu <benjixu2020@gmail.com>
 * Date: 2023-09-14 17:21:28
 * LastEditTime: 2023-11-05 20:52:22
 * LastEditors: Benji Xu
 * FilePath: /dolphin3/fs/file.cpp
 * Description: 
 * 描述: 目前仅支持fat16文件系统，不支持目录，文件名长度最大为8，后缀最大为3
 *      仅支持读取文件，不支持写入文件
 *      仅支持读取根目录下的文件
 *      list_file()函数用于列出根目录下的文件
 *      load_file()函数用于读取文件内容
 *      init_fs()函数用于初始化文件系统
 */
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

namespace Kernel
{

FAT16::FAT16()
{
    init_fs();
}

FAT16::~FAT16()
{
    // Optional destructor body, if cleanup is needed
}

struct FAT16::BPB *FAT16::get_fs_bpb(void)
{
    uint32_t lba = *(uint32_t *)(FS_BASE + 0x1be + 8);
    return (struct BPB *)(FS_BASE + lba * 512);
}

uint16_t *FAT16::get_fat_table(void)
{
    struct BPB *p = get_fs_bpb();
    uint32_t offset = (uint32_t)p->reservedSectorCount * p->bytesPerSector;
    return (uint16_t *)((uint8_t *)p + offset);
}

uint16_t FAT16::get_cluster_value(uint32_t clusterIndex)
{
    uint16_t *fatTable = get_fat_table();
    return fatTable[clusterIndex];
}

uint32_t FAT16::get_cluster_offset(uint32_t index)
{
    uint32_t resSize;
    uint32_t fatSize;
    uint32_t dirSize;

    ASSERT(index - 1, "index can't small than 1");

    struct BPB *p = get_fs_bpb();

    resSize = (uint32_t)p->reservedSectorCount * p->bytesPerSector;
    fatSize = (uint32_t)p->fatCount * p->sectorsPerFat * p->bytesPerSector;
    dirSize = (uint32_t)p->rootEntryCount * sizeof(struct dirEntry);

    return resSize + fatSize + dirSize +
           (index - 2) * ((uint32_t)p->sectorsPerCluster * p->bytesPerSector);
}

uint32_t FAT16::get_cluster_size(void)
{
    struct BPB *bpb = get_fs_bpb();
    return (uint32_t)bpb->bytesPerSector * bpb->sectorsPerCluster;
}

uint32_t FAT16::get_root_directory_count(void)
{
    struct BPB *bpb = get_fs_bpb();
    return bpb->rootEntryCount;
}

struct FAT16::dirEntry *FAT16::get_root_directory(void)
{
    struct BPB *p;
    uint32_t offset;

    p = get_fs_bpb();
    offset = (p->reservedSectorCount + (uint32_t)p->fatCount * p->sectorsPerFat) * p->bytesPerSector;

    return (struct dirEntry *)((uint8_t *)p + offset);
}

bool FAT16::is_file_name_equal(struct dirEntry *dir_entry, char *name, char *ext)
{
    bool status = false;

    if (memcmp(dir_entry->name, name, 8) == 0 &&
        memcmp(dir_entry->ext, ext, 3) == 0)
    {
        status = true;
    }

    return status;
}

bool FAT16::split_path(char *path, char *name, char *ext)
{
    int i;

    for (i = 0; i < 8 && path[i] != '.' && path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            return false;
        }

        name[i] = path[i];
    }

    if (path[i] == '.')
    {
        i++;

        for (int j = 0; j < 3 && path[i] != '\0'; i++, j++)
        {
            if (path[i] == '/')
            {
                return false;
            }

            ext[j] = path[i];
        }
    }

    if (path[i] != '\0')
    {
        return false;
    }

    return true;
}

uint32_t FAT16::search_file(char *path)
{
    char name[8] = {"        "};
    char ext[3] = {"   "};
    uint32_t root_entry_count;
    struct dirEntry *dir_entry;

    bool status = split_path(path, name, ext);

    if (status == true)
    {
        root_entry_count = get_root_directory_count();
        dir_entry = get_root_directory();

        for (uint32_t i = 0; i < root_entry_count; i++)
        {
            if (dir_entry[i].name[0] == ENTRY_EMPTY || dir_entry[i].name[0] == ENTRY_DELETED)
                continue;

            if (dir_entry[i].attributes == 0xf)
            {
                continue;
            }

            if (is_file_name_equal(&dir_entry[i], name, ext))
            {
                return i;
            }
        }
    }

    return 0xffffffff;
}

void FAT16::remove_spaces(char *input_string)
{
    for (int i = 0; input_string[i] != '\0'; ++i)
    {
        if (input_string[i] == ' ')
        {
            input_string[i] = '\0';
            break;
        }
    }
}

void FAT16::list_file(void)
{
    uint32_t root_entry_count;
    struct dirEntry *dir_entry;

    root_entry_count = get_root_directory_count();
    dir_entry = get_root_directory();

    printk("File list in root directory:\n");

    for (uint32_t i = 0; i < root_entry_count; i++)
    {
        if (dir_entry[i].name[0] == ENTRY_EMPTY || dir_entry[i].name[0] == ENTRY_DELETED)
            continue;

        if (dir_entry[i].attributes == 0xf)
        {
            continue;
        }
        char name[8] = {"        "};
        for (int j = 0; j < 8; j++)
        {
            name[j] = dir_entry[i].name[j];
        }
        remove_spaces(name);
        printk("%s.%s\t", name, dir_entry[i].ext);
    }
    printk("\n");
}

uint32_t FAT16::read_raw_data(uint32_t cluster_index, char *buffer, uint32_t size)
{
    struct BPB *bpb;
    char *data;
    uint32_t read_size = 0;
    uint32_t cluster_size;
    uint32_t index;

    bpb = get_fs_bpb();
    cluster_size = get_cluster_size();
    index = cluster_index;

    if (index < 2)
    {
        return 0xffffffff;
    }

    while (read_size < size)
    {
        data = (char *)((uint64_t)bpb + get_cluster_offset(index));
        index = get_cluster_value(index);

        if (index >= 0xfff7)
        {
            memcpy(buffer, data, size - read_size);
            read_size += size - read_size;
            break;
        }

        memcpy(buffer, data, cluster_size);

        buffer += cluster_size;
        read_size += cluster_size;
    }

    return read_size;
}

uint32_t FAT16::read_file(uint32_t cluster_index, void *buffer, uint32_t size)
{
    uint32_t rt = read_raw_data(cluster_index, (char *)buffer, size);
    return rt;
}

int FAT16::load_file(char *path, uint64_t addr)
{
    uint32_t index;
    uint32_t file_size;
    uint32_t cluster_index;
    struct dirEntry *dir_entry;
    int ret = -1;

    index = search_file(path);

    if (index != 0xffffffff)
    {
        dir_entry = get_root_directory();
        file_size = dir_entry[index].fileSize;
        cluster_index = dir_entry[index].clusterIndex;

        if (read_file(cluster_index, (void *)addr, file_size) == file_size)
        {
            ret = 0;
        }
    }

    return ret;
}

/** 
 * function: init_fs
 * return [void]
 * description: 初始化文件系统，检查文件系统的合法性，也就是检查文件系统的标志魔数
 */
void FAT16::init_fs(void)
{
    uint8_t *p = (uint8_t *)get_fs_bpb();

    if (p[0x1fe] != 0x55 || p[0x1ff] != 0xaa)
    {
        printk("invalid signature\n");
        ASSERT(0, "Can't find the file system\n");
    }
    else
    {
        // printk("file system is loaded\r\n");
    }
}

} // namespace Kernel
