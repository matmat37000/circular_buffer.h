/****************************************************************************
 *  rotating_buffer.h
 *  BORDIER-AUPY Mathieu
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at 
 *  your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

/* DECLARATION */
#ifndef MATHIOL_ROTATING_BUFFER_H_
#define MATHIOL_ROTATING_BUFFER_H_

/** Rotating buffer struct */
typedef struct {
  size_t size;
  size_t capacity;

  char* start_ptr;
  char* end_ptr;
  char* storage;

  bool owns_storage;
} rotating_buffer;

/**
 * Initialise a rotating buffer using caller-provided storage
 *
 * @param buf     Buffer object to initialise
 * @param storage Storage area. Must contain at least size + 1 bytes
 * @param size    Usable buffer capacity, excluding the extra byte
 */
void rotating_buffer_init(rotating_buffer *buf, char *storage, size_t size);

/** 
 * Create a buffer object with malloc, and use rotating_buffer_init
 *
 * The allocated storage is of size SIZE + 1
 *
 * @param size Size of the buffer
 */
rotating_buffer rotating_buffer_create(size_t size);

/**
 * Free storage owned by the buffer.
 * Does not free buf itself.
 *
 * @param buf Address of the buffer to free the content
 */
void rotating_buffer_free(rotating_buffer *buf);

/**
 * Add an element to the buffer
 *
 * @param buf Address of the buffer to modify
 * @param element the element to add
 */
void rotating_buffer_add(rotating_buffer *buf, char element);

/**
 * Get an element of the buffer
 *
 * @param buf Address of the buffer to get the element of
 * @param pos The index of the element
 */
char* rotating_buffer_get(const rotating_buffer *buf, int pos);

#endif // MATHIOL_ROTATING_BUFFER_H_

/* IMPLEMENTATION */
#if defined(LIB_ROTATING_BUFFER_IMPLEMENTATION) || defined(__CLION_IDE__) || defined(__INTELLISENSE__)

#include <stdlib.h>

inline void rotating_buffer_init(rotating_buffer *buf, char *storage, const size_t size) {
  buf->size = 0;
  buf->capacity = size + 1;
  buf->storage = storage;
  buf->start_ptr = buf->end_ptr = buf->storage;
  buf->owns_storage = false;
}

inline rotating_buffer rotating_buffer_create(const size_t size) {
  rotating_buffer buf;
  char *storage = malloc(sizeof (char) * (size + 1));

  if (storage == NULL) {
    buf.size = 0;
    buf.capacity = 0;
    buf.start_ptr = NULL;
    buf.end_ptr = NULL;
    buf.storage = NULL;
    buf.owns_storage = true;
    return buf;
  }

  rotating_buffer_init(&buf, storage, size);
  buf.owns_storage = true;
  return buf;
}

inline void rotating_buffer_free(rotating_buffer *buf) {
  if (buf->owns_storage)
    free(buf->storage);

  buf->capacity = buf->size = 0;
  buf->storage = buf->start_ptr = buf->end_ptr = NULL;
  buf->owns_storage = false;
}

inline void rotating_buffer_add(rotating_buffer *buf, const char element) {
  // Move by one the end_ptr
  buf->end_ptr++;
  // Clamp it
  if (buf->end_ptr > buf->storage + (buf->capacity - 1)) {
    buf->end_ptr = buf->storage;
  }
  // Set the value
  *buf->end_ptr = element;
 
  // Start moving the start only if buffer full
  if (buf->size == buf->capacity - 1) {
    // Move by one the start_ptr
    buf->start_ptr++;
    // Clamp it
    if (buf->start_ptr > buf->storage + buf->size) {
      buf->start_ptr = buf->storage;
    }
  }
  else {
    buf->size++;
  }
}

inline char* rotating_buffer_get(const rotating_buffer *buf, const int pos) {
  char *ptr = buf->start_ptr + pos;
  const char *max = buf->storage + (buf->capacity - 1);
  // Clamp the pointer back to the start
  // with the difference of passed cell from the max
  if (ptr > max) {
    ptr = buf->storage + (ptr - max - 1); // Minus one for the buffer cell
  }

  return ptr;
}

#endif // LIB_ROTATING_BUFFER_IMPLEMENTATION