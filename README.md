

# OS Lab — Vaibhav Singh

This repository contains my work for **CS219 (Operating Systems Theory)** and **CS236 (Operating Systems Lab)** at **IIT Bombay**, under the guidance of **Prof. Mythili Vutukuru**.

Course website: [https://www.cse.iitb.ac.in/~mythili/os/](https://www.cse.iitb.ac.in/~mythili/os/)

----------

##  Repository Structure
```graphql
.
├─ ipc-code/               # Inter-Process Communication exercises (FIFOs, sockets, message queues, shared memory)
├─ pthreads-sync-code/     # Pthread and synchronization exercises
├─ shell-code/             # Linux shell implementation exercises
├─ xv6-cow/                # xv6: Copy-on-Write implementation
├─ xv6-mmap/               # xv6: mmap implementation and thread experiments
├─ xv6-threads/            # xv6: thread-related implementations
└─ README.md               # (this file)
```

> Several folders (`shell-code`, `pthreads-sync-code`, and `ipc-code`) have subdirectories for individual exercises.  
> Each such folder contains its own `README.md` with build and run instructions.

----------

##  Folder Details

### [`ipc-code/`](./ipc-code/)

Contains programs demonstrating Inter-Process Communication (IPC) concepts:
-   **FIFOs (Named Pipes)** 
-   **UNIX Domain Sockets**    
-   **System V Message Queues**    
-   Combined examples using multiple IPC mechanisms.
 ----------

### [`pthreads-sync-code/`](./pthreads-sync-code/)

Pthread-based concurrency and synchronization exercises:
-   Master-worker threading model
-   Readers-writers problem
-   Custom semaphore implementation
-   Exercises on Mutex and condition variables
----------
### [`shell-code/`](./shell-code/)
Implementation of a **Linux shell from scratch**.  
Includes parsing, process creation, pipelines, redirection, and built-in commands.

----------

### `xv6-*` Folders
Contain modifications to the **xv6 operating system** (MIT version) for deeper OS concepts:
- **xv6-cow:** Implements **Copy-on-Write fork**, reducing memory duplication by sharing pages between parent and child until modification.  
- **xv6-mmap:** Adds **mmap()**  system calls for memory-mapped file I/O, allowing user programs to map files directly into memory space.  
- **xv6-threads:** > The `xv6-threads` folder includes implementations for **condition variables**, locks, semaphores, and sleep–wake mechanisms, followed by complete **thread management** along with `waitpid` and **barrier synchronization**.

##  Build & Run ``xv6-*``
Each project builds on the standard xv6 environment.
> Other folders include their respective build and run instructions within their own directories.
```bash
make clean && make && make qemu
```
Run within the corresponding folder (e.g., `xv6-cow/`) to boot xv6 in QEMU and test the implemented system calls and features.

## To Test ``xv6-*``
1. ``xv6-cow``: Corresponding file: ``myTest.c``
```xv6
myTest
```
2. ``xv6-mmap``: Corresponding file: ``test-mmap.c``
```xv6
test-mmap
```
3. ``xv6-threads``: Corresponding test files and commands:
```xv6
t_barrier
t_l_cv1
t_l_cv2
t_lock
t_sem1
t_sem2
t_sleepwake
t_threads
t_waitpid
```
---
