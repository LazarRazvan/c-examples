# c-examples

Useful tricks and examples for C language based on my experience.

## Tricks

This folder contains different tricks allowed in C that can prove to be very useful. This are based on my work as a Linux kernel developer and can be found in different ways in the kernel sources. This tricks are made because memory optimizations and other mechanisms are required.

### What can you find here?

```
- const		: How to use const identifier on pointer and what does it mean
- duff		: Manual loop unrolling technique
- funcarray	: Initialize and use an array of functions
- arrayinit	: Initialize slices of an array at declaration time
- jumplabels	: Store all the labels inside an array or jump from inner to outer function
- pointers	: Use macros to initialize and get values from structures
- zeroarray	: Add extra buffer to the end of the structure and reduce its size
- func_generator: Example of dynamically create functions
- endianess	: Detect endianess type for a platform
- giant		: Expose the differece between resident and virtual memory
- null		: Hack null pointer exception (read/write on null)
```

## Pipes

Understand the use case of pipes. Anonymous pipes and named pipes

### What can you find here?

```
- anonymous	: Pipes used to communicate between related processes
- named		: This kind of pipes can be used both for related processes and independent ones.
```

## Ring buffer

Implement a multi-threading and multi-processes ring buffer

### What can you find here?

```
- ring_buffer_thread	: Ring buffer with support for multiple threads (readers/writers)
```
