cmake_minimum_required(VERSION 3.31)
project(RJOS C)

# Set the C Standard
# https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf
set(CMAKE_C_STANDARD 11)

# Define the source files for the core library.
set(CORE_SOURCES
        src/util/net_util.c
        src/config.c
        src/ipc.c
        src/system.c
        src/serial.c
        src/udp.c
        src/logger.c
        src/scheduler.c
        src/scheduler_pt.c
)

set(CORE_HEADERS
        src/util/net_util.h
        src/config.h
        src/ipc.h
        src/system.h
        src/serial.h
        src/udp.h
        src/logger.h
        src/scheduler.h
        src/scheduler_pt.h
)

set(RJOS_SRC
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

add_library(rjos STATIC
        ${RJOS_SRC}
)

# Main executable for the configuration example.
add_executable(rjos_config example/main_config.c)
target_link_libraries(rjos_config PRIVATE rjos)

# Main executable for the UDP example.
add_executable(rjos_udp example/main_udp.c)
target_link_libraries(rjos_udp PRIVATE rjos)

# Main executable for the serial example.
add_executable(rjos_serial example/main_serial.c)
target_link_libraries(rjos_serial PRIVATE rjos)

add_executable(rjos_sched example/main_sched.c)
target_link_libraries(rjos_sched PRIVATE rjos)

add_executable(rjos_sched_pt example/main_sched_pt.c)
target_link_libraries(rjos_sched_pt PRIVATE rjos)

add_executable(rjos_ipc example/main_ipc.c)
target_link_libraries(rjos_ipc PRIVATE rjos)

add_executable(rjos_logger example/main_logger.c)
target_link_libraries(rjos_logger PRIVATE rjos)
