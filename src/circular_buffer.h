/******************************************************************************

    circular_buffer.h
    Copyright (C) 2026  Mathieu Bordier-Aupy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

/*
 * You can define LIB_MATHIOL_CIRCULAR_BUFFER_USE_POWER_OF_TWO_BUFFERS
 * to allow optimisation if all buffers size are power of two.
 */

// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile

/* DECLARATION */
#ifndef LIB_MATHIOL_CIRCULAR_BUFFER_H_
#define LIB_MATHIOL_CIRCULAR_BUFFER_H_
#include <string.h>

#define rotating_buffer_add(buf, value) \
    rotating_buffer_add_impl((buf), &(value))

/** Rotating buffer struct */
typedef struct {
    unsigned char *storage;
    size_t size;
    size_t capacity;
    size_t element_size;
    size_t start;
    size_t end;
    bool owns_storage;
} circular_buffer;

/**
 * Initialise a circular buffer using caller-provided storage
 *
 * @param buf           Buffer object to initialise
 * @param storage       Storage area. Must contain at least size
 * @param capacity      Usable buffer capacity
 * @param element_size  Size of each element the buffer will hold
 */
void circular_buffer_init(circular_buffer *buf, unsigned char *storage, size_t capacity, size_t element_size);

/**
 * Create a buffer object with malloc, and use circular_buffer_init
 *
 * The allocated storage is of the given SIZE
 *
 * @param capacity          Size of the buffer
 * @param element_size     Size of each element the buffer will hold
 */
circular_buffer circular_buffer_create(size_t capacity, size_t element_size);

/**
 * Free storage owned by the buffer.
 * Does not free buf itself.
 *
 * @param buf Address of the buffer to free the content
 */
void circular_buffer_free(circular_buffer *buf);

/**
 * Add an element to the buffer
 *
 * @param buf       Address of the buffer to modify
 * @param element   The element to add
 */
int circular_buffer_add_impl(circular_buffer *buf, const unsigned char *element);

/**
 * Get an element of the buffer
 *
 * @param buf Address of the buffer to get the element of
 * @param pos The index of the element
 */
unsigned char *circular_buffer_get(const circular_buffer *buf, size_t pos);

#endif // LIB_MATHIOL_CIRCULAR_BUFFER_H_

/* IMPLEMENTATION */
#if defined(LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION) || defined(__CLION_IDE__) || defined(__INTELLISENSE__)

#include <stdlib.h>

void circular_buffer_init(circular_buffer *buf, unsigned char *storage, const size_t capacity, const size_t element_size) {
    buf->size = 0;
    buf->capacity = capacity;
    buf->element_size = element_size;
    buf->storage = storage;
    buf->start = buf->end = 0;
    buf->owns_storage = false;
}

circular_buffer circular_buffer_create(const size_t capacity, const size_t element_size) {
    circular_buffer buf = {0};

    if (capacity == 0)
        return buf;
#ifdef LIB_MATHIOL_CIRCULAR_BUFFER_USE_POWER_OF_TWO_BUFFERS
    if ((capacity & (capacity - 1)) != 0)
        return buf;
#endif

    unsigned char *storage = malloc(element_size * capacity);

    if (storage == NULL)
        return buf;

    circular_buffer_init(&buf, storage, capacity, element_size);
    buf.owns_storage = true;

    return buf;
}

void circular_buffer_free(circular_buffer *buf) {
    if (buf == NULL)
        return;

    if (buf->owns_storage)
        free(buf->storage);

    *buf = (circular_buffer){0};
}

int circular_buffer_add_impl(circular_buffer *buf, const unsigned char *element) {
    if (buf == NULL ||
        buf->storage == NULL ||
        buf->capacity == 0)
        return EXIT_FAILURE;

    // Set the value
    memcpy(buf->storage + buf->end * buf->element_size, element, buf->element_size);
    // Move by one the end
    buf->end++;
    // Clamp it
    if (buf->end == buf->capacity)
        buf->end = 0;


    // Start moving the start only if buffer full
    if (buf->size < buf->capacity) {
        buf->size++;
    } else {
        // Move by one the start
        buf->start++;
        // Clamp it
        if (buf->start == buf->capacity)
            buf->start = 0;
    }

    return EXIT_SUCCESS;
}

unsigned char *circular_buffer_get(
    const circular_buffer *buf,
    const size_t pos) {
    if (buf == NULL ||
        buf->storage == NULL ||
        buf->capacity == 0 ||
        pos >= buf->size)
        return NULL;

#ifdef LIB_MATHIOL_CIRCULAR_BUFFER_USE_POWER_OF_TWO_BUFFERS
    const size_t final_pos =
            (buf->start + pos) & (buf->capacity - 1);
#else
    const size_t final_pos =
            (buf->start + pos) % buf->capacity;
#endif

    return buf->storage + final_pos * buf->element_size;
}

#endif // LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION
