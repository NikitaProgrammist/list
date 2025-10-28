#ifdef CHECK

#include <stdio.h>
#include <stdlib.h>

#include "my_printf.h"
#include "list.h"
#include "private_list.h"
#include "check_list.h"

Errors verify(const List * list, const char * file, const char * func, const size_t len, const char * position) {
  if (checkCycle(list) != VALID) {
    return INVALID;
  }
  dump(list, file, func, len, position);
  const size_t max_size = 100000;
  checkStatement(list->first_canary != 0xFEE1DEAD || list->last_canary != 0xFEE1DEAD);
  checkStatement(!list || !list->data || !list->next || !list->prev);
  checkStatement(list->len >= max_size);
  checkStatement(list->size > max_size);
  checkStatement(list->next[list->free_head] == -1 || list->prev[list->free_head] != -1);
  return VALID;
}

Errors checkCycle(const List * list) {
  if (list->next[0] == 0) {
    return VALID;
  }
  int slow = list->next[0];
  int fast = list->next[0];
  while (slow < list->size && fast < list->size) {
    if (slow == fast && slow == list->next[0]) {
      return VALID;
    }
    else if (slow == fast) {
      return INVALID;
    }
    slow = list->next[slow];
    fast = list->next[list->next[fast]];
  }
  return INVALID;
}

void dump(const List * list, const char * file, const char * func, const size_t len, const char * position) {
  FILE * fp = fopen("../log.html", "a");
  if (fp == NULL) {
    return;
  }
  fprintf(fp, "<pre>\n");
  fprintf(fp, "<h3> DUMP <font color=red> %s </font> %s (%zu) </h3>\n", position, func, len);
  fprintf(fp, "List { ../%s:%zu }\n", file, len);
  const size_t max_print = 50;
  static size_t picture_number = 0;
  if (list) {
      if (list->next && list->prev) {
        fprintf(fp, "      %4d", 0);
        for (size_t i = 1; i < max_print; i++) {
          fprintf(fp, "  %4zu", i);
        }
        fprintf(fp, "\nNext: %4d", list->next[0]);
        for (size_t i = 1; i < max_print; i++) {
          fprintf(fp, "  %4d", list->next[i]);
        }
        fprintf(fp, "\nPrev: %4d", list->prev[0]);
        for (size_t i = 1; i < max_print; i++) {
          fprintf(fp, "  %4d", list->prev[i]);
        }
        fprintf(fp, "\nData: %4lf", list->data[0]);
        for (size_t i = 1; i < max_print; i++) {
          fprintf(fp, "  %4lf", list->data[i]);
        }
        createGraph(list, picture_number);
        fprintf(fp, "\nImage:\n<img src=./img/img%zu.png width=100px>\n", picture_number);
        picture_number++;
      }
      else {
        fprintf(fp, "Нулевой указатель на массивы указателей.\n");
      }
  }
  else {
    fprintf(fp, "Нулевой указатель на список.\n");
  }
  fclose(fp);
}

void createGraph(const List * list, size_t picture_number) {
    char filename[100];
    sprintf(filename, "../img/img%zu.txt", picture_number);
    FILE * file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fclose(file);

    char command[256];
    snprintf(command, sizeof(command), "dot -Tpng \"../img/img%zu.txt\" -o \"../img/img%zu.png\"", picture_number, picture_number);
    system(command);
}

void printErrorLines(const char * file, const char * func, size_t line_number) {
  colorPrintf(RED, BOLD, "file: %s:%zu\nfunc: %s\n", file, line_number, func);

  const int MAX_LINE = 150;
  char line[MAX_LINE] = {};
  FILE * fp = fopen(file, "r");

  for (size_t i = 0; i <= line_number; i++) {
    fgets(line, MAX_LINE, fp);
    if (i >= line_number - 2)
      colorPrintf(RED, BOLD, "%zu: %s", i + 1, line);
  }

  fclose(fp);
}

#endif // CHECK
