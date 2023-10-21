#pragma once

#include "memory.h"
#include "stdint.h"
#include "overloadNewK.hpp"

#define FS_BASE 0x30000000

namespace Kernel
{

class FAT16 : public NewK
{
private:
    struct BPB
    {
        uint8_t jump[3];
        uint8_t oem[8];
        uint16_t bytesPerSector;
        uint8_t sectorsPerCluster;
        uint16_t reservedSectorCount;
        uint8_t fatCount;
        uint16_t rootEntryCount;
        uint16_t sectorCount;
        uint8_t mediaType;
        uint16_t sectorsPerFat;
        uint16_t sectorsPerTrack;
        uint16_t headCount;
        uint32_t hiddenSectorCount;
        uint32_t largeSectorCount;
        uint8_t driveNumber;
        uint8_t flags;
        uint8_t signature;
        uint32_t volumeId;
        uint8_t volumeLabel[11];
        uint8_t fileSystem[8];
    } __attribute__((packed));

    struct dirEntry
    {
        uint8_t name[8];
        uint8_t ext[3];
        uint8_t attributes;
        uint8_t reserved;
        uint8_t createMs;
        uint16_t createTime;
        uint16_t createDate;
        uint16_t accessDate;
        uint16_t attrIndex;
        uint16_t mTime;
        uint16_t mDate;
        uint16_t clusterIndex;
        uint32_t fileSize;
    } __attribute__((packed));

#define ENTRY_EMPTY   0
#define ENTRY_DELETED 0xe5

    struct BPB *get_fs_bpb(void);
    uint16_t *get_fat_table(void);
    uint16_t get_cluster_value(uint32_t clusterIndex);
    uint32_t get_cluster_offset(uint32_t index);
    uint32_t get_cluster_size(void);
    uint32_t get_root_directory_count(void);
    struct dirEntry *get_root_directory(void);
    bool is_file_name_equal(struct dirEntry *dir_entry, char *name, char *ext);
    bool split_path(char *path, char *name, char *ext);
    uint32_t search_file(char *path);
    void remove_spaces(char *input_string);
    uint32_t read_raw_data(uint32_t cluster_index, char *buffer, uint32_t size);
    uint32_t read_file(uint32_t cluster_index, void *buffer, uint32_t size);

public:
    FAT16();  // Constructor, it will call init_fs
    ~FAT16(); // Optional destructor, if cleanup is needed
    void init_fs(void);
    void list_file(void);
    int load_file(char *path, uint64_t addr);
};

} // namespace Kernel
