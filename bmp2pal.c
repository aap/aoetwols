#include <stdio.h>

int
main(int argc, char *argv[])
{
	int i, j, c;
	FILE *f;
	unsigned char pal[256*4];
	if(argc < 3)
		return 1;
	f = fopen(argv[1], "rb");
	fseek(f, 0x46, 0);
	fread(pal, 1, 256*4, f);
	fclose(f);

	f = fopen(argv[2], "w");
	fprintf(f, "JASC-PAL\n0100\n256\n");
	for(i = 0; i < 16; i++)
		for(j = 0; j < 16; j++){
			c = (15-i)*16+j;
			fprintf(f, "%d %d %d\n", pal[c*4+3], pal[c*4+2], pal[c*4+1]);
		}
	fclose(f);
	return 0;
}
