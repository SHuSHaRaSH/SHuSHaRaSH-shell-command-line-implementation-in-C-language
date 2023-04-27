#include "list.h"

//return size of array
int sizeof_mas(char** mas) {
    int size = 0;
    while (*(mas++) != NULL)
        size++;
    return size;
}
static char *get_next_str(list node) {
    int number = node->current_string++;
    if (number >= node->mas_size) {
        return NULL;
    }
    return node->mas[number];
}
list createList(char ** mas) {
    list new_iter = malloc(sizeof(struct line));
    new_iter->mas = mas;
    new_iter->mas_size = sizeof_mas(mas);
    new_iter->current_string = 0;
    new_iter->next_string = get_next_str;
    return new_iter;
}

// обработать флаг на сущ некст
void addToList(char** *mas, char *word) {
    if (*mas == NULL) {
        *mas = malloc(2 * sizeof(char *));
        (*mas)[0] = word;
        (*mas)[1] = NULL;
        return;
    }
    int length = sizeof_mas(*mas);
    *mas = realloc(*mas, (length + 2) * sizeof(char *));
    (*mas)[length] = word;
    (*mas)[length + 1] = NULL;
}

// sorts list
//swap
void swap(char ** first, char** second) {
    void *temporary = *first;
    *first = *second;
    *second = temporary;
}
void sort_list(char** mas) {
    if (mas == 0) return;
    int length = sizeof_mas(mas);
    for (int i = 1; i < length; i++)
        for (int j = 0; j < length - i; j++)
            if (strcmp(mas[j], mas[j + 1]) > 0)
                swap(&mas[j], &mas[j + 1]);
}

//prints list
void printList(char** mas) {
    if (mas == 0) {
        printf("list is empty\n");
        return;
    }
    int length = sizeof_mas(mas);
    //printf("length of list is %d\n", length);
    for (int i = 0; i < length; i++) {
        printf("%s\n", mas[i]);
    }
}

//deletes list, free memory
void dellist(char** mas) {
    if (mas == 0)
        return;
    int length = sizeof_mas(mas);
    for (int i = 0; i <= length; i++)
        free((mas)[i]);
    free(mas);
}

// 0 for right sequence
int check_brace(char** mas){
    int brace = 0;
    for (int i = 0; mas[i]!=NULL; ++i){
        if (strcmp(mas[i], "(")==0)
            ++brace;
        else if(strcmp(mas[i], ")") == 0)
            if(--brace < 0)
                return 1;
    }
    return brace;
}

int check_com(char** mas){
    int res = 0;
    for (int i = 0; mas[i]!=NULL; ++i){
        if (!access(mas[i], 0)) ++res;
    }
    printf("\nres = %d\n", res);
    return res;
}