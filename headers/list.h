#ifndef LIST_H
#define LIST_H

typedef struct List List;
typedef double list_t;

enum ListErr {
  SUCCESS,
  ASSERTED,
  CREATE_FAILED,
  NULL_POINTER,
  DELETE_FAILED,
  ALLOC_FAILED,
  CALLOC_FAILED,
  REALLOC_FAILED,
  CHECK_FAILED,
  CYCLE_ERR,
  INCORRECT_INDEX
};

ListErr listInit(List ** list);
ListErr listInsertRight(List * list, size_t index, list_t value);
ListErr listInsertLeft(List * list, size_t index, list_t value);
ListErr listDelete(List * list, size_t index, list_t * value);
ListErr listDestroy(List * list);
ListErr listGetLen(List * list, size_t * len);
ListErr listGetSize(List * list, size_t * size);
ListErr listGetNextIndex(List * list, size_t index, size_t * next);
ListErr listGetElem(List * list, size_t index, list_t * elem);
ListErr listToArray(List * list, list_t * array, size_t * len);
ListErr arrayToList(List * list, list_t * array, size_t * len);

#endif // LIST_H
