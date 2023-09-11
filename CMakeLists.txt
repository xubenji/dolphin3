cmake_minimum_required(VERSION 3.10)

# Set the project name
project(KernelProject)

# Set the compiler to use
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_ASM_COMPILER aarch64-linux-gnu-g++)

include_directories(include)
# Get the current project directory
get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
message("Project directory: ${PROJECT_DIR}")
set(LINKER_SCRIPT ${PROJECT_DIR}/lds/link_arm.lds)

# 设置一个默认的优化级别
set(OPTIMIZATION_LEVEL "-O0" CACHE STRING "Optimization level for compilation")

# Set the flags to use
# set(CXX_FLAGS -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -g -fno-stack-protector -c -w -mgeneral-regs-only -fno-exceptions -fno-rtti -std=c++11 -DMEM=1024)
set(CXX_FLAGS -Wall ${OPTIMIZATION_LEVEL} -nostdlib -nostartfiles -ffreestanding -fpermissive -g -fno-stack-protector -c -w -mgeneral-regs-only -fno-exceptions -fno-rtti -std=c++11 -DMEM=1024)
set(ASM_FLAGS -g)

# Set the linker flags
set(LINKER_FLAGS -nostdlib -nostartfiles)

# Add the source files
file(GLOB_RECURSE SOURCE_FILES "kernel/*.cpp" "kernel/*.S" "boot/*.S" "fs/*.cpp")

# Compile .S files
foreach(ASM_FILE ${SOURCE_FILES})
    if(ASM_FILE MATCHES ".*\\.S$")
        get_filename_component(ASM_FILE_BASENAME ${ASM_FILE} NAME_WE)
        set(ASM_OBJECT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${ASM_FILE_BASENAME}.o")
        add_custom_command(OUTPUT ${ASM_OBJECT_FILE}
                           COMMAND ${CMAKE_ASM_COMPILER} ${ASM_FLAGS} -o ${ASM_OBJECT_FILE} -c ${ASM_FILE}
                           DEPENDS ${ASM_FILE}
                           COMMENT "Building Assembly object ${ASM_OBJECT_FILE}")
        set(ASM_OBJECT_FILES ${ASM_OBJECT_FILES} ${ASM_OBJECT_FILE})
    endif()
endforeach()

# Add the binary target
add_executable(kernel8.elf ${SOURCE_FILES} ${ASM_OBJECT_FILES})
target_compile_options(kernel8.elf PRIVATE ${CXX_FLAGS})
target_link_options(kernel8.elf PRIVATE ${LINKER_FLAGS})
set_target_properties(kernel8.elf PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT}")

# Post-process kernel8.elf to produce kernel8.img
add_custom_command(TARGET kernel8.elf POST_BUILD
    COMMAND aarch64-linux-gnu-objcopy ARGS -O binary ${CMAKE_BINARY_DIR}/kernel8.elf ${CMAKE_BINARY_DIR}/kernel8.img)

# Add a custom target for the dd command
add_custom_target(dd_command
    COMMAND dd if=${CMAKE_SOURCE_DIR}/disk.img of=${CMAKE_BINARY_DIR}/kernel8.img oflag=append conv=notrunc > /dev/null 2>&1
    COMMENT "Appending disk.img to kernel8.img"
)
add_dependencies(dd_command kernel8.elf)

# Add custom target for the qemu-system-aarch64 with debugging named run_debug
add_custom_target(run_debug
    COMMAND qemu-system-aarch64 -M raspi3 -m 1024 -kernel ${CMAKE_BINARY_DIR}/kernel8.img -nographic -s -S 
    COMMENT "Running kernel8.img with qemu (debug mode), please wait seconds..."
)

add_dependencies(run_debug dd_command)

# Add custom target for the qemu-system-aarch64 without debugging named run
add_custom_target(run
    COMMAND qemu-system-aarch64 -M raspi3 -m 1024 -kernel ${CMAKE_BINARY_DIR}/kernel8.img -nographic 
    COMMENT "Running kernel8.img with qemu, please wait seconds..."
)
add_dependencies(run dd_command)
