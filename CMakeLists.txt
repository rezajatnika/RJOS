cmake_minimum_required(VERSION 3.31)
project(RJOS C)

# Set the C Standard
# https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf
set(CMAKE_C_STANDARD 11)

# Define the source files for the core library.
set(CORE_SOURCES
        src/system.c
        src/scheduler.c
        src/serial.c
        src/udp.c
)

set(CORE_HEADERS
        src/system.h
        src/scheduler.h
        src/serial.h
        src/udp.h
)

# Define the source files for the configuration library.
set(CONFIG_SOURCES
        src/config.c
)

set(CONFIG_HEADERS
        src/config.h
)

# Compile RJOS library only.
add_library(rjos STATIC
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

# Main executable for the configuration example.
add_executable(rjos_config
        main_config.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

# Main executable for the UDP example.
add_executable(rjos_udp
        main_udp.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

# Main executable for the serial example.
add_executable(rjos_serial
        main_serial.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

add_executable(rjos_sched
        main_sched.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)
