cmake_minimum_required(VERSION 3.31)
project(RJOS C)

# Set the C Standard
# https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf
set(CMAKE_C_STANDARD 11)

# Enable AddressSanitizer
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# Define the source files for the core library.
set(CORE_SOURCES
        src/config.c
        src/ipc.c
        src/logger.c
        src/rjos.c
        src/scheduler.c
        src/serial.c
        src/system.c
        src/udp.c
        src/util/net_util.c
        src/util/sched_util.c
)

set(CORE_HEADERS
        src/config.h
        src/ipc.h
        src/logger.h
        src/rjos.h
        src/scheduler.h
        src/serial.h
        src/system.h
        src/udp.h
        src/util/net_util.h
        src/util/sched_util.h
)

set(RJOS_SRC
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)

add_library(rjos STATIC
        ${RJOS_SRC}
)

include_directories("src")

add_executable(rjos_config example/main_config.c)
target_link_libraries(rjos_config PRIVATE rjos)

add_executable(rjos_udp example/main_udp.c)
target_link_libraries(rjos_udp PRIVATE rjos)

add_executable(rjos_serial example/main_serial.c)
target_link_libraries(rjos_serial PRIVATE rjos)

add_executable(rjos_sched example/main_sched.c)
target_link_libraries(rjos_sched PRIVATE rjos)

add_executable(rjos_ipc example/main_ipc.c)
target_link_libraries(rjos_ipc PRIVATE rjos)

add_executable(rjos_logger example/main_logger.c)
target_link_libraries(rjos_logger PRIVATE rjos)