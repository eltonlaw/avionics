cmake_minimum_required(VERSION 3.16)
project(stlib C ASM)

get_filename_component(ABS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ext/stm32-cmake/cmake" ABSOLUTE)
list(APPEND CMAKE_MODULE_PATH ${ABS_PATH})
list(APPEND STM32_CMAKE_DIR ${ABS_PATH})
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../ext/stm32-cmake/cmake/stm32_gcc.cmake)
set(FAMILY "G0" CACHE STRING "Target STM32 family")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
# set(STM32_CMAKE_DIR $ENV{STM32_CMAKE_DIR})
# set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH})

set(HAL_COMP_LIST
    CORTEX
    GPIO
    PWR
    RCC
    TIM
    UART
	STM32${FAMILY}
)
set(TARGET_NAME stlib-${FAMILY})
set(CMSIS_COMP_LIST STM32${FAMILY})
set(STM32_CUBE_${FAMILY}_PATH $ENV{STM32_CUBE_${FAMILY}_PATH} CACHE PATH "Path to STM32Cube${FAMILY}")

add_compile_options(
    -std=gnu11
    -Wall
    -ffunction-sections
    -fdata-sections
    -fstack-usage
    -mfloat-abi=soft
    -DUSE_HAL_DRIVER
)
message(STATUS "CMAKE_TOOLCHAIN_FILE is set to ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_MODULE_PATH is set to ${CMAKE_MODULE_PATH}")
message(STATUS "STM32_CUBE_${FAMILY}_PATH is set to ${STM32_CUBE_${FAMILY}_PATH}")
message(STATUS "STM32_CMAKE_DIR is set to ${STM32_CMAKE_DIR}")
find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)
find_package(CMSIS COMPONENTS STM32G0 REQUIRED)

include_directories(Inc)
file(GLOB_RECURSE SOURCES "Src/*.c")
add_library(${TARGET_NAME} ${SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC Inc)

if(${FAMILY} STREQUAL "G0")
	add_compile_options(
		-mcpu=cortex-m0plus
		-DSTM32G070xx
        -mthumb
	)
    target_link_libraries(${TARGET_NAME}
        HAL::STM32::G0::CORTEX
        HAL::STM32::G0::GPIO
        HAL::STM32::G0::PWR
        HAL::STM32::G0::PWREx
        HAL::STM32::G0::RCC
        HAL::STM32::G0::TIM
        HAL::STM32::G0::TIMEx
        HAL::STM32::G0::UART
        HAL::STM32::G0::UARTEx
        CMSIS::STM32::G070RB
        STM32::NoSys
    )
endif()
