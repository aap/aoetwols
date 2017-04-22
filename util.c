#include "aok.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

FILE*
mustopen(const char *path, const char *mode)
{
	FILE *f;
	f = fopen(path, mode);
	if(f == NULL){
		fprintf(stderr, "Error: couldn't open file %s\n", path);
		exit(1);
	}
	return f;
}

uchar*
getFileContents(const char *path, int *length)
{
	FILE *f;
	int len;
	uchar *data;

	f = mustopen(path, "rb");
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	rewind(f);
	data = malloc(len);
	fread(data, 1, len, f);
	fclose(f);
	if(length)
		*length = len;
	return data;
}

HSV
Rgb2Hsv(RGB rgb)
{
	double r, g, b;
	double max, min, c;
	HSV hsv;

	r = rgb.r/255.0f;
	g = rgb.g/255.0f;
	b = rgb.b/255.0f;
	max = MAX(r, MAX(g, b));
	min = MIN(r, MIN(g, b));
	c = max - min;

	hsv.v = max;
	if(hsv.v == 0.0){
		hsv.h = 0.0;
		hsv.s = 0.0;
		return hsv;
	}

	hsv.s = c/hsv.v;
	if(hsv.s == 0.0){
		hsv.h = 0.0;
		return hsv;
	}

	if(hsv.v == r)
		hsv.h = (g - b)/c;
	else if(hsv.v == g)
		hsv.h = 2 + (b - r)/c;
	else
		hsv.h = 4 + (r - g)/c;
	hsv.h *= 60;
	while(hsv.h < 0)
		hsv.h += 360;
	while(hsv.h > 360)
		hsv.h -= 360;
	return hsv;
}

RGB
Hsv2Rgb(HSV hsv)
{
	int seg;
	double hh, ff, p, q, t;
	double r, g, b;
	RGB rgb;

	hh = hsv.h / 60.0;
	seg = hh;
	ff = hh - seg;
	p = hsv.v * (1.0 - hsv.s);
	q = hsv.v * (1.0 - (hsv.s * ff));
	t = hsv.v * (1.0 - (hsv.s * (1.0 - ff)));

	switch(seg){
	case 0:
		r = hsv.v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = hsv.v;
		b = p;
		break;
	case 2:
		r = p;
		g = hsv.v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = hsv.v;
		break;
	case 4:
		r = t;
		g = p;
		b = hsv.v;
		break;
	case 5:
		r = hsv.v;
		g = p;
		b = q;
		break;
	}
	if(r > 1.0) r = 1.0;
	if(g > 1.0) g = 1.0;
	if(b > 1.0) b = 1.0;
	rgb.r = r*255;
	rgb.g = g*255;
	rgb.b = b*255;
	return rgb;
}

Surface
SurfaceSubsurf(Surface *s, int x, int y)
{
	Surface ss = { s->width-x, s->height-y, s->stride,
		s->data + y*s->stride + x };
	return ss;
}
