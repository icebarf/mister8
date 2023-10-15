cmake_minimum_required(VERSION 3.25)
project(mister8 VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(mister8)
target_sources(mister8 
                       PRIVATE src/instructions.h
                       PRIVATE src/chip8.h
                       PRIVATE src/main.c
                       PRIVATE src/util.c
                       PRIVATE src/util.h
              )
target_compile_options(mister8 PUBLIC -Wall -Wextra -Wpedantic
                                      -Wshadow -Wconversion -Wno-dollar-in-identifier-extension -g -Og)