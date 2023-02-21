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

# Set the flags to use
set(CXX_FLAGS -Wall -O0 -nostdlib -nostartfiles -ffreestanding -fpermissive -g -fno-stack-protector -c -w -mgeneral-regs-only -std=c++11 -DMEM=1024)
set(ASM_FLAGS -g)

# Set the linker flags
set(LINKER_FLAGS -nostdlib -nostartfiles)

# Add the source files
file(GLOB_RECURSE SOURCE_FILES "kernel/*.cpp" "kernel/*.S" "boot/*.S")

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

# Add the binary target
add_custom_command(TARGET kernel8.elf POST_BUILD
    COMMAND aarch64-linux-gnu-objcopy ARGS -O binary ${CMAKE_BINARY_DIR}/kernel8.elf ${CMAKE_BINARY_DIR}/kernel8.img)