cmake_minimum_required(VERSION 3.25)
project(mister8 VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(mister8)

target_include_directories(mister8 PUBLIC lib/raygui)

target_sources(mister8 
                       PRIVATE src/audio.h
                       PRIVATE src/audio.c
                       PRIVATE src/chip8.h
                       PRIVATE src/instructions.h
                       PRIVATE src/input.c
                       PRIVATE src/input.h
                       PRIVATE src/main.c
                       PRIVATE src/raygui_impl.c
                       PRIVATE src/util.c
                       PRIVATE src/util.h
              )
target_compile_options(mister8 PUBLIC -Wall -Wextra -Wpedantic -Wno-dollar-in-identifier-extension
                                      -Wshadow -Wconversion -O3
                                      )
target_link_options(mister8 PUBLIC -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
                   )