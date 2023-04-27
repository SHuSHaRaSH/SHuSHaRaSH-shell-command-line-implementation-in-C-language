#ifndef HANDLER_H
#define HANDLER_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "tree.h"

int playtree(tree cmd);

#endif //HANDLER_H