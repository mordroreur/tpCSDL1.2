CC=gcc
LDFLAG=$(shell sdl-config --cflags --libs) -lpthread -D_REENTRANT
CFLAG=-Wall $(shell sdl-config --cflags --libs) 
EXEC=res
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all:$(EXEC)

$(EXEC):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAG)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAG)

.PHONY:clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
	rm -rf compile_commands.json


clangd: clean
	bear make
