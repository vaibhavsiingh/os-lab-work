
# OS Lab — Vaibhav Singh

This repository contains my work for **CS219 (Operating Systems Theory)** and **CS236 (Operating Systems Lab)** at **IIT Bombay**, under the guidance of **Prof. Mythili Vutukuru**.

Course website: [https://www.cse.iitb.ac.in/~mythili/os/](https://www.cse.iitb.ac.in/~mythili/os/)

----------

## 📁 Repository Structure
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
> Each subfolder contains its own small `README.md` with build and run instructions.

----------

## 📘 Folder Details

### [`ipc-code/`](./ipc-code/)

Contains programs demonstrating Inter-Process Communication (IPC) concepts:
-   **FIFOs (Named Pipes)** 
-   **UNIX Domain Sockets**    
-   **System V Message Queues**    
-   Combined examples using multiple IPC mechanisms.
 ----------

### [`pthreads-sync-code/`](./pthreads-sync-code/)

Pthread-based concurrency and synchronization exercises:
-   Mutex and condition variables
-   Producer-consumer problem
-   Readers-writers problem
-   Barrier synchronization
----------
### [`shell-code/`](./shell-code/)
Implementation of a **Linux shell from scratch**.  
Includes parsing, process creation, pipelines, redirection, and built-in commands.

----------

### `xv6-*` Folders
Contain modifications to the **xv6 operating system** (MIT version) for deeper OS concepts:
-   [**`xv6-cow/`**](./xv6-cow/) — Copy-on-Write implementation
-   [**`xv6-mmap/`**](./xv6-mmap/) — ``mmap`` implementation in xv6
-    [**`xv6-threads/`**](./xv6-threads/) — Kernel/user thread management
    
