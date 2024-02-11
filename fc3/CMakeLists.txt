cmake_minimum_required(VERSION 3.16)

# create compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../ext/stm32-cmake/cmake/stm32_gcc.cmake)
set(FAMILY "G0" CACHE STRING "Target STM32 family")
set(PROJ_NAME fc3)
set(TARGET_NAME ${PROJ_NAME}-${FAMILY})
set(HAL_COMP_LIST
    RCC
    GPIO
    CORTEX
    UART
    )
set(CMSIS_COMP_LIST "")

add_definitions(-DFC3_${FAMILY})
message(STATUS "Configuring for STM32${FAMILY} series")
if(${FAMILY} STREQUAL "G0")
    set(FC3_G0 ON CACHE BOOL "Compile for G0" FORCE)
else()
    set(FC3_G0 OFF CACHE BOOL "Compile for G0" FORCE)
endif()

if(${FAMILY} STREQUAL "F1")
    set(FC3_F1 ON CACHE BOOL "Compile for F1" FORCE)
else()
    set(FC3_F1 OFF CACHE BOOL "Compile for F1" FORCE)
endif()

project(${PROJ_NAME} C ASM)

list(APPEND CMSIS_COMP_LIST STM32${FAMILY})
list(APPEND HAL_COMP_LIST STM32${FAMILY})
set(STM32_CUBE_${FAMILY}_PATH $ENV{STM32_CUBE_${FAMILY}_PATH} CACHE PATH "Path to STM32Cube${FAMILY}")

message(STATUS "CMAKE_TOOLCHAIN_FILE is set to ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "STM32_CUBE_${FAMILY}_PATH is set to ${STM32_CUBE_${FAMILY}_PATH}")
message(STATUS "STM32_CMAKE_DIR is set to ${STM32_CMAKE_DIR}")
message(STATUS "CMAKE_MODULE_PATH is set to ${CMAKE_MODULE_PATH}")
find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)
find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)

# FIXME: add FC3_F1
if(FC3_G0)
    # FIXME: put hal_conf into headerfile
    add_executable(${TARGET_NAME} src/main.c src/stm32g0xx_hal_conf.h)
    target_link_libraries(${TARGET_NAME}
        HAL::STM32::G0::RCC
        HAL::STM32::G0::GPIO
        HAL::STM32::G0::CORTEX
        HAL::STM32::G0::UART
        HAL::STM32::G0::UARTEx
        CMSIS::STM32::G070RB
        STM32::NoSys
    )
	target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    stm32_print_size_of_target(${TARGET_NAME})
    stm32_generate_hex_file(${TARGET_NAME})
endif()

add_custom_target(flash
    COMMAND STM32_Programmer_CLI -c port=SWD -w ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex -v -rst
    COMMENT "Flashing the board..."
)
