# Comprehensive Operating Systems Mini-Project
This repository contains a massive, multi-phase Operating Systems project covering user-space system programming, network protocol design, and deep kernel-level modifications in xv6.
## 🚀 Phase 1-3: Custom C-Shell
A fully functional, POSIX-compliant UNIX shell built from scratch in C. The shell is capable of executing standard binaries, managing foreground and background processes, and handling complex I/O pipelines.
### Features:
- **Built-in Commands**:
  - `hop` (cd equivalent): Changes directory and prints the absolute path. Supports `~`, `-`, `.`, and `..`.
  - `reveal` (ls equivalent): Lists files and directories with detailed information, hidden file toggles, and color-coded output.
  - `log`: A persistent command history system that stores up to 15 commands. Supports executing previous commands via `log execute <index>`.
  - `ping`: Sends signals (like `SIGTERM`, `SIGKILL`) to processes directly from the shell using custom job IDs.
  - `activities`: Displays a list of all currently running background processes spawned by the shell.
  - `fg` / `bg`: Brings background processes to the foreground, or resumes stopped processes in the background.
- **Process Management**: Native support for background execution using the `&` operator. Tracks process states and reaps zombies.
- **I/O Redirection**: Supports standard input `<` and output `>`, `>>` redirection for commands.
- **Piping**: Supports arbitrary-length command pipelines (`cmd1 | cmd2 | cmd3`).
- **Signal Handling**: Gracefully handles `Ctrl+C` (SIGINT) and `Ctrl+Z` (SIGTSTP) without crashing the shell, delegating signals to foreground children.
---
## 🌐 Phase 4: Reliable UDP Networking Protocol
A custom reliable data transfer protocol built on top of UDP sockets, simulating TCP-like reliability guarantees using a custom **S.H.A.M.** header.
### Features:
- **3-Way Handshake**: Connection establishment using `SYN`, `SYN-ACK`, and `ACK` to synchronize initial sequence numbers securely.
- **Sliding Window Protocol**: Implements flow control and reliable delivery for transferring massive files over lossy networks.
- **Timeout & Retransmission**: Uses `select()` to implement a 500ms Retransmission Timeout (RTO) for unacknowledged packets.
- **Cumulative ACKs**: The receiver acknowledges chunks of data cumulatively, ensuring data is written exactly in order.
- **Simulated Packet Loss**: Includes arguments to simulate packet drop rates to test network resilience.
- **Logging Mode**: High-precision microsecond-level timestamps for logging all connection states and flow control events.
---
## 🧠 Phase 5: xv6 Kernel Modifications
Deep modifications to the MIT xv6 Operating System kernel to add custom system calls and replace the default scheduling algorithm.
### Features:
- **New System Call (`getreadcount`)**: Added a custom `sys_getreadcount` syscall that tracks the total number of bytes read globally across all processes using the `read()` syscall.
- **First Come First Serve (FCFS)**: Replaced the default Round Robin scheduler with a non-preemptive FCFS policy based on process creation time.
- **Completely Fair Scheduler (CFS)**: Implemented a simplified version of Linux's CFS. Processes are assigned a `nice` value which dictates their system weight. The scheduler tracks normalized `vruntime` and dynamically assigns time slices to ensure fair CPU distribution.
- **Preemptive MLFQ Bonus**: Implemented a 4-queue Multi-Level Feedback Queue scheduler. Processes start at high priority (1 tick slice) and demote to lower priorities (4, 8, 16 ticks) if they consume their entire CPU slice, with priority boosting to prevent starvation.
---
## 🛠️ Build & Usage
### Shell
```bash
make
./mysh
```
### Networking
```bash
cd networking
make
./server 8080
./client <server_ip> 8080 <input_file> output.bin
```
### xv6
```bash
cd xv6/xv6-public
make qemu SCHEDULER=CFS
```
