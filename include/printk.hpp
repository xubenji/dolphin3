#ifndef _PRINTK_H
#define _PRINTK_H
#include <stdint.h>
#define LINE_SIZE 160

struct ScreenBuffer
{
    char *buffer;
    int column;
    int row;
};
#ifdef __cplusplus
extern "C"
{
#endif
    void printk(const char *format, ...);
#ifdef __cplusplus
}
#endif

int read_string(char *buffer, int position, const char *string);
int udecimal_to_string(char *buffer, int position, uint64_t digits);
int decimal_to_string(char *buffer, int position, int64_t digits);
int hex_to_string(char *buffer, int position, uint64_t digits);
void puts(char *str);

#endif
