# Dynamic Memory Allocation using First-Fit Technique

## Overview
This project implements a basic dynamic memory allocation system using the **First-Fit** technique. It simulates memory management by manually handling memory allocation and deallocation within a statically allocated character array.

## Features
- Implements dynamic memory allocation using a **custom heap**.
- Uses **First-Fit** allocation strategy.
- Supports **splitting** large free blocks to optimize memory usage.
- **Merges** adjacent free blocks to reduce fragmentation.
- Tracks allocated and free memory using a **linked list of metadata blocks**.

## Data Structures

### `metadata` Struct
```c
typedef struct metadata_tag{
    int size;                  // Size of the allocated/free memory block
    bool available;            // Availability status (TRUE/FALSE)
    struct metadata_tag* next; // Pointer to the next memory block
} metadata;
```
This structure stores metadata for each allocated or free block.

## Functions

### `void initialize()`
Initializes the memory allocator by setting up the first free block in the `free_space` array.

### `metadata* split(metadata* curr, int size_mem_req)`
Splits a free memory block into two parts:
- One part for the requested allocation.
- Another part remains free with updated metadata.

### `void* allocate_memory(int req_size)`
Allocates a memory block of requested size using the **First-Fit** approach.
- If a block is **exactly** the requested size, it is allocated directly.
- If a block is **larger**, it is split into an allocated part and a remaining free block.
- If no suitable block is found, allocation **fails**.

### `void merge(metadata* curr)`
Merges adjacent free blocks to reduce external fragmentation.

### `void de_allocate(void* loc)`
Deallocates a memory block and marks it as free. Attempts to **merge** it with adjacent free blocks.

## Code Execution Flow
1. **Initialize** the memory manager.
2. **Allocate memory** in chunks (using `allocate_memory`).
3. **Deallocate memory** when no longer needed (`de_allocate`).
4. **Merge free blocks** to optimize memory usage.

## Sample Output
```
Metadata Size: 12
Heap Size : 40000
Start of free_space :   0x55d3fa402010
End of free_space :     0x55d3fa4a9e0f
memory allocated
0x55d3fa40201c
Available free space is 29988
memory allocated
0x55d3fa428210
Available free space is 19976
memory allocated
0x55d3fa448404
Deallocated
memory allocated
0x55d3fa40201c
```

## Compilation & Execution
To compile and run the program, use:
```bash
gcc memory_allocator.c -o memory_allocator
./memory_allocator
```

## Improvements & Enhancements
- Implement a **Best-Fit** or **Worst-Fit** allocation strategy.
- Maintain a **free list with a doubly linked list** to optimize merging.
- Add **debugging utilities** to visualize memory usage.

## License
This project is open-source and can be modified and distributed under the **MIT License**.


