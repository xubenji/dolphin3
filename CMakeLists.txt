cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_USE_FULL_PATH ON)
set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
set(CMAKE_ASM_COMPILER "aarch64-linux-gnu-gcc")
set(CMAKE_C_LINK_EXECUTABLE "aarch64-linux-gnu-ld")
set(CMAKE_ASM_LINKER "aarch64-linux-gnu-ld")
set(CMAKE_OBJCOPY "aarch64-linux-gnu-objcopy")
set(CMAKE_C_LINKER_FLAGS "-nostdlib -nostartfiles -T lds/link_arm.lds" )



# remove include path from flags
set(CMAKE_C_FLAGS "-Wall -O0 -nostdlib -nostartfiles -ffreestanding -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024")

include_directories(include)

add_executable(kernel8.elf
    boot/start.S
    kernel/kernel.c
    kernel/debug.c
    kernel/printk.c
    kernel/init.c
    kernel/register.c
    kernel/handler.c
    kernel/uart.c
    kernel/handlera.S
    kernel/lib.S
    kernel/mmu.S
)

set(CMAKE_ASM_FLAGS "-g")

set(CMAKE_ASM_COMPILE_OBJECT "aarch64-linux-gnu-gcc -g -c -o <OBJECT> <SOURCE>")
set_source_files_properties(boot/start.S PROPERTIES LANGUAGE ASM)
set_source_files_properties(kernel/handlera.S PROPERTIES LANGUAGE ASM)
set_source_files_properties(kernel/lib.S PROPERTIES LANGUAGE ASM)
set_source_files_properties(kernel/mmu.S PROPERTIES LANGUAGE ASM)



add_custom_command(TARGET kernel8.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:kernel8.elf> kernel8.img
)

# cmake_minimum_required(VERSION 3.10)
# project(myproject)

# set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
# set(CMAKE_ASM_COMPILER "aarch64-linux-gnu-gcc")
# set(CMAKE_OBJCOPY "aarch64-linux-gnu-objcopy")
# set(CMAKE_OBJDUMP "aarch64-linux-gnu-objdump")
# set(CMAKE_LINKER "aarch64-linux-gnu-ld")

# set(CMAKE_C_FLAGS "-Wall -O0 -nostdlib -nostartfiles -ffreestanding -Iinclude -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c99 -DMEM=1024")
# set(CMAKE_ASM_FLAGS "-Iinclude -g")
# include_directories(include)

# set(SOURCES
#     boot/start.S
#     kernel/kernel.c
#     kernel/debug.c
#     kernel/printk.c
#     kernel/init.c
#     kernel/register.c
#     kernel/handler.c
#     kernel/uart.c
#     kernel/handlera.S
#     kernel/lib.S
#     kernel/mmu.S
# )

# add_executable(kernel8.elf ${SOURCES})


# set_target_properties(kernel8.elf PROPERTIES 
#     LINK_FLAGS "-nostdlib -nostartfiles -T lds/link_arm.lds"
#     COMPILE_FLAGS ${CMAKE_ASM_FLAGS}
# )

# add_custom_command(TARGET kernel8.elf POST_BUILD
#     COMMAND ${CMAKE_OBJCOPY} -O binary kernel8.elf kernel8.img
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )
