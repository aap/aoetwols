SRC=aoktest.c view.c map.c world.c game.c util.c shape.c drs.c bmp.c pal.c zfile.c
CFLAGS=-g -Wall -fno-diagnostics-show-caret -m32
LDFLAGS=-lSDL2main -lSDL2 -lz

ifdef SYSTEMROOT
	LDFLAGS += -lmingw32
endif

aoktest: $(SRC) aok.h aokbase.h
	gcc -o $@ $(CFLAGS) $(SRC) $(LDFLAGS)

mkicm: mkicm.c bmp.c util.c pal.c aokbase.h
	gcc -o mkicm mkicm.c bmp.c util.c pal.c
