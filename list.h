#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

struct line{
    char** mas;
    int current_string;
    int mas_size;
    char * (*next_string)(struct line*);
};
typedef struct line* list;

int sizeof_mas(char** mas);
list createList(char ** mas);
void dellist(char** mas);
void printList(char** mas);
void sortlist(char** mas);
void addToList(char** *mas, char *word);
void swap(char ** first, char** second);
void sort_list(char** mas);

int check_brace(char** mas);
int check_com(char** mas);

#endif //LIST_H