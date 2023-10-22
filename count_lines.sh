#!/bin/bash

# 检查参数的数量
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

# 检查参数是否是一个目录
if [ ! -d "$1" ]; then
    echo "Error: $1 is not a directory."
    exit 2
fi

# 使用find命令找到所有的.cpp, .hpp和.S文件，然后使用wc命令计算总行数
total_lines=$(find "$1" -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.S" \) -exec wc -l {} + | tail -n 1 | awk '{print $1}')

echo "Total lines in .cpp, .hpp and .S files: $total_lines"

