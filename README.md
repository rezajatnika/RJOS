![Build Status](https://github.com/rezajatnika/RJOS/actions/workflows/ci.yml/badge.svg)
# RJOS
The **RJOS** is a lightweight C framework designed for building real-time and embedded applications.
It provides modular components for scheduling, inter-process communication, logging, and I/O management,
enabling developers to efficiently manage complex systems without being tied to a specific operating system.

## Features
### 1. Task Scheduler
- High precision interval-based scheduling for managing recurring tasks.
- Supports modular task definitions with configurable execution intervals.
- Includes **Preemptive Multitasking** via the `scheduler_pt` module.
- Optimized for efficient CPU usage in real-time systems.

### 2. Inter-process Communication (IPC)
- Lightweight shared memory communication across processes.
- Implements data synchronization and message passing.

### 3. UDP Driver
- Modular networking interface for sending and receiving datagrams.
- Built for fast and reliable communication in networked environments.
- Ideal for lightweight and real-time data exchanges.

### 4. Serial Port Driver
- Provides communication capabilities with serial devices.
- Supports opening, configuring, reading, writing, and closing serial ports.
- Facilitates communication with embedded peripherals and external devices.

### 5. Configuration System
- Modular configuration management provided by `config.c` and `config.h`.
- Enables flexible runtime adjustments to application settings.

### 6. System Abstraction Layer
- Simplifies interaction with hardware and OS-specific components.
- Designed for portability and clean separation of concerns.

## Build Instructions

### Prerequisites
Ensure the following tools are installed and available in your system's PATH:
- `cmake` (≥ version 3.10)
- `ninja` (build backend)

### Build Steps
To compile all builds and configurations:
1. Navigate to the `scripts` directory:
   ```bash
   cd scripts
   ```
2. Execute the build script:
   ```bash
   ./rjos_build.sh
   ```
3. The compiled binaries will be in `bin/<BUILD_TYPE>` for each configuration.

## Examples
Example applications for various components are located in the `example` directory:
- `main_ipc.c`: Demonstrates the use of Inter-process Communication (IPC).
- `main_serial.c`: Implements communication over serial ports.
- `main_udp.c`: Showcases UDP communication.
- `main_sched.c`: Demonstrates the basic task scheduler in action.
- `main_sched_pt.c`: Implements a preemptive multitasking scheduler.
- `main_config.c`: Example of configuration management in RJOS.

## Contributing
Contributions are welcome! Submit issues, feature requests, or pull requests via the project's repository.

## License
This project is licensed under the terms described in `LICENSE.md`.
