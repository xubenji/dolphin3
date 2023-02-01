aarch64-linux-gnu-g++ -Iinclude -g -o start.o -c boot/start.S
aarch64-linux-gnu-g++ -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024 -o  kernel.o -c kernel/kernel.c 
aarch64-linux-gnu-g++ -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024 -o  debug.o -c kernel/debug.c 
aarch64-linux-gnu-g++ -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024 -o  printk.o -c kernel/printk.c 
aarch64-linux-gnu-g++ -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024 -o  init.o -c kernel/init.c 
#aarch64-linux-gnu-g++ -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024 -o  register.o -c kernel/register.c 
aarch64-linux-gnu-g++ -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024 -o  handler.o -c kernel/handler.c 
aarch64-linux-gnu-g++ -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024 -o  uart.o -c kernel/uart.c 
aarch64-linux-gnu-g++ -Iinclude -g -o handlera.o -c kernel/handlera.S
aarch64-linux-gnu-g++ -Iinclude -g -o lib.o -c kernel/lib.S
aarch64-linux-gnu-g++ -Iinclude -g -o mmu.o -c kernel/mmu.S
aarch64-linux-gnu-ld -nostdlib -nostartfiles -T lds/link_arm.lds -o kernel8.elf init.o mmu.o uart.o debug.o handler.o start.o kernel.o handlera.o lib.o printk.o
aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img
qemu-system-aarch64 -M raspi3 -m 1024 -kernel ./kernel8.img -nographic &
# rm -rf build/*
# cmake -B build
# cd build
# make VERBOSE=1

