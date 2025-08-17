cmake_minimum_required(VERSION 3.31)
project(RJOS C)

# Set the C Standard
# https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf
set(CMAKE_C_STANDARD 11)

# Define the source files for the core library.
set(SCHED_SOURCES
        src/scheduler.c
)

set(SCHED_HEADERS
        src/scheduler.h
)

set(SCHED_PT_SOURCES
        src/scheduler_pt.c
)

set(SCHED_PT_HEADERS
        src/scheduler_pt.h
)

set(CORE_SOURCES
        src/util/net_util.c
        src/config.c
        src/ipc.c
        src/system.c
        src/serial.c
        src/udp.c
        src/logger.c
)

set(CORE_HEADERS
        src/util/net_util.h
        src/config.h
        src/ipc.h
        src/system.h
        src/serial.h
        src/udp.h
        src/logger.h
)

set(RJOS_SRC
        ${CORE_SOURCES}
        ${CORE_HEADERS}
        ${SCHED_SOURCES}
        ${SCHED_HEADERS}
)

# Compile RJOS library only.
include_directories(src)

add_library(rjos STATIC
        ${CORE_SOURCES}
        ${CORE_HEADERS}
        ${SCHED_SOURCES}
        ${SCHED_HEADERS}
)

# Main executable for the configuration example.
add_executable(rjos_config
        example/main_config.c
        ${RJOS_SRC}
)

# Main executable for the UDP example.
add_executable(rjos_udp
        example/main_udp.c
        ${RJOS_SRC}
)

# Main executable for the serial example.
add_executable(rjos_serial
        example/main_serial.c
        ${RJOS_SRC}
)

add_executable(rjos_sched
        example/main_sched.c
        ${RJOS_SRC}
)

add_executable(rjos_sched_pt
        example/main_sched_pt.c
        ${RJOS_SRC}
)

add_executable(rjos_ipc
        example/main_ipc.c
        ${RJOS_SRC}
)

add_executable(rjos_logger
        example/main_logger.c
        src/logger.c
        src/logger.h
)
