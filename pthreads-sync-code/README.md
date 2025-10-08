# pthread-sync-code

This folder contains implementations of various **POSIX Threads (pthreads)** synchronization concepts. Each subfolder demonstrates a specific concurrency mechanism or pattern using minimal C programs — from basic warmup exercises to synchronization primitives like reader–writer locks and custom semaphores.

---

##  Directory Structure
```graphql
pthread-sync-code/  
├─ warmup/        # Basic thread creation and synchronization exercises  
├─ master-worker/ # Master–worker threading model implementation  
├─ rwlock/        # Reader–writer lock usage and behavior  
└─ zemaphore/     # Custom semaphore built using mutexes and condition variables
```

---

##  Highlights
- **Warmup:** Simple exercises to practice creating, joining, and synchronizing threads.  
- **Master–Worker:** Demonstrates task delegation and coordination among multiple worker threads.  
- **Reader–Writer Lock:** Allows multiple readers or one writer to access a shared resource safely.  
- **Zemaphore:** A self-implemented semaphore using pthread mutexes and condition variables, without relying on `sem_t`.

##  Build & Run
```bash
gcc -pthread filename.c -o filename
./filename
```
For programs involving multiple threads, output order may vary between runs due to concurrent execution.
