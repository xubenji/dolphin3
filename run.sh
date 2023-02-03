cmake -S . -B build
bear cmake --build build -v
qemu-system-aarch64 -M raspi3 -m 1024 -kernel build/kernel8.img -nographic &