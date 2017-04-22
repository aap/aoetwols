SRC=aoktest.c view.c map.c world.c game.c util.c shape.c drs.c bmp.c pal.c zfile.c
CFLAGS=-g -Wall  -fno-diagnostics-show-caret
aoktest: $(SRC) aok.h
	gcc -o aoktest $(CFLAGS) $(SRC) -lmingw32 -lSDL2main -lSDL2 -lz
