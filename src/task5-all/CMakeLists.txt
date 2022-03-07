cmake_minimum_required(VERSION 3.20)
project(myshell C)

set(CMAKE_C_STANDARD 99)
add_executable(myshell main.c)

add_subdirectory(myShellLib)

target_link_libraries(${PROJECT_NAME} myShellLib)