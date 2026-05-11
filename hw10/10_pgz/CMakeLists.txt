# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(10_pgz C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(10_pgz 10_pgz.c
                     ../../pegues_lib/pegues_lib.c
                     ../../pegues_lib/ssd1306.c
                     ../../pegues_lib/i2c.c
                     ../../pegues_lib/spi.c)

pico_set_program_name(10_pgz "10_pgz")
pico_set_program_version(10_pgz "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(10_pgz 0)
pico_enable_stdio_usb(10_pgz 1)

# Add the standard library to the build
target_link_libraries(10_pgz
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(10_pgz PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(10_pgz
                      hardware_i2c
                      hardware_spi
                      pico_cyw43_arch_none
        )

pico_add_extra_outputs(10_pgz)

