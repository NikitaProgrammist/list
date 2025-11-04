#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <new>

#include "list.h"
#include "time_memory.h"

void check() {
  list_t elem = 0;


  std::list<double> l1 = {};
  time_t t0 = clock();
  for (int i = 0; i < 10000000; i++) {
    l1.insert(l1.begin(), rand() % 1000000);
  }
  time_t t1 = clock();
  printf("std: %lg, %zu\n", (double) (t1 - t0) / CLOCKS_PER_SEC, sizeof(l1) * 10000000);


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


  time_t t4 = clock();
  for (int i = 0; i < 10000000; i++) {
    l1.erase(l1.begin());
  }
  time_t t5 = clock();
  printf("std delete: %lg\n", (double) (t5 - t4) / CLOCKS_PER_SEC);


  time_t t6 = clock();
  for (int i = 0; i < 10000000; i++) {
    listDelete(l2, 0, &elem);
  }
  time_t t7 = clock();
  printf("my delete: %lg\n", (double) (t7 - t6) / CLOCKS_PER_SEC);
  printf("div delete: %lg\n", double(t5 - t4) / double(t7 - t6));
  listDestroy(l2);
}
