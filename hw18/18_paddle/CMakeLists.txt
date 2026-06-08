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

project(18_paddle C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(18_paddle 18_paddle.c
ina219.c
as5600.c
../../pegues_lib/pegues_lib.c
../../pegues_lib/ssd1306.c
../../pegues_lib/i2c.c
../../pegues_lib/spi.c)

pico_set_program_name(18_paddle "18_paddle")
pico_set_program_version(18_paddle "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(18_paddle 0)
pico_enable_stdio_usb(18_paddle 1)

# Add the standard library to the build
target_link_libraries(18_paddle
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(18_paddle PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(18_paddle
                        hardware_i2c
                        hardware_spi
                        hardware_pwm
                        pico_cyw43_arch_none
)

pico_add_extra_outputs(18_paddle)

