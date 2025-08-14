cmake_minimum_required(VERSION 3.31)
project(RJOS C)

set(CMAKE_C_STANDARD 11)

add_executable(RJOS main.c
        src/scheduler.h
        src/scheduler.c
        src/serial.h
        src/serial.c
        src/system.h
        src/system.c
        src/udp.h
        src/udp.c)

add_executable(RJOS_config main_config.c
        src/config.c
        src/config.h
        src/scheduler.h
        src/scheduler.c
        src/serial.h
        src/serial.c
        src/system.h
        src/system.c
        src/udp.h
        src/udp.c)
