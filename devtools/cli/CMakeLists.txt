cmake_minimum_required(VERSION 3.10)

# Project name
project(cli)

# Add the executable
add_executable(${PROJECT_NAME}
    cli/main.c
    cli/buildtools.c
)
include_directories(cli)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

# target_link_libraries( library_name)
