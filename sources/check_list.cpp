#ifdef CHECK

#include <stdio.h>
#include <stdlib.h>

#include "my_printf.h"
#include "list.h"
#include "private_list.h"
#include "check_list.h"

Errors verify(const List * list, const char * file, const char * func, const size_t len, const char * position) {
  if (checkCycle(list) != VALID) {
    dump(list, file, func, len, position, INVALID);
    return INVALID;
  }
  dump(list, file, func, len, position, VALID);
  const size_t max_size = 100;
  checkStatement(list->first_canary != 0xFEE1DEAD || list->last_canary != 0xFEE1DEAD);
  checkStatement(!list || !list->data || !list->next || !list->prev);
  checkStatement(list->len >= max_size);
  checkStatement(list->size > max_size);
  checkStatement(list->next[list->free_head] == -1 || list->prev[list->free_head] != -1);
  return VALID;
}

Errors checkCycle(const List * list) {
  int * visited = (int *) calloc(list->size, sizeof(int));
  int cycle_count = 0, visit_count = 0;
  int current = list->next[0];
  while (current > 0 && current < list->size) {
    if (visited[current]) {
      cycle_count++;
      break;
    }
    visited[current] = 1;
    visit_count++;
    current = list->next[current];
  }
  free(visited);
  return (cycle_count == 0 && current == 0 && visit_count == list->len) ? VALID : INVALID;
}

void dump(const List * list, const char * file, const char * func, const size_t len, const char * position, Errors img) {
  FILE * fp = fopen("../log.html", "a");
  if (fp == NULL) {
    return;
  }
  fprintf(fp, "<pre>\n");
  fprintf(fp, "<h3> DUMP <font color=red> %s </font> %s (%zu) </h3>\n", position, func, len);
  fprintf(fp, "List { ../%s:%zu }\n", file, len);
  size_t max_print = list->size < 50 ? list->size : 50;
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
      fprintf(fp, "\nData: %4.02lf", list->data[0]);
      for (size_t i = 1; i < max_print; i++) {
        fprintf(fp, "  %4.02lf", list->data[i]);
      }
      if (!img) {
        createGraph(list, picture_number);
        fprintf(fp, "\nImage:\n<img src=./img/img%zu.png width=1000px>\n", picture_number);
        picture_number++;
      }
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
  fprintf(file, "digraph G {\n");
  fprintf(file, "  bgcolor=\"#1f1f1f\";\n");
  fprintf(file, "  rankdir=LR;\n");
  fprintf(file, "  nodesep=0.8;\n");
  fprintf(file, "  ranksep=0.8;\n");
  fprintf(file, "  compound=true;\n\n");
  fprintf(file, "  node [shape=plain, fontname=\"Arial\", fontsize=12, style=\"filled,setlinewidth(2)\", fillcolor=\"#6e7681\", color=\"#6e7681\", fontcolor=\"black\", margin=\"0.05\"];\n");
  fprintf(file, "  edge [penwidth=1, arrowsize=0.5];\n\n");
  fprintf(file, "  node0 [label=<\n");
  fprintf(file, "    <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" bgcolor=\"#6e7681\" color=\"#FFFF00\">\n");
  fprintf(file, "      <tr><td port=\"head\" border=\"1\" color=\"#ffff66\"><b>head = %d</b></td></tr>\n", list->next[0]);
  fprintf(file, "      <tr><td port=\"tail\" border=\"1\" color=\"#ffff66\">tail = %d</td></tr>\n", list->prev[0]);
  fprintf(file, "      <tr><td port=\"free_head\" border=\"1\" color=\"#ffff66\">free_head = %zu</td></tr>\n", list->free_head);
  fprintf(file, "    </table>\n");
  fprintf(file, "  >];\n\n");
  for (size_t i = 1; i < list->size; i++) {
    if (list->prev[i] == -1) {
      fprintf(file, "  node%zu [label=<\n", i);
      fprintf(file, "    <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" bgcolor=\"#6e7681\" color=\"#FFFFFF\">\n");
      fprintf(file, "      <tr><td port=\"idx\" border=\"1\" color=\"#ffffFF\"><b>phys idx = %zu</b></td></tr>\n", i);
      fprintf(file, "      <tr><td port=\"elem\" border=\"1\" color=\"#ffffFF\">elem = PZN</td></tr>\n");
      fprintf(file, "      <tr><td port=\"pointers\" border=\"1\" color=\"#ffffFF\">\n");
      fprintf(file, "        <table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n");
      fprintf(file, "          <tr>\n");
      fprintf(file, "            <td port=\"prev\" border=\"1\" color=\"#ffffFF\">prev = %d</td>\n", list->prev[i]);
      fprintf(file, "            <td port=\"next\" border=\"1\" color=\"#ffffFF\">next = %d</td>\n", list->next[i]);
    }
    else {
      fprintf(file, "  node%zu [label=<\n", i);
      fprintf(file, "    <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" bgcolor=\"#6e7681\" color=\"#FFFF00\">\n");
      fprintf(file, "      <tr><td port=\"idx\" border=\"1\" color=\"#ffff00\"><b>phys idx = %zu</b></td></tr>\n", i);
      fprintf(file, "      <tr><td port=\"elem\" border=\"1\" color=\"#ffff00\">elem = %.1lf</td></tr>\n", list->data[i]);
      fprintf(file, "      <tr><td port=\"pointers\" border=\"1\" color=\"#ffff00\">\n");
      fprintf(file, "        <table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n");
      fprintf(file, "          <tr>\n");
      fprintf(file, "            <td port=\"prev\" border=\"1\" color=\"#ffff00\">prev = %d</td>\n", list->prev[i]);
      fprintf(file, "            <td port=\"next\" border=\"1\" color=\"#ffff00\">next = %d</td>\n", list->next[i]);
    }
    fprintf(file, "          </tr>\n");
    fprintf(file, "        </table>\n");
    fprintf(file, "      </td></tr>\n");
    fprintf(file, "    </table>\n");
    fprintf(file, "  >];\n\n");
  }
  fprintf(file, "  layout=neato;\n");
  fprintf(file, "  overlap=false;\n");
  fprintf(file, "  splines=false;\n");
  fprintf(file, "  bgcolor=\"#1f1f1f\";\n\n");
  fprintf(file, "  node [shape=plaintext, fontname=\"Arial\", fontsize=12];\n");
  fprintf(file, "  edge [penwidth=2, arrowsize=0.8, dir=both];\n\n");
  int base_x = 100;
  int step = 200;
  for (size_t i = 0; i < list->size; i++) {
    fprintf(file, "  node%zu [pos=\"%d,0!\"];\n", i, base_x + (int)i * step);
  }
  int head_x = base_x + list->next[0] * step;
  fprintf(file, "  HEAD_PTR [label=\"HEAD\", shape=box, style=filled, fillcolor=\"#4d78cc\", fontcolor=\"white\", pos=\"%d,120!\"];\n\n", head_x);
  int tail_index = 0;
  for (int i = list->next[0]; i > 0 && i < list->size; i = list->next[i]) {
    if (list->next[i] <= 0 || list->next[i] >= list->size) {
      tail_index = i;
      break;
    }
  }
  int tail_x = base_x + tail_index * step;
  fprintf(file, "  TAIL_PTR [label=\"TAIL\", shape=box, style=filled, fillcolor=\"#4d78cc\", fontcolor=\"white\", pos=\"%d,120!\"];\n\n", tail_x);
  int free_head_x = base_x + list->free_head * step;
  fprintf(file, "  FREE_HEAD_PTR [label=\"FREE_HEAD\", shape=box, style=filled, fillcolor=\"#4d78cc\", fontcolor=\"white\", pos=\"%d,120!\"];\n\n", free_head_x);
  fprintf(file, "  HEAD_PTR -> node%d [color=\"#4d78cc\", penwidth=3, arrowhead=none, arrowtail=none];\n", list->next[0]);
  fprintf(file, "  TAIL_PTR -> node%d [color=\"#4d78cc\", penwidth=3, arrowhead=none, arrowtail=none];\n\n", tail_index);
  fprintf(file, "  FREE_HEAD_PTR -> node%zu [color=\"#4d78cc\", penwidth=3, arrowhead=none, arrowtail=none];\n\n", list->free_head);
  int arrow_count = 0;
  for (int i = list->next[0]; i > 0 && i < list->size; i = list->next[i]) {
    if (list->next[i] > 0 && list->next[i] < list->size) {
      int current_x = base_x + i * step;
      int next_x = base_x + list->next[i] * step;
      int height = 60 + (arrow_count * 25);
      fprintf(file, "  exit_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, current_x + 60, height);
      fprintf(file, "  entry_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, next_x - 60, height);
      fprintf(file, "  top_exit_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, current_x + 60, 40);
      fprintf(file, "  top_entry_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, next_x - 60, 40);
      fprintf(file, "  node%d -> top_exit_%d [color=\"#109b48ff\", arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  top_exit_%d -> exit_%d [color=\"#109b48ff\", arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  exit_%d -> entry_%d [color=\"#109b48ff\", dir=both, arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  entry_%d -> top_entry_%d [color=\"#109b48ff\", arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  top_entry_%d -> node%d [color=\"#109b48ff\", arrowhead=normal, arrowtail=none];\n", i, list->next[i]);
      arrow_count++;
      }
  }
  fprintf(file, "  // FREE list bidirectional arrows with VERTICAL connections\n");
  int free_arrow_count = 0;
  for (int i = list->free_head; i > 0 && i < list->size; i = list->next[i]) {
    if (list->next[i] > 0 && list->next[i] < list->size) {
      int current_x = base_x + i * step;
      int next_x = base_x + list->next[i] * step;
      int height = -60 - (free_arrow_count * 25);
      fprintf(file, "  free_exit_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, current_x + 60, height);
      fprintf(file, "  free_entry_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, next_x - 60, height);
      fprintf(file, "  free_top_exit_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, current_x + 60, -40);
      fprintf(file, "  free_top_entry_%d [shape=point, width=0, height=0, pos=\"%d,%d!\"];\n",
              i, next_x - 60, -40);
      fprintf(file, "  node%d -> free_top_exit_%d [color=\"#f30be7ff\", arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  free_top_exit_%d -> free_exit_%d [color=\"#f30be7ff\", arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  free_exit_%d -> free_entry_%d [color=\"#f30be7ff\", dir=both, arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  free_entry_%d -> free_top_entry_%d [color=\"#f30be7ff\", arrowhead=none, arrowtail=none];\n", i, i);
      fprintf(file, "  free_top_entry_%d -> node%d [color=\"#f30be7ff\", arrowhead=normal, arrowtail=none];\n", i, list->next[i]);
      free_arrow_count++;
    }
  }
  fprintf(file, "}\n");
  fclose(file);
  char command[256];
  snprintf(command, sizeof(command), "dot -Tpng \"../img/img%zu.txt\" -o \"../img/img%zu.png\" -n2", picture_number, picture_number);
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
