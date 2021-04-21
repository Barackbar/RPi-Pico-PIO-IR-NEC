
cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(pio_ir_nec C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(pio_ir_nec)

# by default the header is generated into the build dir
pico_generate_pio_header(pio_ir_nec ${CMAKE_CURRENT_LIST_DIR}/ir_nec.pio)

target_sources(pio_ir_nec PRIVATE ir_nec.c)

target_link_libraries(pio_ir_nec PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(pio_ir_nec)
