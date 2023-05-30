/*
Name: Kenneth Chen, David Luo
CSE 374

getmem.c ->  Contains the getmem function which allocates memory blocks of a certain size, 
  maintaining a free list for efficient allocation.

*/

#include "mem.h"
#include "mem_impl.h"
#include <stdint.h>

// Function to allocate memory, adding to the total size and removing from the free list
void *getmem(uintptr_t size) {
  // Round up the requested size to the nearest multiple of 16.
  if (size % 16 != 0) {
    size = ((size / 16) + 1) * 16;
  }

  // If the requested size is not positive, return NULL
  if (size <= 0)
    return NULL;

  // Initialize pointers to traverse the free list
  block_header *previous = NULL;
  block_header *current = free_list;

  // Loop through the free list until we find a block that's large enough
  while (current != NULL && current->size < size) {
    previous = current;
    current = current->next;
  }

  // If we found a suitable block
  if (current != NULL) {
    // Remove the block from the free list
    if (previous == NULL)
      free_list = current->next;
    else
      previous->next = current->next;

    // Update the global variables
    total_free -= current->size;
    n_free_blocks--;

    check_heap();  // Verify heap integrity after a block is removed from the free list.

    // Return the memory block, offset by the size of the header
    return (void *)(current + 1);
  } else {
    // If no suitable block was found in the free list, allocate a new block
    block_header *new_block =
        (block_header *)malloc(size + sizeof(block_header));
    if (new_block == NULL)
      return NULL;

    // Initialize the block and update the total size
    new_block->size = size;
    total_size += size;
    
    check_heap();  // Verify heap integrity after a new block is allocated.

    // Return the memory block, offset by the size of the header
    return (void *)(new_block + 1);
  }
}