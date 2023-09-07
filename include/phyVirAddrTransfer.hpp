#pragma once
#define KERNEL_BASE 0xffff000000000000
//物理地址转虚拟地址
#define P2V(p) ((uint64_t)(p) + KERNEL_BASE)
//虚拟地址转物理地址
#define V2P(v) ((uint64_t)(v) - KERNEL_BASE)
