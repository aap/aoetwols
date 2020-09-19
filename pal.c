#include "aok.h"

uchar*
ColormapRead(const char *filename)
{
	FILE *f;
	uchar *data;

	f = mustopen(filename, "r");
	data = malloc(256);

	for (int i = 0; i < 256; i++){
		int c;

		fscanf(f, "%d\n", &c);

		data[i] = c;
	}

	fclose(f);
	return data;
}

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
PaletteReadJASCFromFile(Palette *pal, char *name)
{
	FILE *f;
	int version, n;
	int r, g, b;
	RGB *col;

	f = mustopen(name, "r");
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
PaletteReadJASCFromString(Palette *pal, char *str)
{
	int i, r, g, b;
	RGB *col;
	char *line = NULL;

	col = pal->col;
	line = strtok(str, "\n");
	for(i = 0; i < 259; i++) {
		if(i >= 3) {
			sscanf(line, "%d %d %d\n", &r, &g, &b);
			col->r = r;
			col->g = g;
			col->b = b;
			col++;
		}

		line = strtok(NULL, "\n");
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

void
PaletteWriteBmpColMap(Palette *pal, FILE *f, uchar *map)
{
	int i;
	uchar indices[16*16];

	BmpWriteHeader(16, 16, 8, f);
	BmpWritePalette(pal, f);
	for(i = 0; i < 256; i++)
		indices[i] = map[i];
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
