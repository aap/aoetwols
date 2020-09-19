#include "aokbase.h"

extern int bmpres;

int
main(int argc, char *argv[])
{
	FILE *f;
	Palette pal;
	char path[260];

	bmpres = 200;

	if(argc < 2)
		return 1;

	f = fopen(argv[1], "r");
	if(f == nil)
		return 2;
	PaletteReadJASC(&pal, f);
	fclose(f);

	if(argc > 2)
		f = fopen(argv[2], "wb");
	else{
		strcpy(path, argv[1]);
		strcat(path, ".bmp");
		f = fopen(path, "wb");
	}
	if(f == nil)
		return 3;
	if (argc < 4)
		PaletteWriteBmp(&pal, f);
	else{
		uchar *map = ColormapRead(argv[3]);
		PaletteWriteBmpColMap(&pal, f, map);
		free(map);
	}
	fclose(f);
	return 0;
}
