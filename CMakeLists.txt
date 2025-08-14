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

# Main executable for the configuration example.
add_executable(RJOS_config
        main_config.c
        ${CONFIG_SOURCES}
        ${CONFIG_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)
