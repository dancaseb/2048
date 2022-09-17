# Projekt: projekt.c
# Autor:   Daniel Reeves
# Verze:   1.0
# Datum:   6.12.2021

CC=gcc
LDFLAGS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf
CFLAGS= -fsanitize=address -g 
NAME = projekt

projekt:  projekt.o gamelogic.o rendering.o gamechange.o
	#gcc $^ -o $@ $(LDFLAGS);
	$(CC) $(CFLAGS) projekt.o rendering.o gamelogic.o gamechange.o -o projekt $(LDFLAGS) 









