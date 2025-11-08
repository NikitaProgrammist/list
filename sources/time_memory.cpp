#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <new>

#include "list.h"
#include "time_memory.h"

void check() {
  std::list<double> l1 = {};
  List * l2 = NULL;
  listInit(&l2);
  stdInsert(&l1);
  myInsert(l2);
  stdDelete(&l1);
  myDelete(l2);
  listDestroy(l2);
}

void stdInsert(std::list<double> * l1) {
  time_t t0 = clock();
  for (int i = 0; i < 100000000; i++) {
    l1->emplace_front(rand() % 1000000);
  }
  time_t t1 = clock();
  printf("std: %lg\n", (double) (t1 - t0) / CLOCKS_PER_SEC);
}

void myInsert(List * l2) {
  time_t t2 = clock();
  for (int i = 0; i < 100000000; i++) {
    listInsertRight(l2, 0, rand() % 1000000);
  }
  time_t t3 = clock();
  size_t size = 0;
  listGetSize(l2, &size);
  printf("my: %lg\n", (double) (t3 - t2) / CLOCKS_PER_SEC);
}

void stdDelete(std::list<double> * l1) {
  time_t t4 = clock();
  for (int i = 0; i < 100000000; i++) {
    l1->pop_front();
  }
  time_t t5 = clock();
  printf("std delete: %lg\n", (double) (t5 - t4) / CLOCKS_PER_SEC);
}

void myDelete(List * l2) {
  list_t elem = 0;
  time_t t6 = clock();
  for (int i = 0; i < 100000000; i++) {
    listDelete(l2, 0, &elem);
  }
  time_t t7 = clock();
  printf("my delete: %lg\n", (double) (t7 - t6) / CLOCKS_PER_SEC);
}
