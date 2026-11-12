# DSAForge

DSAForge is a header-only C++ data structures and algorithms library. The initial release targets C++17.

## Current components

- Singly linked list
- Doubly linked list

## Requirements

- C++17 compiler (GCC 8+, Clang 7+, or a compatible MSVC)
- CMake 3.16+

## Build, test, and run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/examples/dsaforge_linked_list_example
```

## Use in code

```cpp
#include <dsaforge/dsaforge.hpp>

int main() {
    dsaforge::SinglyLinkedList<int> values;
    values.pushBack(10);
    values.pushBack(20);
    values.remove(10);

    dsaforge::DoublyLinkedList<int> other;
    other.pushFront(5);
    other.pushBack(15);
}
```

## API

### SinglyLinkedList<T>

- `pushBack(value)`
- `deleteHead()`
- `deleteTail()`
- `remove(value)` — removes the first matching value
- `find(value)` — returns a pointer to the stored value, or `nullptr`
- `count()`
- `isEmpty()`
- `clear()`

### DoublyLinkedList<T>

- `pushFront(value)`
- `pushBack(value)`
- `popFront()`
- `popBack()`
- `remove(value)` — removes the first matching value
- `find(value)` — returns a pointer to the stored value, or `nullptr`
- `count()`
- `isEmpty()`
- `clear()`

Deletion functions return `true` when an element was removed and `false` when no element could be removed. `remove` deletes only the first matching occurrence. Duplicate values are allowed.

## Install

```bash
cmake --install build --prefix "$HOME/.local"
```

A consuming CMake project can then use:

```cmake
find_package(DSAForge CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE DSAForge::dsaforge)
```

Configure the consuming project with the install prefix in `CMAKE_PREFIX_PATH`, for example:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$HOME/.local"
```

## Current limitations

This is the initial foundation, not a finished STL replacement. Iterators, allocator customization, sanitizers in CI, benchmarks, and broader algorithm modules are future work. The `find` pointer is invalidated when its node is erased or the list is cleared/destroyed.
