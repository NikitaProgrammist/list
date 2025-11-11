#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "private_list.h"
#include "check_list.h"

ListErr listInit(List ** list) {
  const int init_elem = 10;
  *list = (List *) calloc(1, sizeof(List));
  if (!*list) {
    return CREATE_FAILED;
  }
  (*list)->first_canary = (*list)->last_canary = 0xFEE1DEAD;
  (*list)->size = init_elem;
  (*list)->data = (list_t *) calloc(init_elem, sizeof(list_t));
  (*list)->next = (int *) calloc(init_elem, sizeof(int));
  (*list)->prev = (int *) calloc(init_elem, sizeof(int));
  (*list)->free_head = 1;
  (*list)->len = 0;
  if ((*list)->data == NULL || (*list)->next == NULL || (*list)->prev == NULL) {
    return CALLOC_FAILED;
  }
  for (size_t i = 1; i < init_elem; i++) {
    (*list)->next[i] = i + 1;
    (*list)->prev[i] = -1;
  }
  listVerify(*list, "AFTER");
  return SUCCESS;
}

ListErr listInsertRight(List * list, size_t index, list_t value) {
  listVerify(list, "BEFORE");
  if (index >= list->size || list->prev[index] == -1) {
    return INCORRECT_INDEX;
  }
  list->data[list->free_head] = value;
  int swap = list->next[list->free_head];
  list->next[list->free_head] = list->next[index];
  list->next[index] = list->free_head;
  list->free_head = swap;
  list->prev[list->next[index]] = index;
  list->prev[list->next[list->next[index]]] = list->next[index];
  list->len++;
  if (list->free_head >= list->size) {
    ListErr result = listRealloc(list);
    listVerify(list, "AFTER");
    return result;
  }
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listRealloc(List * list) {
  list->size *= 2;
  list->data = (list_t *) realloc(list->data, list->size * sizeof(list_t));
  list->next = (int *) realloc(list->next, list->size * sizeof(int));
  list->prev = (int *) realloc(list->prev, list->size * sizeof(int));
  if (list->data == NULL || list->next == NULL || list->prev == NULL) {
    return REALLOC_FAILED;
  }
  for (size_t i = list->free_head; i < list->size; i++) {
    list->next[i] = i + 1;
    list->prev[i] = -1;
  }
  return SUCCESS;
}

ListErr listInsertLeft(List * list, size_t index, list_t value) {
  if (index >= list->size || list->prev[index] == -1) {
    return INCORRECT_INDEX;
  }
  return listInsertRight(list, list->prev[index], value);
}

ListErr listDelete(List * list, size_t index, list_t * value) {
  index++;
  if (index >= list->size || list->prev[index] == -1) {
    return INCORRECT_INDEX;
  }
  listVerify(list, "BEFORE");
  *value = list->data[index];
  list->next[list->prev[index]] = list->next[index];
  list->prev[list->next[index]] = list->prev[index];
  list->next[index] = list->free_head;
  list->free_head = index;
  list->prev[index] = -1;
  list->len--;
  listVerify(list, "AFTER");
  return SUCCESS;
}

void listDestroy(List * list) {
  free(list->prev);
  free(list->next);
  free(list->data);
  free(list);
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

ListErr listBegin(List * list, size_t * head) {
  listVerify(list, "BEFORE");
  *head = list->next[0];
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr listEnd(List * list, size_t * tail) {
  listVerify(list, "BEFORE");
  *tail = 0;
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

ListErr listGetPrevIndex(List * list, size_t index, size_t * prev) {
  listVerify(list, "BEFORE");
  if (index > list->size || list->prev[index] == -1) {
    return INCORRECT_INDEX;
  }
  *prev = list->prev[index];
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

ListErr ListToArray(List * list, list_t ** array, size_t * len) {
  listVerify(list, "BEFORE");
  *len = list->len;
  size_t start = 0, end = 0, i = 0;
  list_t elem = 0;
  *array = (list_t *) calloc(list->len, sizeof(list_t));
  listBegin(list, &start);
  listEnd(list, &end);
  for (; start != end; listGetNextIndex(list, start, &start)) {
    listGetElem(list, start, &elem);
    (*array)[i++] = elem;
  }
  listVerify(list, "AFTER");
  return SUCCESS;
}

ListErr ArrayToList(List * list, list_t * array, size_t len) {
  listVerify(list, "BEFORE");
  if (list->next[0] != 0) {
    return NOT_EMPTY_LIST;
  }
  for (size_t i = 0; i < len; i++) {
    listInsertRight(list, i, array[i]);
  }
  listVerify(list, "AFTER");
  return SUCCESS;
}
