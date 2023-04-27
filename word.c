#include "word.h"
#define STR 255


static int sizeof_mas(char** mas) {
    int size = 0;
    while (*(mas++) != NULL)
        size++;
    return size;
}
/*обрабатываем stdin*/
int isspecial(unsigned char c, int flag) {
    char sp_wr[] = {'|', '&', '>', ';', '<', '(', ')'};
    int n = 3;
    if (!flag) n = sizeof(sp_wr);
    for (int i = 0; i < n; ++i){
        if (c == sp_wr[i]) return 1;}
    return 0;
}
char buff[STR]; /* буфер для ungetch*/
int bufp = 0; /* след. свободная позиция в буфере */
/*дай (возможно возвращенную) литеру */
int getch(void)
{
/*    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigset_t   set;
    sigemptyset(&set);
    sigaddset(&set, SIGHUP);
    sigaction(SIGHUP, &act, 0);
    //signal(SIGHUP, &hdl);
    int c = ;*/
    return (bufp > 0) ? buff[--bufp] : getchar();
}
/* верни литеру на ввод */
void ungetch (char c)
{
    if (bufp >= STR)
        printf("too long word\n");
    else
        buff[bufp ++] = c;
}

int get_w(char* *word, int lim) {
    char c, i = 0;

    while (isspace(c = getch())) { if (c == '\n') return EOL; }
    if (c == EOF) return EOF;

    char *w = malloc(BUFSIZ*sizeof(char));
    *word = w;
    if (isspecial(c, 0)) {
        w[i] = c; i++;
        if (isspecial(c = (char) getch(), 1)) {
            w[i] = c; i++;
        } else ungetch(c);
    } else if (c == '"') {
        while ((c = (char) getch()) != '"') {
            w[i] = c; i++;
        }
    } else
        while (--lim > 0) {
            if (c == '#') {
                while ((c = (char) getch()) != '\n') {}
                ungetch(c);
                free(*word);
                return EOL;
            }
            w[i] = c; i++;
            if (isspecial(c = (char) getch(), 0)
                || isspace(c) || c == '#') {
                ungetch(c);
                break;
            }
        }
    w[i] = '\0';
    if (lim == 0) {free(*word); return 2;}
    *word = realloc(*word, i*sizeof (char));
    return 0;
}
#define MAX_PATH 32
#define PATH_MAX
#include <limits.h>
char *path_handler(char *word) {
    if (word[0] != '$')
        return word;

    if (!strcmp(word, "$HOME")) {
        word = realloc(word, MAX_PATH);
        strcpy(word, getenv("HOME"));
    } else if (!strcmp(word, "$SHELL")) {
        word = realloc(word, MAX_PATH);
        getcwd(word, PATH_MAX + 1);
    } else if (!strcmp(word, "$USER")) {
        word = realloc(word, MAX_PATH);
        strcpy(word, getenv("LOGNAME"));
    } else if (!strcmp(word, "$EUID")) {
        word = realloc(word, MAX_PATH);
        sprintf(word, "%d", getuid());
    }
    return word;
}
