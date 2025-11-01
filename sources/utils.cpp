#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "utils.h"

void clear() {
  system("rm -rf \"/home/nikita/Рабочий стол/list/img/\" && mkdir \"/home/nikita/Рабочий стол/list/img/\"");
  FILE *file = fopen("filename.txt", "w");
  if (file != NULL) {
    fclose(file);
  }
}

void parseErrors(ListErr error) {
  switch (error) {
    case SUCCESS:
      break;
    case CREATE_FAILED:
      printf("Ошибка при создании\n");
      break;
    case NULL_POINTER:
      printf("Нулевой указатель\n");
      break;
    case ALLOC_FAILED:
      printf("Ошибка при аллокации\n");
      break;
    case CALLOC_FAILED:
      printf("Ошибка при аллокации\n");
      break;
    case REALLOC_FAILED:
      printf("Ошибка при реаллокации\n");
      break;
    case DELETE_FAILED:
      printf("Ошибка при удалении элементов из списка\n");
      break;
    case ASSERTED:
      break;
    case CHECK_FAILED:
      printf("Ошибка при проверке\n");
      break;
    case INCORRECT_INDEX:
      printf("Неправильный индекс элемента\n");
      break;
    case NOT_EMPTY_LIST:
      printf("Преобразование массива в уже заполненный список\n");
      break;
    case CYCLE_ERR:
      printf("Список некорректно зациклился\n");
      break;
    default:
      printf("Неизвестная ошибка\n");
      break;
  }
}
