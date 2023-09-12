/*
 * @Author: your name
 * @Date: 2021-10-16 15:24:57
 * @LastEditTime: 2023-02-04 18:27:40
 * @LastEditors: xubenji 459547070@qq.com
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /dolphin3/include/debug.h
 */
#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>

// 当e为false时，输出错误信息, info（string类型）为错误信息
#define ASSERT(e, infor)                            \
    do                                              \
    {                                               \
        if (!(e))                                   \
            error_check(__FILE__, __LINE__, infor); \
    } while (0)

void error_check(uint8_t *file, uint64_t line, uint8_t *infor);

#endif
