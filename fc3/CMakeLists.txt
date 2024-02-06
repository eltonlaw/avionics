cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../ext/stm32-cmake/cmake/stm32_gcc.cmake)
project(fc3 C ASM)

option(FC3_G0 "Compile G0" ON)
option(FC3_F1 "Compile F1" OFF)
set(HAL_COMP_LIST RCC GPIO CORTEX)
set(CMSIS_COMP_LIST "")
if(FC3_G0)
    message(STATUS "Configuring for STM32G0 series")
    set(FAMILY G0)
    list(APPEND CMSIS_COMP_LIST STM32G0)
    list(APPEND HAL_COMP_LIST STM32G0)
endif()
if(FC3_F1)
    set(FAMILY F1)
    message(STATUS "Configuring for STM32F1 series")
    list(APPEND CMSIS_COMP_LIST STM32F1)
    list(APPEND HAL_COMP_LIST STM32F1)
endif()
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
    add_executable(fc3 src/main.c src/stm32g0xx_hal_conf.h)
    target_link_libraries(fc3
        HAL::STM32::G0::RCC
        HAL::STM32::G0::GPIO
        HAL::STM32::G0::CORTEX
        CMSIS::STM32::G070RB
        STM32::NoSys
    )
    stm32_print_size_of_target(fc3)
endif()
