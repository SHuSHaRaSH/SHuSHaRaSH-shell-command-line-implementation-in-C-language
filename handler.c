#include "handler.h"

// настраивается ввод / вывод в фаилы
void dup2file(tree unit) {
    if (unit->outfile != NULL) {
        char mode[2];
        if (unit->append == 1) strcpy(mode, "a");
        else strcpy(mode, "w");
        freopen(unit->outfile, mode, stdout);
    }
    if (unit->infile)
        freopen(unit->infile, "r", stdin);
}

// запускает (текущие/следующие) команды
int run (tree cmd) {
    //для промежуточных элементов дерева
    if (cmd->argv[0] == NULL)
        //return tree_handler(cmd->psubcmd);
        return  playtree(cmd->psubcmd);
    else { /* нет, я не устраиваю здесь зомби-апокалипсис
            * будет выдавать ошибки на cd и false
            * printf("UNIT %s in line %d\n", unit->argv[0], __LINE__);*/
        if (execvp(cmd->argv[0], cmd->argv) == -1) return 1;
        return 0;
    }
}
int runFar(tree cmd, int status){
    if (cmd->next != NULL)
        if ((cmd->type == NXT) ||
            ((cmd->type == AND) && (status == 0)) ||
            ((cmd->type == OR) && (status != 0)))
            return  playtree(cmd->next);
    return 1;
}

// перематывает дерево к концу сабшелла
tree rewind_pipe(tree unit) {
    if ((unit->pipe == NULL) && (unit->psubcmd == NULL))
        return unit;
    if (unit->pipe != NULL)
        return rewind_pipe(unit->pipe);
    if (unit->psubcmd != NULL)
        return rewind_pipe(unit->psubcmd);
    return NULL;
}
// в цикле обрабатывает пайп
int pipe_handler(tree cmd) {
    int status;
    int fd[2];
    while (cmd != NULL)
    {
        pipe(fd);
        if (!fork()) {
            signal(SIGINT, SIG_DFL);
            dup2file(cmd);
            if (cmd->pipe != NULL) dup2(fd[1], 1);
            close(fd[1]);
            close(fd[0]);

            if (run (cmd)) {
                fprintf(stderr, "error in line %d, in run pipe\n", __LINE__);
                return 1;
            }
            exit(0);
        }
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        cmd = cmd->pipe;
    }
    while (wait(&status) != -1);
    return status;
}

int playtree(tree cmd){
    int status = 0;
    if (cmd == NULL) return 0;

    // обрабатывается cd (смена директории)
    if ((cmd->argv[0] != NULL) && (strcmp(cmd->argv[0], "cd") == 0)) {
        chdir(cmd->argv[1]);
        status =  playtree(cmd->psubcmd);
        return runFar(cmd, status);
    }

    //обрабатываются пайпы
    if (cmd->pipe) {
        int in = dup(0);
        status = pipe_handler(cmd);
        dup2(in, 0);
        cmd = rewind_pipe(cmd);

        if (cmd == NULL) return status;
        return runFar(cmd, status);
    }

    //обрабатываются фоновые процессы
    if (!fork()) {
        dup2file(cmd);
        /*фон*/
        if (cmd->backgrnd) {
            signal(SIGINT, SIG_IGN);
            if (!fork()) {
                run (cmd);
                exit(0);
            } else kill(getpid(), SIGKILL);
        } else exit(run(cmd));
    } else {
        wait(&status);
        status = WEXITSTATUS(status);
        return runFar(cmd, status);
    }
}
