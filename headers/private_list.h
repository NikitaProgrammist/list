#ifndef PRIVATE_LIST_H
#define PRIVATE_LIST_H

struct List {
  size_t first_canary;
  list_t * data;
  int * next;
  int * prev;
  size_t size;
  size_t len;
  size_t free_head;
  size_t last_canary;
};

#endif // PRIVATE_LIST_H
