#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "word.h"
#include "handler.h"
#define ERRINP 5


void inv() /*приглашение к работе*/ {
    printf("%s", "\x1b[32m"); /*здесь изменяется цвет на зеленый */
    char s[100]; /* ограничение: имя хоста и текущей директории не должно быть слишком длинным! */
    gethostname(s, 100);
    printf("%s@%s", getenv("USER"), s);
    printf("%s", "\033[22;34m"); /* здесь изменяется цвет на blue */
    getcwd(s, 100);
    printf(":%s", s);
    printf("%s", "\033[01;37m"); /* здесь изменяется цвет на white */
    printf("$");
}

int parse(char* **arg){
    int flag;
    char *word = NULL;
    while ((flag = get_w(&word, BUFSIZ)) != EOL) {
        if (flag == EOF) return EOF;
        if (!flag) addToList(arg, path_handler(word));
        else if (flag == 2) fprintf(stderr, "Do you really want me to do this terrible command!?\n");
    }
    if (!word) return ERRINP;
    /*if (check_com(*arg)){
        fprintf(stderr, "incorrect command, try again\n");
        return ERRINP;
    }*/
    if (check_brace(*arg)) {
        fprintf(stderr, "incorrect braces, try again\n");
        dellist(*arg);
        return ERRINP;
    }
    printList(*arg);
    printf("\n");
    return 0;
}

int main (void)
{
    printf("Start\n");
    while(1) {

        inv();

        signal(SIGINT, SIG_IGN);
        char **arg = NULL;
        int flag = parse(&arg);
        if (flag == EOF) break;
        if (flag == ERRINP) continue;
        signal(SIGINT, SIG_DFL);

        tree cmd = list_to_tree(arg);
        playtree(cmd);
        printTree(cmd);

        dellist(arg);
        deltree(cmd);
    }
    printf("\nFinish\n");
    return 0;
}