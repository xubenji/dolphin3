
#rm -rf build/*
cmake -S . -B build
bear cmake --build build
if [ "$1" = "D" ]; then
    echo "debug mode"
    qemu-system-aarch64 -M raspi3 -m 1024 -kernel build/kernel8.img -nographic -s -S &
    sleep 0.5
else
    qemu-system-aarch64 -M raspi3 -m 1024 -kernel build/kernel8.img -nographic &
fi