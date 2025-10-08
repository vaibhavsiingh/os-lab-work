


# ipc-code

This folder contains implementations of various **Inter-Process Communication (IPC)** mechanisms. Each subfolder demonstrates a specific IPC concept using minimal C programs — from basic FIFOs to POSIX shared memory and message queues.

---
## 📂 Directory Structure
```
ipc-code/
├─ fifo/     # Named pipes (FIFOs): simple reader/writer
├─ msgq/     # System V message queue sender/receiver
├─ shm-msgq/ # Combined shared memory + message queue example
└─ sockets/  # UNIX domain sockets: datagram and file-transfer examples
```
## 🧩 Highlights
-  **FIFO:** Demonstrates unidirectional communication between two processes using named pipes.
-  **UNIX Sockets:** Includes both datagram and stream-based local socket communication.
-  **Shared Memory + Message Queue:** Shows synchronization between processes sharing memory.
-  **Message Queues:** Simple System V implementation using msgget, msgsnd, and msgrcv.
## ⚙️ Build & Run
```bash
gcc  reader.c  -o  reader
gcc  writer.c  -o  writer
./writer & ./reader
```
For socket or shared memory examples, open two terminals and run the corresponding server/client or producer/consumer programs.
