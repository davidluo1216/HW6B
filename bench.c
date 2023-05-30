#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void freemem(void *p);
void *getmem(uintptr_t size);

typedef struct block_header {
    uintptr_t size;
    struct block_header* next;
} block_header;

block_header* free_list = NULL;
uintptr_t total_size = 0;
uintptr_t total_free = 0;
uintptr_t n_free_blocks = 0;

void* getmem(uintptr_t size) {
    if(size <= 0)
        return NULL;
    
    block_header* previous = NULL;
    block_header* current = free_list;
    while(current != NULL && current->size < size) {
        previous = current;
        current = current->next;
    }

    if(current != NULL) {
        if(previous == NULL)
            free_list = current->next;
        else
            previous->next = current->next;

        total_free -= current->size;
        n_free_blocks--;
        return (void*)(current + 1);
    } else {
        block_header* new_block = (block_header*)malloc(size + sizeof(block_header));
        if(new_block == NULL)
            return NULL;
        
        new_block->size = size;
        total_size += size;
        return (void*)(new_block + 1);
    }
}

void freemem(void* p) {
    if(p == NULL)
        return;

    block_header* block = ((block_header*)p) - 1;
    block->next = free_list;
    free_list = block;
    total_free += block->size;
    n_free_blocks++;
}

void get_mem_stats(uintptr_t* p_total_size, uintptr_t* p_total_free, uintptr_t* p_n_free_blocks) {
    *p_total_size = total_size;
    *p_total_free = total_free;
    *p_n_free_blocks = n_free_blocks;
}

void print_heap(FILE * f) {
    block_header* current = free_list;
    while(current != NULL) {
        fprintf(f, "0x%p, %lu\n", (void*)(current + 1), current->size);
        current = current->next;
    }
}
