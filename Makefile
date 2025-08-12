# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -std=c99 -I./src
LDFLAGS =

# Source and object files
SRCS = main.c src/scheduler.c src/system.c
OBJS = $(SRCS:.c=.o)

# Output binary
TARGET = rjos_demo

# Default target
all: $(TARGET)

# Link object files into final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Optional: run the demo
run: $(TARGET)
	./$(TARGET)
