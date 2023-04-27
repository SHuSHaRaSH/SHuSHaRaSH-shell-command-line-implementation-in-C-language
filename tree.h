#ifndef TREE_H
#define TREE_H

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

enum type_of_next{
    NXT, AND, OR   // виды связей соседних команд в списке команд
};

struct cmd_inf {
    int argc;
    char ** argv; // список из имени команды и аргументов
    char *infile; // переназначенный файл стандартного ввода
    char *outfile; // переназначенный файл стандартного вывода
    int append;
    int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
    struct cmd_inf* psubcmd; // команды для запуска в дочернем shell
    struct cmd_inf* pipe; // следующая команда после "|"
    struct cmd_inf* next; // следующая после ";" (или после "&")
    enum type_of_next type;// связь со следующей командой через ; или && или ||
};
typedef struct cmd_inf *tree;

tree list_to_tree(char** mas);
tree Init_cmd_inf();

void printTree(tree tree);
void deltree(tree tree);
char isspesial (const char *str);

//void ERprint_argv(char **p, int shift);

#endif //TREE_H