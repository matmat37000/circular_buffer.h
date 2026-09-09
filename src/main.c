#include <stdio.h>

#define LIB_ROTATING_BUFFER_IMPLEMENTATION
#include "rotating_buffer.h"


/// Quick demo of the buffer
int main() {

  // HEAP - Own Storage
  rotating_buffer buf_heap = rotating_buffer_create(4); // Use malloc

  // STACK - Doesn't own storage
  char storage[5]; // Buffer of 4 elements + 1 extra byte
  rotating_buffer buf_stack;
  rotating_buffer_init(&buf_stack, storage, 4);

  return 0;

  for (int i = 0; i < buf_heap.capacity; i++)
    printf("%d (%p)\n", buf_heap.storage[i], &buf_heap.storage[i]);

  for (int i = 0; i <= 7; i++)
    rotating_buffer_add(&buf_heap, i);

  printf("--------\n");
  
  for (int i = 0; i < buf_heap.capacity; i++) {
    if (&buf_heap.storage[i] == buf_heap.start_ptr) printf("\033[32m");
    else if (&buf_heap.storage[i] == buf_heap.end_ptr) printf("\033[31m");

    printf("%d (%p)\033[0m\n", buf_heap.storage[i], &buf_heap.storage[i]);
  }

  printf("--------\n");

  for (int i = 0; i < buf_heap.size; i++) {
    char* val = rotating_buffer_get(&buf_heap, i);
    printf("%d (%p)\n", *val, val);
  }
}
