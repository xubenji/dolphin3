#!/bin/bash

# 检查参数个数
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number>"
    exit 1
fi

# 定义单位转换函数
convert_units() {
    local bytes=$1

    local GB=$((bytes / 1024 / 1024 / 1024))
    local remainder_gb=$((bytes % (1024 * 1024 * 1024)))

    local MB=$((remainder_gb / 1024 / 1024))
    local remainder_mb=$((remainder_gb % (1024 * 1024)))

    local KB=$((remainder_mb / 1024))
    local remainder_kb=$((remainder_mb % 1024))

    local unit_str=""
    if [ $GB -gt 0 ]; then
        unit_str="${unit_str}${GB}GB"
    fi

    if [ $MB -gt 0 ]; then
        unit_str="${unit_str}${MB}MB"
    fi

    if [ $KB -gt 0 ]; then
        unit_str="${unit_str}${KB}KB"
    fi

    if [ $remainder_kb -gt 0 ]; then
        unit_str="${unit_str}${remainder_kb}B"
    fi

    echo "Equivalent size: $unit_str"
}

# 获取输入参数
input=$1

# 使用正则表达式检查参数是十进制还是十六进制
if [[ $input =~ ^0[xX][0-9a-fA-F]+$ ]]; then
    # 十六进制
    dec_num=$((input)) # 使用内建算术运算将十六进制转换为十进制
    echo "Input is hexadecimal: $input"
    echo "Converted to decimal: $dec_num"
    convert_units $dec_num
elif [[ $input =~ ^[0-9]+$ ]]; then
    # 十进制
    hex_num=$(printf '%x\n' $input)
    echo "Input is decimal: $input"
    echo "Converted to hexadecimal: $hex_num"
    convert_units $input
else
    # 输入无效
    echo "Invalid input. Please enter a decimal or hexadecimal number."
    echo "对于十六进制数：./convert_number.sh 0x1FF
          对于十进制数：./convert_number.sh 1024"
    exit 1
fi
