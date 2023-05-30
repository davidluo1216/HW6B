/*
Name: Kenneth Chen, David Luo
CSE 374

mem_utils.c -> Implements utility functions to track memory stats, print the state of the heap, 
  and check the integrity of the heap.
*/

#include "mem_impl.h"
#include "mem.h"

// Global variable declarations, used across the memory management functions
uintptr_t total_size = 0;  // Total size of allocated memory
uintptr_t total_free = 0;  // Total size of currently free memory
uintptr_t n_free_blocks = 0;  // Number of free memory blocks

// Pointer to the start of the list of free memory blocks
block_header *free_list = NULL;

// Function declarations for freemem and getmem, which are defined in separate .c files
void freemem(void *p);
void *getmem(uintptr_t size);

// Function to return the current state of the memory manager
void get_mem_stats(uintptr_t *total_size_ptr, uintptr_t *total_free_ptr,
                   uintptr_t *n_free_blocks_ptr) {
  // Populate the values of the pointers with the global variable values
  *total_size_ptr = total_size;
  *total_free_ptr = total_free;
  *n_free_blocks_ptr = n_free_blocks;
}

// Function to print out the current state of the heap
void print_heap(FILE *f) {
  block_header *current = free_list;  // Start at the beginning of the free list
  // Loop through the free list, printing each block's address and size
  while (current != NULL) {
    fprintf(f, "0x%p, %lu\n", (void *)(current + 1), current->size);
    current = current->next;  // Move to the next block
  }
}

// Function to validate the integrity of the heap
void check_heap() {
  block_header *current = free_list;  // Start at the beginning of the free list
  uintptr_t total_free_check = 0;
  uintptr_t n_free_blocks_check = 0;
  // Loop through the free list, checking each block's size and summing the total
  while (current != NULL) {
    // Ensure the block size is positive
    if (!(current->size > 0)) {
      fprintf(stderr, "Block size is not positive!\n");
      return;
    }

    // Add the block's size to the total and increment the count of free blocks
    total_free_check += current->size;
    n_free_blocks_check++;
    current = current->next;  // Move to the next block
  }

  // Ensure that the calculated total free size matches the global variable
  if (!(total_free == total_free_check)) {
    fprintf(stderr, "total_free does not match total_free_check!\n");
  }

  // Ensure that the calculated count of free blocks matches the global variable
  if (!(n_free_blocks == n_free_blocks_check)) {
    fprintf(stderr, "n_free_blocks does not match n_free_blocks_check!\n");
  }
}
