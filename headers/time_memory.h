#ifndef TIME_MEMORY_H
#define TIME_MEMORY_H

struct AllocStats {
  static inline size_t total_bytes = 0;
  static inline size_t peak_bytes = 0;

};

template <typename T>
struct SimpleCountingAllocator : AllocStats {
    using value_type = T;

    SimpleCountingAllocator() = default;

    T* allocate(size_t n) {
        size_t bytes = n * sizeof(T);
        total_bytes += bytes;
        if (total_bytes > peak_bytes) {
          peak_bytes = total_bytes;
        }
        return static_cast<T*>(::operator new(bytes));
    }

    void deallocate(T* p, size_t n) noexcept {
        size_t bytes = n * sizeof(T);
        total_bytes -= bytes;
        ::operator delete(p);
    }
};

void check();

#endif // TIME_MEMORY_H
