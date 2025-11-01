#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <time.h>

#include "list.h"
#include "utils.h"

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


  std::list<double> l1 = {};
  List * l2 = NULL;
  listInit(&l2);
  time_t t0 = clock();
  for (int i = 0; i < 10000000; i++) {
    l1.insert(l1.begin(), rand() % 1000000);
  }
  time_t t1 = clock();
  printf("std: %lg\n", (double) (t1 - t0) / CLOCKS_PER_SEC);
  time_t t2 = clock();
  for (int i = 0; i < 10000000; i++) {
    listInsertRight(l2, 0, rand() % 1000000);
  }
  time_t t3 = clock();
  printf("my: %lg\n", (double) (t3 - t2) / CLOCKS_PER_SEC);
  printf("div: %lg\n", double(t1 - t0) / double(t3 - t2));
  listDestroy(l2);
  return 0;
}
