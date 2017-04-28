#include "aokbase.h"
#include <fcntl.h>

void
IcmReadBin(Icm *icm, FILE *f)
{
	fread(icm->map, 1, 32*32*32, f);
}

void
IcmWriteBuffer(Icm *icm, uchar *buf, int stride)
{
	int r, g, b;
	for(r = 0; r < 32; r++)
		for(g = 0; g < 32; g++){
			for(b = 0; b < 32; b++)
				buf[b] = icm->map[r][g][b];
			buf += stride;
		}
}

void
PaletteOnlyValue(Palette *pal)
{
	HSV hsv;
	int i;
	for(i = 0; i < 256; i++){
		hsv = Rgb2Hsv(pal->col[i]);
		hsv.s = 0;
		pal->col[i] = Hsv2Rgb(hsv);
	}
}

void
PaletteOnlySaturation(Palette *pal)
{
	HSV hsv;
	int i;
	for(i = 0; i < 256; i++){
		hsv = Rgb2Hsv(pal->col[i]);
		hsv.v = hsv.s;
		hsv.s = 0;
		pal->col[i] = Hsv2Rgb(hsv);
	}
}

#define NICM 10

int
main(int argc, char *argv[])
{
	int i, s;
	Icm icms[NICM];
	Palette pal;
	FILE *f;
	static uchar buf[32*32*32*NICM];

	setmode(fileno(stdout), O_BINARY);
	setmode(fileno(stdin), O_BINARY);

	CHECKARGC(1);
	f = mustopen(argv[1], "rb");
	PaletteReadJASC(&pal, f);
	fclose(f);

	for(i = 0; i < NICM; i++)
		IcmReadBin(&icms[i], stdin);
	BmpWriteHeader(32*NICM, 32*32, 8, stdout);
//	PaletteOnlySaturation(&pal);
	BmpWritePalette(&pal, stdout);

	s = BmpGetStride(32*NICM, 8);
	for(i = 0; i < NICM; i++)
		IcmWriteBuffer(&icms[i], buf+i*32, s);
	BmpFlipVert(32*NICM, 32*32, 8, buf);
	fwrite(buf, 1, 32*NICM*32*32, stdout);

	return 0;
}
