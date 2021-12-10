#make file - this is a comment section

all: 	p3.c process_list.o dynamic_list.o memorylist.o
	gcc -Wall -g -O0 -o p3 p3.c processlist.c dynamic_list.c memorylist.c		#-g compila los objetos de depuración		//-O0 optimización 0(no cambie el código al depurar)
	./p3

process_list.o: processlist.c processlist.h
	gcc -g -O0 -c processlist.c

dynamic_list.o: dynamic_list.c dynamic_list.h
	gcc -g -O0 -c dynamic_list.c

memorylist.o: memorylist.c memorylist.h
	gcc -g -O0 -c memorylist.c
