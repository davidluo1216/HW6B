/*
Name: Kenneth Chen, David Luo
CSE 374

freemem.c -> Implements the freemem function that frees a block of memory and adds it back to the free list,
  merging with adjacent blocks if possible.
*/

#include "mem.h"
#include "mem_impl.h"

// Function to free a memory block, adding it to the free list
void freemem(void *p) {
  // If the pointer is NULL, do nothing
  if (p == NULL)
    return;

  // Convert the pointer to a block_header pointer
  block_header *block = ((block_header *)p) - 1;
  block->next = NULL;

  // Update the global variables
  total_free += block->size;
  n_free_blocks++;

  // If the free list is empty, add the block to the list
  if (free_list == NULL) {
    free_list = block;
  } else {
    // If the free list is not empty, traverse the list to find the correct spot
    block_header *current = free_list;
    block_header *previous = NULL;
    while (current != NULL && current < block) {
      previous = current;
      current = current->next;
    }

    // Insert the block into the correct spot in the list
    if (previous == NULL) {
      block->next = free_list;
      free_list = block;
    } else {
      previous->next = block;
      block->next = current;
    }

    // Merge with next block if they are adjacent.
    if (block->next != NULL &&
        (uintptr_t)((char *)block + sizeof(block_header) + block->size) == (uintptr_t)block->next) {
      block->size += sizeof(block_header) + block->next->size;
      block->next = block->next->next;
      n_free_blocks--;
    }

    // Merge with previous block if they are adjacent.
    if (previous != NULL &&
        (uintptr_t)((char *)previous + sizeof(block_header) + previous->size) == (uintptr_t)block) {
      previous->size += sizeof(block_header) + block->size;
      previous->next = block->next;
      n_free_blocks--;
    }
  }

  check_heap();  // Verify heap integrity after a block is added to the free list.
}