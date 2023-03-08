# make file implementation inspired by COMP20003 w2.3 workshop

# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# Define libraries to be linked (for example -lm)
LDLIBS = 

# Define set of implementation source files and object files
SRC = data.c main.c dictionary.c dict3.c dict4.c
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)
 

# the first target:
dict3: data.o main.o dictionary.o dict3.o dict4.o
	gcc -o dict3 data.o main.o dictionary.o dict3.o dict4.o

# the second target:
dict4: data.o main.o dictionary.o dict3.o dict4.o
	gcc -o dict4 data.o main.o dictionary.o dict3.o dict4.o


# Other targets specify how to create .o files and what they rely on
	
main.o: main.c 
	gcc -c main.c 

data.o: data.c data.h
	gcc -c data.c

dictionary.o: dictionary.c dictionary.h
	gcc -c dictionary.c

dict3.o: dict3.c dict3.h
	gcc -c dict3.c

dict4.o: dict4.c dict4.h
	gcc -c dict4.c

clean:
	rm -f *.o dict3 dict4
