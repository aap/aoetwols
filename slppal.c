#include "aokbase.h"

/* slppal pal_in.pal pal_out.pal slp_in.slp slp_out.slp */

uchar colmap[256];

uchar
colmapCB(uchar col)
{
	return colmap[col];
}

int
main(int argc, char *argv[])
{
	int i;
	FILE *f;
	Palette pal_in, pal_out;
	Shape *slp;
	uchar *data;
	int size;

	if(argc < 4)
		return 1;

	f = mustopen(argv[1], "r");
	PaletteReadJASC(&pal_in, f);
	fclose(f);

	f = mustopen(argv[2], "r");
	PaletteReadJASC(&pal_out, f);
	fclose(f);

	data = getFileContents(argv[3], &size);
	slp = ShapeCreate(data);
	slp->size = size;

	f = mustopen(argv[4], "wb");

	for(i = 0; i < 256; i++)
		colmap[i] = PaletteFindColor(&pal_out, pal_in.col[i]);
	ShapeForallPixels(slp, colmapCB);

	fwrite(slp->header, 1, slp->size, f);
	fclose(f);

	return 0;
}
