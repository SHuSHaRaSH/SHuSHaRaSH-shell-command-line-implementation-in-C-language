#include "tree.h"
#include "list.h"

jmp_buf begin;
void error(char *err, int line) {
    fprintf(stderr, "error: %s\nin line %d in file %s\n", err, line, __FILE__);
    longjmp(begin, 1);
}

static void add_argv(tree unit, char *str) {
    int size = ++unit->argc;
    unit->argv = realloc(unit->argv, (size + 1) * sizeof(char *));
    unit->argv[size - 1] = str;
    unit->argv[size] = NULL;
}

static tree create_tree(list iter) {
    tree unit = Init_cmd_inf();
    int unit_filled = 0;
    char *lexeme = iter->next_string(iter);
    while (lexeme != NULL) {
        if (strcmp(lexeme, ">") == 0) {
            if (unit->outfile != NULL) {
                deltree(unit);
                error("two input files", __LINE__);
            }
            if (isspesial(lexeme = iter->next_string(iter))) {
                deltree(unit);
                error("parse error near `>'", __LINE__);
            }
            unit->outfile = (char *) lexeme;
            unit->append = 0;

        } else if (strcmp(lexeme, ">>") == 0) {
                if (unit->outfile != NULL) {
                    deltree(unit);
                    error("two input files", __LINE__);
                }
                if (isspesial(lexeme = iter->next_string(iter))) {
                    deltree(unit);
                    error("parse error near `>>'", __LINE__);
                }
                unit->outfile = (char *) lexeme;
                unit->append = 1;
        } else if (strcmp(lexeme, "<") == 0) {
            if (unit->infile != NULL) {
                deltree(unit);
                error("two output files", __LINE__);
            }
            if (isspesial(lexeme = iter->next_string(iter))) {
                deltree(unit);
                error("parse error near `<'", __LINE__);
            }
            unit->infile = (char *) lexeme;
        } else if (strcmp(lexeme, ";") == 0) {
            if (!unit_filled) {
                deltree(unit);
                error("parse error", __LINE__);
            }
            unit->type = NXT;
            unit->next = create_tree(iter);
            break;

        } else if (strcmp(lexeme, "&&") == 0) {
            unit->type = AND;
            unit->next = create_tree(iter);
            break;

        } else if (strcmp(lexeme, "||") == 0) {
            unit->type = OR;
            unit->next = create_tree(iter);
            break;

        } else if (strcmp(lexeme, "&") == 0) {
            unit->backgrnd = 1;
            unit->next = create_tree(iter);
            break;

        } else if (strcmp(lexeme, "|") == 0) {
            unit->pipe = create_tree(iter);
            break;

        } else if (strcmp(lexeme, "(") == 0) {
            if (unit->argc != 0) {
                deltree(unit);
                error("unexpected arg before '('", __LINE__);
            }
            unit->psubcmd = create_tree(iter);

        } else if (strcmp(lexeme, ")") == 0) {
            if (!unit_filled) {
                deltree(unit);
                return NULL;
            } else
                return unit;
        } else {
            char *str = malloc(sizeof(char) * (strlen(lexeme) + 1));
            strcpy(str, lexeme);
            add_argv(unit, str);
        }
        lexeme = iter->next_string(iter);
        unit_filled = 1;
    }
    if (!unit_filled) {
        deltree(unit);
        return NULL;
    } else
        return unit;
}

tree list_to_tree(char** mas) {
    list arg = createList(mas);
    tree root;
    if (!setjmp(begin)) {
        root = create_tree(arg);
    } else {
        printf("syntax error, please try again\n");
        root = NULL;
    }
    free(arg);
    return root;
}

//--------------------------------------------
void deltree(tree tree) {
    if (tree == NULL)
        return;
    deltree(tree->psubcmd);
    deltree(tree->pipe);
    deltree(tree->next);
    for (int i = 0; tree->argv[i] != NULL; ++i)
        free(tree->argv[i]);
    free(tree->argv);
    free(tree);
}
void printTree(tree tree) {

    if (tree == NULL) return;

    printf("shell (%s):\n", tree->argv[0]);
    printf("argv: ");
    for (int i = 0; tree->argv[i] != NULL; ++i)
        printf("%s ", tree->argv[i]);
    printf("\n");
    printf("infile: %s\n", tree->infile);
    printf("outfile: %s\n", tree->outfile);
    printf("backgrnd: %d\n", tree->backgrnd);
    if (tree->psubcmd)
        printf("exist psubcmd (%s)\n", tree->psubcmd->argv[0]);
    if (tree->pipe)
        printf("exist pipe (%s)\n", tree->pipe->argv[0]);
    if (tree->next)
        printf("exist next (%s)\n", tree->next->argv[0]);

    printTree(tree->psubcmd);
    printf("\n");
    printTree(tree->pipe);
    printf("\n");
    printTree(tree->next);
    printf("\n");
}

char isspesial (const char *str){
    for (int i = 0; i < strlen((str)); ++i)
        switch (str[i]) {
            case '>': case '<': case '&': case '#':
            case '(': case ')': case '|': case ';':
                return 1;
            default:
                continue;
        }
    return 0;
}

struct cmd_inf *Init_cmd_inf() {
    struct cmd_inf *unit = malloc(sizeof(struct cmd_inf));
    unit->argc = 0;
    unit->argv = malloc(sizeof(char *));
    unit->argv[0] = NULL;
    unit->infile = NULL;
    unit->outfile = NULL;
    unit->append = 0;
    unit->backgrnd = 0;
    unit->psubcmd = NULL;
    unit->pipe = NULL;
    unit->next = NULL;
    unit->type = NXT;
    return unit;
}

//--------------------------------------
void make_shift(int n){
    while(n--)
        putc(' ', stderr);
}
void ERprint_argv(char **p, int shift) {
    char **q = p;
    if (p != NULL) {
        while (*p != NULL) {
            make_shift(shift);
            fprintf(stderr, "argv[%d]=%s\n", (int) (p - q), *p);
            p++;
        }
    }
}
