#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "private_list.h"
#include "check_list.h"

ListErr listInit(List ** list) {
  const int init_elem = 100;
  *list = (List *) calloc(1, sizeof(List));
  if (!*list) {
    return CREATE_FAILED;
  }
  (*list)->first_canary = (*list)->last_canary = 0xFEE1DEAD;
  (*list)->size = init_elem;
  (*list)->data = (list_t *) calloc(init_elem, sizeof(list_t));
  (*list)->next = (int *) calloc(init_elem, sizeof(int));
  (*list)->prev = (int *) calloc(init_elem, sizeof(int));
  (*list)->free_head = 0;
  (*list)->len = 0;
  if ((*list)->data == NULL || (*list)->next == NULL || (*list)->prev == NULL) {
    return CALLOC_FAILED;
  }
  for (size_t i = 0; i < init_elem; i++) {
    (*list)->next[i] = i + 1;
    (*list)->prev[i] = -1;
  }
  listVerify(*list, "AFTER");
  return SUCCESS;
}

ListErr listInsertRight(List * list, size_t index, list_t value) {
  listVerify(list, "BEFORE");
  if (index > list->len) {
    return INCORRECT_INDEX;
  }
  list->data[list->free_head] = value;
  list_t swap = list->next[list->free_head];
  list->next[list->free_head] = list->next[index];
  list->next[index] = list->free_head;
  list->free_head = swap;
  list->prev[list->next[index]] = index;
  list->prev[list->next[list->next[index]]] = list->next[index];
  list->len++;
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listInsertLeft(List * list, size_t index, list_t value) {
  if (index > list->len) {
    return INCORRECT_INDEX;
  }
  return listInsertRight(list, list->prev[index], value);
}

ListErr listDelete(List * list, size_t index, list_t * value) {
  if (index > list->len) {
    return INCORRECT_INDEX;
  }
  listVerify(list, "BEFORE");
  list->prev[index] = list->next[index];
  list->next[list->prev[index]] = list->next[index];
  list->next[index] = list->free_head;
  list->free_head = index;
  list->prev[index] = -1;
  list->len--;
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listDestroy(List * list) {
  listVerify(list, "BEFORE");
  free(list->prev);
  free(list->next);
  free(list->data);
  free(list);
  return SUCCESS;
}

ListErr listGetLen(List * list, size_t * len) {
  listVerify(list, "BEFORE");
  *len = list->len;
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listGetSize(List * list, size_t * size) {
  listVerify(list, "BEFORE");
  *size = list->size;
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listGetNextIndex(List * list, size_t index, size_t * next) {
  listVerify(list, "BEFORE");
  if (index > list->size || list->prev[index] == -1) {
    return INCORRECT_INDEX;
  }
  *next = list->next[index];
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listGetElem(List * list, size_t index, list_t * elem) {
  listVerify(list, "BEFORE");
  if (index > list->size || list->prev[index] == -1) {
    return INCORRECT_INDEX;
  }
  *elem = list->data[index];
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listToArray(List * list, list_t * array, size_t * len) {
  return SUCCESS;
}

ListErr arrayToList(List * list, list_t * array, size_t * len) {
  return SUCCESS;
}
