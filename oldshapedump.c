#include "aokbase.h"

int
main(int argc, char *argv[])
{
	FILE *f;
	Palette pal;
	OldShape *shp;
	uchar *data;
	int size;
	Surface s;
	int a = 1;
	uchar *colmap = nil;

	if (argc < 4)
		return 1;

	f = mustopen(argv[a++], "r");
	PaletteReadJASC(&pal, f);
	fclose(f);

	data = getFileContents(argv[a++], &size);
	shp = OldShapeCreate(data);

	if (argc > 4)
		colmap = ColormapRead(argv[a++]);

	OldShapeDumpFrame(argv[a++], &pal, shp, 0, colmap);

	return 0;
}
