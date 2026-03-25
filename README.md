# Custom Heap Memory Manager

> A robust C implementation simulating dynamic memory allocation, featuring custom `allocate` and `free` functions over a fixed memory pool.

## 📖 Description

This project demonstrates the inner workings of heap management by simulating a 10 KB memory pool. It replaces standard library functions like `malloc` and `free` with a custom implementation built from scratch. By using a linked list of metadata blocks, the program efficiently tracks allocated and available memory, ensuring proper memory alignment and preventing common memory mismanagement errors.

It serves as an excellent educational tool for understanding low-level memory operations, pointer arithmetic, and data structure manipulation in C.

## ✨ Features

* **First-Fit Allocation:** Iterates through the memory pool to find the first available block that satisfies the requested size.
* **Block Splitting:** Automatically divides larger free blocks into a requested allocation and a new, smaller free block to minimize internal fragmentation.
* **Automatic Coalescing:** Merges adjacent free memory blocks upon deallocation to combat external fragmentation and optimize available space.
* **Memory Alignment:** Aligns all memory requests to the nearest multiple of 8 bytes to ensure optimal hardware memory access.
* **Safety Measures:** Includes built-in edge-case protections against integer overflows, out-of-bounds pointer frees, and double-free vulnerabilities.
* **Heap Status Debugging:** Features a visualization function (`print_heap_status`) to track the exact state, addresses, and sizes of all memory blocks in real-time.

## 🛠️ Tech Stack

* **Language:** C
* **Core Concepts:** Pointer arithmetic, linked lists, memory alignment, and dynamic memory simulation.

## 🚀 Getting Started

### Prerequisites

You will need a standard C compiler (such as GCC) installed on your system.

### Installation & Execution

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/C0NFR1NGO/heap-management.git](https://github.com/C0NFR1NGO/heap-management.git)
