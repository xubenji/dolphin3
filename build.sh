# 只编译不运行！
# rm -rf build/*
# cmake -B build
# cd build
# make VERBOSE=1
pkill qemu
rm -rf build/*
rm ./compile_commands.json
cmake -S . -B build -DOPTIMIZATION_LEVEL=-O2
bear cmake --build build

