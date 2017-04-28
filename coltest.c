#include "aokbase.h"

int
main(int argc, char *argv[])
{
	int i;
	FILE *f;
	Palette pal, pal2;
	uchar colmap[256];
	char path[260];

	if(argc < 3)
		return 1;

	f = mustopen(argv[1], "r");
	PaletteReadJASC(&pal, f);
	fclose(f);

	f = mustopen(argv[2], "r");
	for(i = 0; i < 256; i++)
		fscanf(f, "%hhd\n", &colmap[i]);
	fclose(f);

	for(i = 0; i < 256; i++)
		pal2.col[i] = pal.col[colmap[i]];

	if(argc > 3)
		f = mustopen(argv[3], "wb");
	else{
		strcpy(path, argv[1]);
		strcat(path, ".bmp");
		f = mustopen(path, "wb");
	}
	PaletteWriteBmp(&pal2, f);
	fclose(f);
	return 0;
}
