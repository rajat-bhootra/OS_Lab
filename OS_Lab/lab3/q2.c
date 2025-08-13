#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
 * Configuration:
 * - Total memory size (must be a power of two)
 * - Minimum block size (must be a power of two)
 * - MAX_ORDER is defined as:
 *     MAX_ORDER = log2(TOTAL_MEMORY_SIZE) - log2(MIN_BLOCK_SIZE)
 *
 * For example, with MEMORY_SIZE = 1024 and MIN_BLOCK_SIZE = 32,
 * we have: log2(1024)=10 and log2(32)=5, so MAX_ORDER = 5.
 */
#define MEMORY_SIZE   1024         // Total memory managed
#define MIN_BLOCK_SIZE 32          // Smallest block size
#define MAX_ORDER      5           // Orders: 0 .. 5

// A block header is stored at the beginning of each block.
typedef struct Block {
    int order;             // Order of the block: block size = MIN_BLOCK_SIZE << order.
    int free;              // 1 if free, 0 if allocated.
    struct Block *next;    // Pointer to the next block in the free list.
} Block;

// Global memory region (for simulation).
static char memory[MEMORY_SIZE];

// Array of free lists: free_lists[i] points to a linked list of free blocks of size MIN_BLOCK_SIZE << i.
static Block *free_lists[MAX_ORDER + 1];

/*
 * Initialize the buddy allocator.
 * The entire memory region is initially one free block of order MAX_ORDER.
 */
void buddy_init() {
    for (int i = 0; i <= MAX_ORDER; i++)
        free_lists[i] = NULL;

    Block *initial = (Block*)memory;
    initial->order = MAX_ORDER;
    initial->free = 1;
    initial->next = NULL;
    free_lists[MAX_ORDER] = initial;
}

/*
 * Helper function: print the free lists.
 * This is useful for debugging and visualizing the state of the allocator.
 */
void print_free_lists() {
    printf("Free lists:\n");
    for (int i = 0; i <= MAX_ORDER; i++) {
        printf("Order %d (block size %d): ", i, MIN_BLOCK_SIZE << i);
        Block *cur = free_lists[i];
        while (cur) {
            uintptr_t offset = (uintptr_t)((char*)cur - memory);
            printf("[%p (offset %lu)] -> ", (void*)cur, (unsigned long)offset);
            cur = cur->next;
        }
        printf("NULL\n");
    }
}

/*
 * Allocate memory using the buddy system.
 *
 * The caller requests a certain number of bytes. We add the size of our header,
 * then round up to the next power-of-two that is at least MIN_BLOCK_SIZE.
 *
 * If a free block of the appropriate order is not available, we look for a block
 * of a higher order and split it repeatedly until a block of the desired order is obtained.
 *
 * On success, we mark the chosen block as allocated and return a pointer to the memory
 * immediately after the header.
 */
void *buddy_alloc(size_t size) {
    // Total required size includes our header.
    size_t total_size = size + sizeof(Block);
    // Determine the smallest block size (power-of-two) that fits total_size.
    int order = 0;
    size_t block_size = MIN_BLOCK_SIZE;
    while (block_size < total_size && order <= MAX_ORDER) {
        order++;
        block_size = MIN_BLOCK_SIZE << order;
    }
    if (order > MAX_ORDER)
        return NULL;  // Request is too large.

    // Search for a free block of order >= 'order'.
    int current_order = order;
    while (current_order <= MAX_ORDER && free_lists[current_order] == NULL)
        current_order++;

    if (current_order > MAX_ORDER)
        return NULL;  // No block available.

    // Remove the block from the free list.
    Block *block = free_lists[current_order];
    free_lists[current_order] = block->next;
    block->free = 0;

    // Split the block repeatedly until reaching the desired order.
    while (current_order > order) {
        current_order--;
        size_t split_size = MIN_BLOCK_SIZE << current_order;
        // The buddy block starts at an offset equal to the split size from the beginning.
        Block *buddy = (Block*)((char*)block + split_size);
        buddy->order = current_order;
        buddy->free = 1;
        // Insert the buddy into the free list for this order.
        buddy->next = free_lists[current_order];
        free_lists[current_order] = buddy;
        // Update the current block's order.
        block->order = current_order;
    }
    // Return pointer to the memory after the header.
    return (void*)(block + 1);
}

/*
 * Helper function to remove a given block from the free list at a specific order.
 */
static void remove_from_free_list(Block *target, int order) {
    Block **cur = &free_lists[order];
    while (*cur && *cur != target)
        cur = &((*cur)->next);
    if (*cur == target)
        *cur = target->next;
}

/*
 * Free memory previously allocated by buddy_alloc.
 *
 * The function computes the buddy of the freed block (using XOR on the offset)
 * and, if the buddy is free and of the same order, merges (coalesces) them.
 * This process is repeated recursively up to the maximum order.
 */
void buddy_free(void *ptr) {
    if (ptr == NULL)
        return;
    // Get pointer to the block header.
    Block *block = ((Block*)ptr) - 1;
    int order = block->order;
    uintptr_t offset = (uintptr_t)((char*)block - memory);
    block->free = 1;

    // Try to merge with buddy repeatedly.
    while (order < MAX_ORDER) {
        size_t size = MIN_BLOCK_SIZE << order;
        // Compute buddy's offset using XOR.
        uintptr_t buddy_offset = offset ^ size;
        Block *buddy = (Block*)(memory + buddy_offset);
        // Check if the buddy is free and of the same order.
        if (!(buddy->free && buddy->order == order))
            break;
        // Remove the buddy from the free list.
        remove_from_free_list(buddy, order);
        // Decide new base address: choose the lower address of the two buddies.
        if (buddy < block) {
            block = buddy;
            offset = buddy_offset;
        }
        order++;
        block->order = order;
        block->free = 1;
    }
    // Insert the (possibly merged) block into the free list.
    block->next = free_lists[order];
    free_lists[order] = block;
}

// Simple test routine to exercise the buddy allocator.
int main() {
    buddy_init();
    print_free_lists();

    printf("\nAllocating 20 bytes...\n");
    void *p = buddy_alloc(20);
    print_free_lists();

    printf("\nAllocating 50 bytes...\n");
    void *q = buddy_alloc(50);
    print_free_lists();

    printf("\nFreeing first block...\n");
    buddy_free(p);
    print_free_lists();

    printf("\nFreeing second block...\n");
    buddy_free(q);
    print_free_lists();

    return 0;
}

