#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// Block header structure representing each memory block.
typedef struct Block {
    size_t size;         // Size of the block (payload size, not including header)
    int free;            // Flag: 1 if free, 0 if allocated
    struct Block *next;  // Next block in the free list
    struct Block *prev;  // Previous block in the free list
} Block;

#define BLOCK_SIZE sizeof(Block)
#define MIN_BLOCK_SIZE 32  // Minimum remaining size to allow a split

// Global pointer to the free list (doubly-linked list)
Block *free_list = NULL;

/*
 * Splits the given block if it is large enough to be split into two:
 * one block of exactly requested_size and a second free block containing the rest.
 */
void split_block(Block *block, size_t requested_size) {
    // Check if splitting is worthwhile (enough space for a new header and minimum payload)
    if (block->size >= requested_size + BLOCK_SIZE + MIN_BLOCK_SIZE) {
        // Calculate the address for the new block header (after the allocated part)
        Block *new_block = (Block *)((char *)block + BLOCK_SIZE + requested_size);
        new_block->size = block->size - requested_size - BLOCK_SIZE;
        new_block->free = 1;
        new_block->next = block->next;
        new_block->prev = block;
        
        // Update next pointer of block and link in free list
        if (new_block->next)
            new_block->next->prev = new_block;
        block->next = new_block;
        
        // Update size of the current block to the requested size
        block->size = requested_size;
    }
}

/*
 * Coalesces (merges) a free block with its adjacent free neighbors.
 * Returns a pointer to the coalesced block.
 */
Block *coalesce(Block *block) {
    // Coalesce with the next block if it's free and contiguous.
    if (block->next && block->next->free) {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        if (block->next)
            block->next->prev = block;
    }
    
    // Coalesce with the previous block if it's free.
    if (block->prev && block->prev->free) {
        block->prev->size += BLOCK_SIZE + block->size;
        block->prev->next = block->next;
        if (block->next)
            block->next->prev = block->prev;
        block = block->prev;
    }
    
    return block;
}

/*
 * Simulated memory allocation function.
 * Searches the free list for a free block large enough for the request.
 * If a block is larger than needed, it splits the block.
 */
void *my_malloc(size_t size) {
    Block *current = free_list;
    
    // Search free list for a free block with enough size.
    while (current) {
        if (current->free && current->size >= size) {
            // Optionally split the block if it's too large.
            split_block(current, size);
            current->free = 0; // Mark as allocated
            // Return a pointer just after the header.
            return (void *)(current + 1);
        }
        current = current->next;
    }
    
    // In a real allocator, we’d request more memory from the OS here.
    return NULL; // Allocation failed (for this simulation)
}

/*
 * Simulated memory free function.
 * Marks a block as free and then attempts to coalesce with adjacent free blocks.
 */
void my_free(void *ptr) {
    if (ptr == NULL)
        return;
    
    // Get the block header. (We assume the user pointer points right after the header.)
    Block *block = (Block *)ptr - 1;
    block->free = 1;
    
    // Coalesce with any adjacent free blocks.
    coalesce(block);
}

/*
 * Utility function to add a block to our free list.
 * For simulation, we assume a large chunk of memory is allocated from the OS.
 */
void add_to_free_list(void *mem, size_t size) {
    Block *block = (Block *)mem;
    block->size = size;
    block->free = 1;
    block->next = free_list;
    block->prev = NULL;
    if (free_list)
        free_list->prev = block;
    free_list = block;
}

int main() {
    // Simulation: allocate a large chunk of memory (say 1024 bytes)
    void *memory = malloc(1024);
    if (!memory) {
        fprintf(stderr, "Failed to allocate memory for simulation.\n");
        return 1;
    }
    
    // Initialize the free list with the entire chunk (minus header size)
    add_to_free_list(memory, 1024 - BLOCK_SIZE);
    
    // Allocate some memory blocks
    void *a = my_malloc(100);  // Allocate 100 bytes
    void *b = my_malloc(200);  // Allocate 200 bytes
    
    // Free the allocated blocks to trigger coalescing
    my_free(a);
    my_free(b);
    
    // Print the free list to show the effect of coalescing
    Block *current = free_list;
    while (current) {
        printf("Block at %p: size = %zu, free = %d\n", (void *)current, current->size, current->free);
        current = current->next;
    }
    
    free(memory);
    return 0;
}