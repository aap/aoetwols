#include "aok.h"
#include <unistd.h>

Game gGame;

char lighting[][10] = {
	{ 0, 4, 12, 34, -1 },
	{ 0, 5, 28, -1 },
	{ 2, 5, 10, 17, 21, 22, -1 }
};

uchar
GetLighting(View *view, ushort idx, int patternidx)
{
	char *pattern;
	uchar res, icm, pix;
	if(patternidx < 0)
		return 4;
	pattern = lighting[patternidx];
	pix = view->patternmasks[(int)*pattern++][idx];
	res = (pix >> 2) & 0x1F;
	while(*pattern >= 0){
		pix = view->patternmasks[(int)*pattern++][idx];
		icm = pix>>2;
		if(!(pix & 1)){
			if(pix & 2){
				if(icm > res)
					res = icm;
			}else{
				if(icm < res)
					res = icm;
			}
		}
	}
	return view->lightmaps[res&0x1F][idx];
}

void
FilterTile(View *view, FilterMap *filter, Shape *dst, Shape *src, int nf, int pattern)
{
	uchar *cmd;
	uchar width;
	uchar *lp;
	uchar *srcdata;
	int y;
	int *off;
	int r, g, b;
	uint pix;
	uint alpha;
	uint icm;;
	RGB col;
	ushort lightidx, n;

	assert(dst->frames[0].height == filter->height);
	srcdata = ShapeGetCommand(src, *ShapeGetCommandTbl(src, nf));
	cmd = filter->cmds;
	off = ShapeGetCommandTbl(dst, 0);
	for(y = 0; y < filter->height; y++){
		lp = ShapeGetCommand(dst, off[y]);
		width = *cmd++;
		if(width <= 0x3f)
			*lp++ = width<<2;	// small write
		else{
			*lp++ = 0x2;		// write
			*lp++ = width;
		}
		while(width--){
			lightidx = *(ushort*)cmd;
			cmd += 2;
			n = lightidx & 0xF;
			lightidx >>= 4;
			r = g = b = 0;
			while(n--){
				pix = *(uint*)(cmd) & 0xFFFFFF;
				cmd += 3;
				alpha = pix & 0x1FF;
				pix >>= 9;
				col = view->mainPalette.col[srcdata[pix]];
				r += (int)col.r * alpha;
				g += (int)col.g * alpha;
				b += (int)col.b * alpha;
			}
			icm = GetLighting(view, lightidx, pattern);
			*lp++ = view->icm[icm]->map[r>>11][g>>11][b>>11];
		}
		*lp = 0x0F;	// end of line
	}
}

void
slopetest(View *view, Shape *slp)
{
	Shape *testslope[17];
	int i, j;
	FILE *f;
	Surface s;

	for(i = 0; i < 17; i++){
		testslope[i] = ShapeCreateFromTemplate(view->stemplet[i]);
//		FilterTile(view->filtermaps[i], testslope[i], slp, 0, 2);
	}

//	width = 97;
	s.width = 97 * 4;
	s.height = 73*17;

	s.stride = BmpGetStride(s.width, 8);
	s.data = malloc(s.height*s.stride);
	memset(s.data, 0, s.height*s.stride);

//	for(i = 0; i < 17; i++)
//		SlpDrawFrame(testslope[i], 0, data + 73*i*stride, stride);
	for(i = 0; i < 4; i++)
		for(j = 0; j < 17; j++){
			FilterTile(view, view->filtermaps[j], testslope[j], slp, 0, i-1);
			ShapeDrawFrame(testslope[j], 0, &s, i*97, j*73);
		}

	f = mustopen("data/tools/slopetest.bmp", "wb");
	BmpWriteHeader(s.width, s.height, 8, f);
	BmpWritePalette(&view->mainPalette, f);
	BmpFlipVert(s.width, s.height, 8, s.data);
	fwrite(s.data, 1, s.height*s.stride, f);
}

int
main(int argc, char *argv[])
{
//	int i;
	static View view;
	Map *map;


	CHECKARGC(1);
	chdir(argv[1]);

	assert(sizeof(void*) == 4);

	DrsOpen("data/sounds_x1.drs");
	DrsOpen("data/gamedata_x1.drs");
	DrsOpen("data/gamedata_x1_p1.drs");
	DrsOpen("data/sounds.drs");
	DrsOpen("data/graphics.drs");
	DrsOpen("data/terrain.drs");
	DrsOpen("data/interfac.drs");
	DrsOpen("data/gamedata.drs");

	gGame.screen.width = 1024;
	gGame.screen.height = 768;
	gGame.view = &view;
	if(GameInitSDL(&gGame))
		return 1;

	gGame.world = WorldCreate("data/empires2_x1_p1.dat");
	map = gGame.world->map;
	map->view = &view;
	view.map = map;
//	WorldDump(gGame.world);
	ViewLoadFiles(&view);

	PaletteReadJASC(&view.mainPalette, DrsGetFilePointer(DrsBin, 50500));
	GameMakeSDLPalette(&gGame, &view.mainPalette);

//	for(i = 0; i < 19; i++)
//		printf("tile %2d: %5d\n", i, map->tileSize[i].z);
//	fflush(stdout);


	MapInit(map);
	MapNew(map, 20, 20);
	MapSetElev(map, 7, 7, 2);

	map->tiles[6][8].slope = 1;
	map->tiles[7][8].slope = 5;
	map->tiles[8][8].slope = 3;

	map->tiles[6][7].slope = 7;
	map->tiles[8][7].slope = 6;

	map->tiles[6][6].slope = 4;
	map->tiles[7][6].slope = 8;
	map->tiles[8][6].slope = 2;

	MapSetTerrain(map, 4, 6, 10, 15, 2);
	MapSetTerrain(map, 9, 9, 15, 15, 7);


//	SlpDrawFrame(slp, 0, &gGame.screen, -50, 100);

//	slopetest(&view, slp);
//	SlpDumpFrame("data/tools/test.bmp", &view.mainPalette, slp, 0);

	GameMainLoop(&gGame);

	return 0;
}

/* vim: set ts=8 sw=8 tw=0 noexpandtab cindent softtabstop=8 :*/
