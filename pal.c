#include "aok.h"

void
PaletteMakeGreyscale(Palette *pal)
{
	int i;
	for(i = 0; i < 256; i++){
		pal->col[i].r = i;
		pal->col[i].g = i;
		pal->col[i].b = i;
	}
}

void
PaletteReadJASC(Palette *pal, FILE *f)
{
	int version, n;
	int r, g, b;
	RGB *col;

	fscanf(f, "JASC-PAL\n");
	fscanf(f, "%d\n", &version);
	fscanf(f, "%d\n", &n);
	col = pal->col;
	while(n--){
		fscanf(f, "%d %d %d\n", &r, &g, &b);
		col->r = r;
		col->g = g;
		col->b = b;
		col++;
	}
}

void
PaletteWriteBmp(Palette *pal, FILE *f)
{
	int i;
	uchar indices[16*16];

	BmpWriteHeader(16, 16, 8, f);
	BmpWritePalette(pal, f);
	for(i = 0; i < 256; i++)
		indices[i] = i;
	BmpFlipVert(16, 16, 8, indices);
	fwrite(indices, 1, 256, f);
}

float
ColorDist(RGB c1, RGB c2)
{
	float dr = c1.r - c2.r;
	float dg = c1.g - c2.g;
	float db = c1.b - c2.b;
	return dr*dr + dg*dg + db*db;
}

uchar
PaletteFindColor(Palette *pal, RGB col)
{
	int i;
	int best;
	float bestdist, d;
	best = 0;
	bestdist = 100000000.0;
	for(i = 0; i < 256; i++){
		d = ColorDist(pal->col[i], col);
		if(d < bestdist){
			bestdist = d;
			best = i;
		}
	}
	return best;
}
