CC=gcc
LDFLAG=$(shell sdl-config --cflags --libs) -lpthread -D_REENTRANT -lSDL_ttf -lSDL_image -lSDL_gfx
CFLAG=-Wall $(shell sdl-config --cflags --libs) 
EXEC=jeu
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

run: all
	./jeu


