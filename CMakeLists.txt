cmake_minimum_required(VERSION 3.12)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(PicoSDKProject1 C CXX ASM)
include(PicoSDKConfig.cmake)
pico_sdk_init()

if(DEFINED SYSPROGS_FRAMEWORKS_FILE)
include(${SYSPROGS_FRAMEWORKS_FILE})
endif()
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_executable(blink
    main.cpp
    ESP8285.cpp)

# pull in common dependencies
target_link_libraries(blink pico_stdlib hardware_uart hardware_gpio)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(blink pico_cyw43_arch_none)
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)
set_source_files_properties(ESP8285.hpp PROPERTIES HEADER_FILE_ONLY TRUE)

# add url via pico_set_program_url
# example_auto_set_url(blink)
