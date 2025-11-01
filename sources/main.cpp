#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"
#include "utils.h"

#include <list>
#include <new>

struct AllocStats {
  static inline size_t total_bytes = 0;
  static inline size_t peak_bytes = 0;

};

template <typename T>
struct SimpleCountingAllocator : AllocStats {
    using value_type = T;

    SimpleCountingAllocator() = default;

    T* allocate(std::size_t n) {
        size_t bytes = n * sizeof(T);
        total_bytes += bytes;
        if (total_bytes > peak_bytes) {
          peak_bytes = total_bytes;
        }
        return static_cast<T*>(::operator new(bytes));
    }

    void deallocate(T* p, std::size_t n) noexcept {
        size_t bytes = n * sizeof(T);
        total_bytes -= bytes;
        ::operator delete(p);
    }
};

int main() {
  clear();
  List * list = NULL;
  list_t result = 0;
  listInit(&list);
  listInsertRight(list, 0, 10);
  listInsertRight(list, 0, 20);
  listDelete(list, 0, &result);
  listInsertRight(list, 1, 30);
  listInsertRight(list, 2, 40);
  listInsertRight(list, 1, 50);
  listInsertRight(list, 0, 60);
  listInsertRight(list, 4, 70);
  listInsertRight(list, 3, 80);
  listInsertRight(list, 2, 90);
  listDelete(list, 0, &result);
  List * arraylist = NULL;
  listInit(&arraylist);
  double array[15] = {1, 2, 3, 4, 5, 6, 7, 0, 8, 9, 10, 11, 12, 13, 14};
  ArrayToList(arraylist, array, 15);
  double * arr = NULL;
  size_t n = 0;
  ListToArray(list, &arr, &n);


  size_t start = 0, end = 0;
  double elem = 0;
  ListErr result1 = listBegin(arraylist, &start), result2 = listEnd(arraylist, &end);
  if (result1 == SUCCESS && result2 == SUCCESS) {
    for (; start != end; listGetNextIndex(arraylist, start, &start)) {
      listGetElem(arraylist, start, &elem);
      printf("%lg ", elem);
    }
    printf("\n");
  }


  for (size_t i = 0; i < n; i++) {
    printf("%lg ", arr[i]);
  }
  printf("\n");

  listDestroy(list);
  listDestroy(arraylist);
  free(arr);


  std::list<double, SimpleCountingAllocator<double>> l1 = {};
  time_t t0 = clock();
  for (int i = 0; i < 10000000; i++) {
    l1.insert(l1.begin(), rand() % 1000000);
  }
  time_t t1 = clock();
  printf("std: %lg, %zu\n", (double) (t1 - t0) / CLOCKS_PER_SEC, SimpleCountingAllocator<double>::peak_bytes);


  List * l2 = NULL;
  listInit(&l2);
  time_t t2 = clock();
  for (int i = 0; i < 10000000; i++) {
    listInsertRight(l2, 0, rand() % 1000000);
  }
  time_t t3 = clock();
  size_t size = 0;
  listGetSize(l2, &size);
  printf("my: %lg, %zu\n", (double) (t3 - t2) / CLOCKS_PER_SEC, 5 * sizeof(size_t) + sizeof(list_t) * size + 2 * sizeof(int) * size);
  printf("div: %lg\n", double(t1 - t0) / double(t3 - t2));
  listDestroy(l2);


  return 0;
}
