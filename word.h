#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#define EOL -2

int isspecial(unsigned char c, int flag);
int getch(void);
void ungetch (char c);
/* return value: new lexeme in word
 * 1 if word > lim, -1 = EOF, 0 if well done*/
int get_w(char* *word, int lim);
char *path_handler(char *word);