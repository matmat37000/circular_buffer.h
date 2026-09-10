#include <stdio.h>
#include <unistd.h>

#define LIB_MATHIOL_CIRCULAR_BUFFER_USE_POWER_OF_TWO_BUFFERS
#define LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION
#include "circular_buffer.h"

static void print_status(const circular_buffer *buf_stack, int storage[]);

/// Quick demo of the buffer
int main() {
  // STACK - Doesn't own storage
  int storage[4]; // Buffer of 4 elements
  circular_buffer buf_stack;
  circular_buffer_init(&buf_stack, storage, 4, sizeof(int));

  for (int i = 0; i < 21; i++) {
      printf("\033[H\033[J");
      circular_buffer_add(&buf_stack, i);
      print_status(&buf_stack, storage);
      sleep(1);
  }
}

void print_status(const circular_buffer *buf_stack, int storage[]) {
    printf("Start: %lu\nEnd: %lu\n", buf_stack->start, buf_stack->end);

    printf("--------\n");

    for (int i = 0; i < buf_stack->capacity; i++) {
        // if (i == buf_stack->start) printf("\033[32m");
        if (i == buf_stack->end) printf("\033[31m");

        printf("%d (%p)\033[0m\n", storage[i], &storage[i]);
    }

    printf("--------\n");

    for (int i = 0; i < buf_stack->size; i++) {
        int* val = circular_buffer_get(buf_stack, i);
        printf("%d (%p)\n", *val, val);
    }
}
