#include "aok.h"

void
BmpWriteHeader(int width, int height, int depth, FILE *f)
{
	uchar buf[54];
	uchar *p;

	int pallen = depth > 8  ? 0 :
	               depth == 4 ? 16 : 256;
	int stride = BmpGetStride(width, depth);
	depth = depth == 32 ? 24 : depth;

	// File header
	p = buf;
	*p++ = 'B';
	*p++ = 'M';
	*(int*)p = 0x36 + 4*pallen + height*stride; p += 4;
	*(short*)p = 0; p += 2;       // reserved
	*(short*)p = 0; p += 2;
	*(int*)p = 0x36 + 4*pallen; p += 4;    // data offset

	// DIB header
	*(int*)p = 0x28; p += 4;    // header size
	*(int*)p = width; p += 4;
	*(int*)p = height; p += 4;
	*(short*)p = 1; p += 2;        // number of planes
	*(short*)p = depth; p += 2;
	*(int*)p = 0; p += 4;       // compression: none
	*(int*)p = 0; p += 4;        // size, not needed in our case
	*(int*)p = 2835; p += 4;     // x resolution, 72dpi
	*(int*)p = 2835; p += 4;     // y resolution
	*(int*)p = 0; p += 4;        // number of used palette colors: max
	*(int*)p = 0; p += 4;        // important pixels

	fwrite(buf, 1, 54, f);
}

void
BmpWritePalette(Palette *pal, FILE *f)
{
	int i;
	uchar c[4];
	for(i = 0; i < 256; i++){
		c[0] = pal->col[i].b;
		c[1] = pal->col[i].g;
		c[2] = pal->col[i].r;
		c[3] = 255;
		fwrite(c, 1, 4, f);
	}
}

void
BmpFlipVert(int w, int h, int d, uchar *data)
{
	int s;
	char *new;
	int y;
	s = BmpGetStride(w, d);
	new = malloc(h*s);
	for(y = 0; y < h; y++)
		memcpy(new+y*s, data+(h-1-y)*s, s);
	memcpy(data, new, h*s);
	free(new);
}
