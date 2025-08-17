![Build Status](https://github.com/rezajatnika/RJOS/actions/workflows/ci.yml/badge.svg)
# RJOS: RJ Operating System

RJOS is a minimalist, purpose-built operating system designed for embedded and system-level applications that require deterministic behavior and continuous operation. It provides a lightweight foundation for infinite-loop system applications, with a focus on modularity, reliability, and real-time responsiveness.

## Features

### 1. Task Scheduler
- High-precision interval-based scheduling for managing recurring tasks.
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

### Example Build Configurations
The build script supports the following configurations:
- `Debug`: Build with debug symbols.
- `Release`: Optimized build for deployment.
- `RelWithDebInfo`: Release build with additional debug information.
- `MinSizeRel`: Optimized for minimum size.

## Design Philosophy
- **Bare-metal simplicity**: No unnecessary abstractions—just the essentials for predictable, continuous operation.
- **Modularity**: Flexible and extensible for specific use cases.
- **Efficiency**: Optimized components for embedded and resource-constrained environments.
- **Real-time responsiveness**: Designed to prioritize time-critical operations.

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
