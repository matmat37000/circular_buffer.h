# circular_buffer.h

A single header C/C++ library

## How to use ?

To use this library you just `#include "circular_buffer.h"` in any file using it. 

You must also define `LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION` before one of the includes to include the implementation in that file.

**Exemple:**

```c
#define LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION
#include "circular_buffer.h"

int main() {
  // Library-owned storage
  circular_buffer *buf = create_circular_buffer(4, sizeof(int));
  
  // Caller-owned storage
  circular_buffer buf2;
  char storage[4];
  circular_buffer_init(&buf2, storage, 4, sizeof(char))
}
```

Only define `LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION` in one source file of your project.

Other source files only need:

```c
#include "circular_buffer.h"
```

### How to correctly manage your memory

The library provides two ways to create a circular buffer.

#### Dynamically allocated storage

The library provide `create_circular_buffer` to allocate the buffer in memory.

> [!NOTE]
> This library use `malloc` to allocated memory for the buffer when using `create_circular_buffer(capacity, element_size)`

To free the buffer (if created with `create_circular_buffer`), call `circular_buffer_free(buf)` on it.

```c
// Create the object
circular_buffer buf = create_circular_buffer(10, sizeof(float));
circular_buffer_free(buf);  // Free allocated memory by the buffer
```

> [!WARNING]
> `circular_buffer_free(buf)` is unsafe with [Caller-provided storage](#caller-provided-storage).

#### Caller-provided storage

You can also provide your own storage. This is useful when the buffer should live entirely on the stack or when you want to control the memory allocation yourself.

```c
double storage[10];
circular_buffer buf;

circular_buffer_init(&buf, storage, sizeof(storage), sizeof(double));
```

In this case, the library does not own the storage, so `circular_buffer_free()` will not attempt to free it.

`circular_buffer_free(&buf);`

### Optimisation

If all buffers used by your application have a power-of-two capacity, you can define:
```c
#define LIB_MATHIOL_CIRCULAR_BUFFER_USE_POWER_OF_TWO_BUFFERS
```

This enables bitwise operations for circular-buffer index wrapping instead of the modulo operator.

For example:

```c
#define LIB_MATHIOL_CIRCULAR_BUFFER_USE_POWER_OF_TWO_BUFFERS
#define LIB_MATHIOL_CIRCULAR_BUFFER_IMPLEMENTATION
#include "circular_buffer.h"
```

> [!IMPORTANT]
> With this option enabled, buffer capacities must be powers of two: 1, 2, 4, 8, 16, 32, 64, ...
> 
> Attempting to create a buffer with a non-power-of-two capacity will fail.
>
> **For caller-provided storage, `circular_buffer_init` will not fail, be careful.**

---
This project is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.html).
