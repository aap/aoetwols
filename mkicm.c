#include "aokbase.h"

static RGB
svmod(RGB rgb, float s, float v)
{
	HSV hsv;
	if(s == 1.0 && v == 1.0)
		return rgb;
	hsv = Rgb2Hsv(rgb);
	hsv.s *= s;
	hsv.v *= v;
	if(hsv.s > 1.0) hsv.s = 1.0;
	if(hsv.v > 1.0) hsv.v = 1.0;
	rgb = Hsv2Rgb(hsv);
	return rgb;
}

void
writeCube(uchar *buf, int stride, float s, float v)
{
	int r, g, b;
	RGB rgb;
	for(r = 0; r < 32; r++){
		for(g = 0; g < 32; g++){
			for(b = 0; b < 32; b++){
				rgb.r = (r<<3) + 2;
				rgb.g = (g<<3) + 2;
				rgb.b = (b<<3) + 2;
				rgb = svmod(rgb, s, v);
				buf[b*3+0] = rgb.b;
				buf[b*3+1] = rgb.g;
				buf[b*3+2] = rgb.r;
			}
			buf += stride;
		}
	}
}

void
writeCubePal(uchar *buf, int stride, float s, float v, Palette *pal)
{
	int r, g, b;
	RGB rgb;
	for(r = 0; r < 32; r++){
		for(g = 0; g < 32; g++){
			for(b = 0; b < 32; b++){
				rgb.r = (r<<3) + 2;
				rgb.g = (g<<3) + 2;
				rgb.b = (b<<3) + 2;
				rgb = svmod(rgb, s, v);
				buf[b] = PaletteFindColor(pal, rgb);
			}
			buf += stride;
		}
	}
}

RGB
RgbOnlyValue(RGB rgb)
{
	HSV hsv;
	hsv = Rgb2Hsv(rgb);
	hsv.s = 0;
	rgb = Hsv2Rgb(hsv);
	return rgb;
}

RGB
RgbOnlySaturation(RGB rgb)
{
	HSV hsv;
	hsv = Rgb2Hsv(rgb);
	hsv.v = hsv.s;
	hsv.s = 0;
	rgb = Hsv2Rgb(hsv);
	return rgb;
}

#define DEPTH 8
#define WIDTH (32*10)
#define HEIGHT (32*32)
#define STRIDE BmpGetStride(WIDTH, DEPTH)

/* usage: mkicm palette.pal view_icm.dat >view_icm.bmp */

int
main(int argc, char *argv[])
{
	int i;
	float s, v;
	uchar buf[HEIGHT*STRIDE];
	Palette pal;
	FILE *fp;

	if(DEPTH == 8){
		CHECKARGC(1);
		fp = mustopen(argv[1], "r");
		PaletteReadJASC(&pal, fp);
		fclose(fp);
		argc--;
		argv++;
	}

	CHECKARGC(1);
	fp = mustopen(argv[1], "wb");

#define STARTV 0.6
#define STARTS 0.8
	float incv = (1.0-STARTV)/4.0;
	float incs = (1.0-STARTS)/4.0;
	s = STARTS;
	v = STARTV;
	for(i = 0; i < 4; i++){
		if(DEPTH == 8)
			writeCubePal(buf + i*32, STRIDE, s, v, &pal);
		else
			writeCube(buf + i*32*3, STRIDE, s, v);
		s += incs;
		v += incv;
	}
	if(DEPTH == 8)
		writeCubePal(buf + i*32, STRIDE, 1.0, 1.0, &pal);
	else
		writeCube(buf + i*32*3, STRIDE, 1.0, 1.0);
	i++;
#define ENDV 1.19
#define ENDS 0.93
	s = 1.0;
	v = 1.0;
	incv = (ENDV-1.0)/4.0;
	incs = (ENDS-1.0)/4.0;
	for(; i < 9; i++){
		s += incs;
		v += incv;
		if(DEPTH == 8)
			writeCubePal(buf + i*32, STRIDE, s, v, &pal);
		else
			writeCube(buf + i*32*3, STRIDE, s, v);
	}
	if(DEPTH == 8)
		writeCubePal(buf + i*32, STRIDE, 1.0, 1.0, &pal);
	else
		writeCube(buf + i*32*3, STRIDE, 1.0, 1.0);

	BmpWriteHeader(WIDTH, HEIGHT, DEPTH, stdout);
	if(DEPTH == 8)
		BmpWritePalette(&pal, stdout);
	BmpFlipVert(WIDTH, HEIGHT, DEPTH, buf);
	fwrite(buf, 1, sizeof(buf), stdout);




	incv = (1.0-STARTV)/4.0;
	incs = (1.0-STARTS)/4.0;
	s = STARTS;
	v = STARTV;
	for(i = 0; i < 4; i++){
		writeCubePal(buf + i*32*32*32, 32, s, v, &pal);
		s += incs;
		v += incv;
	}
	writeCubePal(buf + i*32*32*32, 32, 1.0, 1.0, &pal);
	i++;
	s = 1.0;
	v = 1.0;
	incv = (ENDV-1.0)/4.0;
	incs = (ENDS-1.0)/4.0;
	for(; i < 9; i++){
		s += incs;
		v += incv;
		writeCubePal(buf + i*32*32*32, 32, s, v, &pal);
	}
	writeCubePal(buf + i*32*32*32, 32, 1.0, 1.0, &pal);
	fwrite(buf, 1, 32*32*32*10, fp);
	fclose(fp);



	return 0;
}
