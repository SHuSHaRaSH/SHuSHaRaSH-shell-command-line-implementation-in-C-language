CC = gcc
FLAGS = -c -o
USE = $@.o $@.c

all: main list tree word handler
	$(CC) -o shell main.o word.o list.o tree.o handler.o

main: main.c
	$(CC) $(FLAGS) $(USE)
word: word.c
	$(CC) $(FLAGS) $(USE)
list: list.c
	$(CC) $(FLAGS) $(USE)
tree: tree.c
	$(CC) $(FLAGS) $(USE)
handler: handler.c
	$(CC) $(FLAGS) $(USE)

run: shell
	./shell
clean:
	rm -rf *.o
