cmake_minimum_required(VERSION 3.10)

project(parser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(${PROJECT_NAME}
    src/main.cc
    src/scanner.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE "include")
