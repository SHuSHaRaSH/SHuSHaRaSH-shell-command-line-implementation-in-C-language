
## SHEll comamd line by Shestitko Alexandr

 **This is a small command interpreter.**

### MAIN
*Consist an endless circle in which:*
1. **inv()**: that print invetaithion once 

2. **parse**: analise enter line using **word.h** and **list.h** Add every word to list

3. **list_to_tree**: Create tree and cast list to it

4. **playtree**: going through the tree and run it

5. clear memory 

## word.h 

#### Consist:
#### FUNCTIONS:

1) *int get_w(char* *word, int lim);*

    DOING:
        1) spliting words
        2) check comments (# )
        3) check " "
        4) malloc memory to word

  RETURN VALUE: new lexeme in word and 0; NULL and error code

2) *char *path_handler(char *word);*

    DOING: exchange word if it needs; processed expressions like ($VAL)

## list.h

#### Consist:

1) Struct list with char** and some special info.
#### FUNCTIONS:
2) *int sizeof_mas(char ** mas);*

3) *void addToList(char ** * mas, char * word);*

    DOING: add each word

4) *list createList(char ** mas);*

    DOING: add all special info about mas to list

5) *void dellist(char ** mas);* 

6) *void printList(char ** mas);* 

7) *int check_brace(char ** mas);*

    DOING: check if braces are right
    
8) *int check_com(char ** mas);*

    DOING: check if enter command or file are right

## tree.h

#### Consist:

1) Great struct cmd_inf with all information about mas which allow running enter line
#### FUNCTIONS:
2) *tree list_to_tree(char ** mas);*

    DOING: check if it mistakes during create_tree
3) *tree create_tree(list iter);*

     DOING: 
       1. read mas
       2. cast it to list (gather info)
       3. read list and refactor it to tree
       4. analise some errors within

4) *tree Init_cmd_inf();*

5) *void printTree(tree tree);*

6) *void deltree(tree tree);*

7) *char isspesial (const char * str);*

## handler.h

**Consist**:
Main function ***int playtree(tree cmd)*** that call yourself 
recursive, process every command in enter line.

Processing:

1. Changing directory

2. Pipes all line one by one (use ***int pipe_handler(tree cmd)*** )

3. Background process (create son and grandson)

After each part (1-3) it allows ***int runFar(tree cmd, int status)***, which make test next command and make recursive call.
Also here is function ***int run (tree cmd)*** that just play command by exec or play subshell


## **NB:**
All headers consist a few count not mentioned static function. They are small and used 
just as helper for the main functions.
