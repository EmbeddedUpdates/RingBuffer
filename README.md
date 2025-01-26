# Ringbuffer

Ringbuffer module to be used in various projects, should be mostly dependency-less, but can utilize the mempool component.


## Design
1. Should the RingBuffer be totally indepdent of the Mempool?
    1.  Independent Implementations
        1. Advantages:
        Modularity: Each module (ring buffer and memory pool) can be used independently in other projects without requiring the other.
        Reusability: You can reuse the ring buffer or memory pool in contexts where one isn't needed.
        Simplicity: The separation of concerns makes debugging and maintenance easier.
        Flexibility: You can use a different memory allocation strategy with the ring buffer or a different queue implementation with the memory pool.
        2. Implementation:
        The ring buffer manages its own memory (e.g., with a preallocated buffer or dynamic allocation).
        The memory pool manages a pool of fixed-size blocks that other components can request.
    2.  Dependent Implementation
        1. Advantages:
        Optimized Memory Management: The ring buffer can directly use the memory blocks from the memory pool, reducing fragmentation and possibly improving performance. (Syntactic recency, development optimizations to mempool)
        Encapsulation of Memory Allocation: If the memory pool provides a central place for allocation and deallocation, it can simplify overall system resource management.
        Custom Allocator Integration: You might want the ring buffer to work with a specific memory allocation strategy (e.g., pooling for real-time constraints).
        2. Implementation:
        The ring buffer uses the memory pool for dynamic allocation of its elements or storage.
        The memory pool is either tightly coupled or abstracted through an interface.

So we have already begun our implementation with "Independence", and we can use a compile-time switch to swap to 'Dependent".

If we use a Compile time switch for the RingBuffer, we then must update the Make for Ringbuffer to be adapted and run tests with various compile time switches. <-- This is a good infrastructure to implement anyway.

## Debt

## [Technical Debt for this component is located here](./debt.md#TechnicalDebt)