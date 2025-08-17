cmake_minimum_required(VERSION 3.31)
project(RJOS C)

# Set the C Standard
# https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf
set(CMAKE_C_STANDARD 11)

# Define the source files for the core library.
set(CORE_SOURCES
        src/ipc.c
        src/system.c
        src/scheduler.c
        src/serial.c
        src/udp.c
        src/logger.c
)

set(CORE_SOURCES_PT
        src/ipc.c
        src/system.c
        src/scheduler_pt.c
        src/serial.c
        src/udp.c
        src/logger.c
)

set(CORE_HEADERS
        src/ipc.h
        src/system.h
        src/scheduler.h
        src/serial.h
        src/udp.h
        src/logger.h
)

set(CORE_HEADERS_PT
        src/ipc.h
        src/system.h
        src/scheduler_pt.h
        src/serial.h
        src/udp.h
        src/logger.h
)

# Define the source files for the configuration library.
set(CONFIG_SOURCES
        src/config.c
)

set(CONFIG_HEADERS
        src/config.h
)

# Compile RJOS library only.
include_directories(src)

add_library(rjos STATIC
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
        ${CORE_SOURCES_PT}
        ${CORE_HEADERS_PT}
)

# Main executable for the configuration example.
add_executable(rjos_config
        example/main_config.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

# Main executable for the UDP example.
add_executable(rjos_udp
        example/main_udp.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

# Main executable for the serial example.
add_executable(rjos_serial
        example/main_serial.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

add_executable(rjos_sched
        example/main_sched.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

add_executable(rjos_sched_pt
        example/main_sched_pt.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES_PT}
        ${CORE_HEADERS_PT}
)

add_executable(rjos_ipc
        example/main_ipc.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

add_executable(rjos_logger
        example/main_logger.c
        src/logger.c
        src/logger.h
)
