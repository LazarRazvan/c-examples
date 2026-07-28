# c-examples

Useful C tricks and examples based on my experience, mostly around low-level and systems programming.

Each subfolder is self-contained. Where a `Makefile` is present, build with `make` inside that folder; otherwise compile the `.c` file directly with `gcc`. Some folders have their own `README.md` with more detail and run instructions.

## Contents

- [Tricks](#tricks)
- [Pipes](#pipes)
- [Ring buffer](#ring-buffer)
- [Bounded buffer](#bounded-buffer)

## Tricks

This folder contains various tricks allowed in C that can prove to be very useful. They are based on my work as a Linux kernel developer and can be found in different ways in the kernel sources. These tricks exist because memory optimizations and other low-level mechanisms are sometimes required.

| File | Description |
| --- | --- |
| `const` | How to use the `const` identifier on a pointer and what it means |
| `duff` | Manual loop unrolling technique |
| `funcarray` | Initialize and use an array of functions |
| `arrayinit` | Initialize slices of an array at declaration time |
| `jumplabels` | Store labels inside an array, or jump from an inner to an outer function |
| `pointers` | Use macros to initialize and get values from structures |
| `zeroarray` | Add extra buffer to the end of a structure and reduce its size |
| `func_generator` | Dynamically create functions with macros |
| `endianess` | Detect endianness type for a platform |
| `giant` | Expose the difference between resident and virtual memory |
| `null` | Hack null pointer exception (read/write on null) |

## Pipes

Understand the use case of pipes: anonymous pipes and named pipes.

| Folder | Description |
| --- | --- |
| `anonymous` | Pipes used to communicate between related processes |
| `named` | Pipes that can be used both for related and independent processes |

## Ring buffer

Implement a multi-threading and multi-process ring buffer.

| Folder | Description |
| --- | --- |
| `ring_buffer_threads` | Ring buffer with support for multiple threads (readers/writers) |
| `ring_buffer_process` | Ring buffer with support for multiple processes (readers/writers), backed by shared memory and a named semaphore *(work in progress)* |

## Bounded buffer

The *bounded buffer* problem is a classic concurrency problem where multiple producers and consumers share a fixed-size buffer, requiring synchronization to prevent overflow and underflow. It is typically implemented using a mutex to protect shared state and condition variables (or semaphores) to block threads when the buffer is full or empty.

The *weighted bounded buffer* extends this model by associating a variable "weight" with each item, so producers must wait not only for free slots but also for sufficient remaining capacity. This requires tracking both the number of items and the total weight, and using condition variables to wake threads when either constraint changes.
