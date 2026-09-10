#include <stdio.h>

#define LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION
#include "circular_buffer.h"


/// Quick demo of the buffer
int main() {

  // HEAP - Own Storage
  circular_buffer buf_heap = circular_buffer_create(4); // Use malloc

  // STACK - Doesn't own storage
  int storage[4]; // Buffer of 4 elements
  circular_buffer buf_stack;
  circular_buffer_init(&buf_stack, (char[]) storage, 4);

  return 0;

  for (int i = 0; i < buf_heap.capacity; i++)
    printf("%d (%p)\n", buf_heap.storage[i], &buf_heap.storage[i]);

  for (int i = 0; i <= 7; i++)
    circular_buffer_add_impl(&buf_heap, i);

  printf("--------\n");
  
  for (int i = 0; i < buf_heap.capacity; i++) {
    if (&buf_heap.storage[i] == buf_heap.start) printf("\033[32m");
    else if (&buf_heap.storage[i] == buf_heap.end) printf("\033[31m");

    printf("%d (%p)\033[0m\n", buf_heap.storage[i], &buf_heap.storage[i]);
  }

  printf("--------\n");

  for (int i = 0; i < buf_heap.size; i++) {
    char* val = circular_buffer_get(&buf_heap, i);
    printf("%d (%p)\n", *val, val);
  }
}
