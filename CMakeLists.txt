cmake_minimum_required(VERSION 3.21)
project(spritzenschwurbler_c C)

set(CMAKE_C_STANDARD 99)

add_executable(spritzenschwurbler_c src/main.c src/utils/injector.c src/utils/injector.h src/utils/process_t.h)
