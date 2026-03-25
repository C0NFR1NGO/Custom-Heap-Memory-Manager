#include <stdio.h>
#include <stddef.h>

// Define the size of our simulated heap memory pool
#define HEAP_SIZE 10240 // 10 KB
#define ALIGNMENT 8

// Macro to align size to the nearest multiple of 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

// Simulated memory pool
// Using a long long array guarantees the compiler aligns the memory pool to 8 bytes
long long memory_pool[HEAP_SIZE / sizeof(long long)];

// Metadata structure for each memory block
typedef struct Block {
    size_t size;           // Size of the usable memory (excluding metadata)
    int is_free;           // 1 if the block is free, 0 if allocated
    struct Block* next;    // Pointer to the next block in the heap
} Block;

// Pointer to the start of our memory pool
Block* head = NULL;
int is_initialized = 0;

// Initialize the heap with a single large free block
void initialize_heap() {
    head = (Block*)memory_pool;
    head->size = HEAP_SIZE - sizeof(Block);
    head->is_free = 1;
    head->next = NULL;
    is_initialized = 1;
}

// Helper function: Splits a larger free block into two smaller blocks
void split_block(Block* fitting_slot, size_t size) {
    // Calculate the address for the new free block's metadata
    Block* new_block = (Block*)((char*)fitting_slot + sizeof(Block) + size);
    
    // Set up the new free block
    new_block->size = fitting_slot->size - size - sizeof(Block);
    new_block->is_free = 1;
    new_block->next = fitting_slot->next;

    // Update the allocated block
    fitting_slot->size = size;
    fitting_slot->is_free = 0;
    fitting_slot->next = new_block;
}

// Allocate memory (similar to malloc)
void* allocate(size_t size) {
    if (size == 0) return NULL;
    if (!is_initialized) initialize_heap();

    // Edge Case: Prevent integer overflow and immediately reject overly large requests
    if (size > HEAP_SIZE - sizeof(Block)) {
        printf("Memory Allocation Failed: Requested size too large\n");
        return NULL;
    }

    // Align requested size to maintain memory alignment
    size = ALIGN(size);

    Block* current = head;

    // Traverse the linked list to find a suitable free block (First-Fit)
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            
            // If the block is large enough to be split into an allocation + a new free block
            if (current->size > size + sizeof(Block)) {
                split_block(current, size);
            } else {
                // Block is an exact fit (or close enough that splitting isn't possible)
                current->is_free = 0;
            }
            
            // Return a pointer to the usable memory (skip the metadata header)
            return (void*)(current + 1); 
        }
        current = current->next;
    }
    
    // Not enough contiguous memory found
    printf("Memory Allocation Failed: Out of Memory\n");
    return NULL; 
}

// Helper function: Merges adjacent free blocks to prevent fragmentation
void merge_free_blocks() {
    Block* current = head;
    
    while (current != NULL && current->next != NULL) {
        if (current->is_free && current->next->is_free) {
            // Combine current block with the next block
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
            // Note: We don't advance 'current' here, because the NEW next block 
            // might ALSO be free, and we want to merge that in the next iteration.
        } else {
            // Move to the next block
            current = current->next;
        }
    }
}

// Free memory (similar to free)
void free_mem(void* ptr) {
    if (ptr == NULL) return;

    // Edge Case: Validate that the pointer is within the bounds of our memory pool
    if ((char*)ptr < (char*)memory_pool || (char*)ptr >= (char*)memory_pool + HEAP_SIZE) {
        printf("Error: Attempt to free an out-of-bounds pointer %p!\n", ptr);
        return;
    }

    // Get the metadata block by moving backwards from the pointer
    Block* block_to_free = (Block*)ptr - 1;
    
    // Edge Case: Prevent Double Frees
    if (block_to_free->is_free) {
        printf("Error: Double free detected on pointer %p!\n", ptr);
        return;
    }

    // Mark the block as free
    block_to_free->is_free = 1;

    // Immediately trigger a merge to coalesce adjacent free blocks
    merge_free_blocks();
}

// Helper function: Prints the current layout of the heap for debugging
void print_heap_status() {
    Block* current = head;
    int block_num = 1;
    printf("\n--- Current Heap Status ---\n");
    while (current != NULL) {
        printf("Block %d: Address = %p | Size = %5zu bytes | Status = %s\n", 
               block_num++, 
               (void*)current, 
               current->size, 
               current->is_free ? "FREE" : "ALLOCATED");
        current = current->next;
    }
    printf("---------------------------\n");
}

int main(void) {
    printf("Initializing Heap...\n");
    initialize_heap();
    print_heap_status();

    printf("\nAllocating 'A' (1000 bytes) and 'B' (2000 bytes)...");
    void* a = allocate(1000);
    void* b = allocate(2000);
    print_heap_status();

    printf("\nAllocating 'C' (3000 bytes)...");
    void* c = allocate(3000);
    print_heap_status();

    printf("\nFreeing 'A' (creates a free block at the top)...");
    free_mem(a);
    print_heap_status();

    printf("\nFreeing 'C' (merges with the remaining free space at the bottom)...");
    free_mem(c);
    print_heap_status();

    printf("\nFreeing 'B' (merges everything into a single free block)...");
    free_mem(b);
    print_heap_status();


}
