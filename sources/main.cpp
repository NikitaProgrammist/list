#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main() {
  system("rm -rf \"/home/nikita/Рабочий стол/list/img/\"");
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
  return 0;
}
