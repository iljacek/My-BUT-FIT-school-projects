CC=gcc

all: build

build: code_generator.o expression_parser.o parser.o scanner.o str.o symstack.o symtable.o
	gcc -g -Wall -Werror -Wextra -pedantic --std=c99 -o ifj18  main.c code_generator.c expression_parser.c parser.c scanner.c str.c symstack.c symtable.c token_list.c

clean:
	rm *.o ifj18
