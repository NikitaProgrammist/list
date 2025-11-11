#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <new>
#include <boost/pool/pool_alloc.hpp>
#include <boost/container/list.hpp>

#include "list.h"
#include "time_memory.h"

void check() {
  std::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> l1 = {};
  boost::container::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> l3 = {};
  List * l2 = NULL;
  listInit(&l2);
  stdInsert(&l1);
  stdDelete(&l1);
  boostInsert(&l3);
  boostDelete(&l3);
  myInsert(l2);
  myDelete(l2);
  listDestroy(l2);
}

void stdInsert(std::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l1) {
  time_t t0 = clock();
  l1->assign(300000000, 1);
  time_t t1 = clock();
  printf("std: %lg\n", (double) (t1 - t0) / CLOCKS_PER_SEC);
}

void boostInsert(boost::container::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l3) {
  time_t t0 = clock();
  l3->assign(300000000, 1);
  time_t t1 = clock();
  printf("boost: %lg\n", (double) (t1 - t0) / CLOCKS_PER_SEC);
}

void myInsert(List * l2) {
  time_t t2 = clock();
  for (int i = 0; i < 300000000; i++) {
    listInsertRight(l2, 0, 1);
  }
  time_t t3 = clock();
  size_t size = 0;
  listGetSize(l2, &size);
  printf("my: %lg\n", (double) (t3 - t2) / CLOCKS_PER_SEC);
}

void stdDelete(std::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l1) {
  time_t t4 = clock();
  l1->clear();
  time_t t5 = clock();
  printf("std delete: %lg\n", (double) (t5 - t4) / CLOCKS_PER_SEC);
}

void boostDelete(boost::container::list<list_t, boost::fast_pool_allocator<list_t, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex, 4096u, 2048u>> * l3) {
  time_t t4 = clock();
  l3->clear();
  time_t t5 = clock();
  printf("boost delete: %lg\n", (double) (t5 - t4) / CLOCKS_PER_SEC);
}

void myDelete(List * l2) {
  list_t elem = 0;
  time_t t6 = clock();
  for (int i = 0; i < 300000000; i++) {
    listDelete(l2, 0, &elem);
  }
  time_t t7 = clock();
  printf("my delete: %lg\n", (double) (t7 - t6) / CLOCKS_PER_SEC);
}
