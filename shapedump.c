#include "aokbase.h"

int
main(int argc, char *argv[])
{
	FILE *f;
	Palette pal;
	Shape *slp;
	uchar *data;
	int size;

	if(argc < 3)
		return 1;

	f = mustopen(argv[1], "r");
	PaletteReadJASC(&pal, f);
	fclose(f);

	data = getFileContents(argv[2], &size);
	slp = ShapeCreate(data);
	slp->size = size;

	ShapeDumpFrame(argv[3], &pal, slp, 1);

	return 0;
}
