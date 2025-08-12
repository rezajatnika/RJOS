![RJOS: Real-Time OS for Embedded Systems](https://gitlab.com/rezajatnika/rjos/-/raw/main/assets/rjos-logo.png?ref_type=heads&inline=false)

# RJOS: RJ Operating System
RJOS is a minimalist, purpose-built operating system designed for embedded and system-level
applications that require deterministic behavior and continuous operation. It provides a
lightweight foundation for infinite-loop system applications, with a focus on modularity,
reliability, and real-time responsiveness.

## Core Components
- **Task Scheduler**: A high-precision, interval-based scheduler for managing recurring tasks. 
Supports modular task definitions, configurable execution intervals, and efficient CPU usage for 
real-time systems.
- **UDP Driver**: Modular network interface for sending and receiving datagrams.


## Design Philosophy
- **Bare-metal simplicity**: No unnecessary abstractions—just the essentials for predictable,
continuous operation.