# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -std=c11 -I./src
AR = ar
ARFLAGS = rcs

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files for core and config modules
CORE_SRCS = $(SRC_DIR)/system.c $(SRC_DIR)/scheduler.c $(SRC_DIR)/serial.c $(SRC_DIR)/udp.c $(SRC_DIR)/ipc.c
CORE_HEADERS = $(SRC_DIR)/system.h $(SRC_DIR)/scheduler.h $(SRC_DIR)/serial.h $(SRC_DIR)/udp.h $(SRC_DIR)/ipc.h
CONFIG_SRCS = $(SRC_DIR)/config.c
CONFIG_HEADERS = $(SRC_DIR)/config.h

# Object files
CORE_OBJS = $(CORE_SRCS:%.c=$(BUILD_DIR)/%.o)
CONFIG_OBJS = $(CONFIG_SRCS:%.c=$(BUILD_DIR)/%.o)

# Static library
LIBRARY = $(BUILD_DIR)/librjos.a

# Executables
CONFIG_EXEC = rjos_config
UDP_EXEC = rjos_udp
SERIAL_EXEC = rjos_serial
SCHED_EXEC = rjos_sched
SCHED_PT_EXEC = rjos_sched_pt
IPC_EXEC = rjos_ipc

# Default target
all: $(LIBRARY) $(CONFIG_EXEC) $(UDP_EXEC) $(SERIAL_EXEC) $(SCHED_EXEC) $(SCHED_PT_EXEC) $(IPC_EXEC)

# Rule to build the static library
$(LIBRARY): $(CORE_OBJS) $(CONFIG_OBJS)
	mkdir -p $(BUILD_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Rule to build the config example executable
$(CONFIG_EXEC): example/main_config.c $(LIBRARY)
	$(CC) $(CFLAGS) example/main_config.c $(LIBRARY) -o $@

# Rule to build the UDP example executable
$(UDP_EXEC): example/main_udp.c $(LIBRARY)
	$(CC) $(CFLAGS) example/main_udp.c $(LIBRARY) -o $@

# Rule to build the serial example executable
$(SERIAL_EXEC): example/main_serial.c $(LIBRARY)
	$(CC) $(CFLAGS) example/main_serial.c $(LIBRARY) -o $@

# Rule to build the scheduler example executable
$(SCHED_EXEC): example/main_sched.c $(LIBRARY)
	$(CC) $(CFLAGS) example/main_sched.c $(LIBRARY) -o $@

# Rule to build the scheduler pt example executable
$(SCHED_PT_EXEC): example/main_sched_pt.c $(LIBRARY)
	$(CC) $(CFLAGS) example/main_sched_pt.c $(LIBRARY) -o $@

# Rule to build the IPC example executable
$(IPC_EXEC): example/main_ipc.c $(LIBRARY)
	$(CC) $(CFLAGS) example/main_ipc.c $(LIBRARY) -o $@

# Rule to compile core source files
$(BUILD_DIR)/%.o: %.c $(CORE_HEADERS)
	mkdir -p $(BUILD_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile config source files
$(BUILD_DIR)/%.o: %.c $(CONFIG_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(CONFIG_EXEC) $(UDP_EXEC) $(SERIAL_EXEC) $(SCHED_EXEC) $(SCHED_PT_EXEC) $(IPC_EXEC)

# Phony targets
.PHONY: all clean