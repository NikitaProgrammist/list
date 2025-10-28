#include <stdio.h>

#include "list.h"

int main() {
  List * list = NULL;
  list_t result = 0;
  listInit(&list);
  listInsertRight(list, 0, 10);
  listInsertRight(list, 1, 20);
  listDelete(list, 0, &result);
  listInsertRight(list, 1, 30);
  listDestroy(list);
  return 0;
}
