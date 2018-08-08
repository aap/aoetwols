#include "aok.h"

enum
{
	SLOPE_FLAT,
	SLOPE_S_UP,
	SLOPE_N_UP,
	SLOPE_W_UP,
	SLOPE_E_UP,
	SLOPE_SW_UP,
	SLOPE_NW_UP,
	SLOPE_SE_UP,
	SLOPE_NE_UP,
	SLOPE_S_UP2,	// what are these?
	SLOPE_N_UP2,
	SLOPE_W_UP2,
	SLOPE_E_UP2,
	SLOPE_N_DOWN,
	SLOPE_S_DOWN,
	SLOPE_W_DOWN,
	SLOPE_E_DOWN,

	NUM_SLOPES,

	SLOPE_SWE_UP = SLOPE_N_DOWN,
	SLOPE_NWE_UP = SLOPE_S_DOWN,
	SLOPE_NSE_UP = SLOPE_W_DOWN,
	SLOPE_NSW_UP = SLOPE_E_DOWN,
};

typedef struct Neighbor Neighbor;
struct Neighbor
{
	int dx, dy, bit;
};

enum
{
	TILE_W,
	TILE_N,
	TILE_E,
	TILE_S,
	TILE_NW,
	TILE_NE,
	TILE_SE,
	TILE_SW,
};

enum
{
	TILEBIT_W = 1<<TILE_W,
	TILEBIT_N = 1<<TILE_N,
	TILEBIT_E = 1<<TILE_E,
	TILEBIT_S = 1<<TILE_S,
	TILEBIT_NW = 1<<TILE_NW,
	TILEBIT_NE = 1<<TILE_NE,
	TILEBIT_SE = 1<<TILE_SE,
	TILEBIT_SW = 1<<TILE_SW,
};

Neighbor neighbors[8] = {
	{ -1, -1, TILEBIT_W},
	{  1, -1, TILEBIT_N},
	{  1,  1, TILEBIT_E},
	{ -1,  1, TILEBIT_S},
	{  0, -1, TILEBIT_NW},
	{  1,  0, TILEBIT_NE},
	{  0,  1, TILEBIT_SE},
	{ -1,  0, TILEBIT_SW},
};

static uchar
ViewLightSlope(View *view, ushort idx, int *patterns, int numpatterns)
{
	uchar res, icm, pix;
	if(numpatterns <= 0)
		return 4;
	pix = view->patternmasks[(int)*patterns++][idx];
	res = (pix >> 2) & 0x1F;
	while(--numpatterns){
		pix = view->patternmasks[(int)*patterns++][idx];
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

static void
ViewFilterTile(View *view, FilterMap *filter, Shape *dst, Shape *src, int nf, int *patterns, int numpatterns)
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
			icm = ViewLightSlope(view, lightidx, patterns, numpatterns);
			*lp++ = view->icm[icm]->map[r>>11][g>>11][b>>11];
		}
		*lp = 0x0F;	// end of line
	}
}

static int
ViewGetTerrainFrame(View *view, int terrain, int x, int y)
{
	Terrain *terr;
	terr = &view->map->terrain[terrain];
	return (y % terr->num_tiles_y)*terr->num_tiles_x + (x % terr->num_tiles_x);
}

#include "viewlight.inc.c"

static void
ViewBlendTile(View *view, Shape *dst, int dstfrm,
	Shape *src1, int src1frm,
	Shape *src2, int src2frm,
	int inflbits, int mode, int variation)
{
	int i;
	int x, y;
	char *maps;
	Blendmode *bm;
	int nummasks;
	uchar *masks[4];
	int *width;
	int *dstoff, *src1off, *src2off;
	uchar *dstdata, *src1data, *src2data;
	int alpha;
	RGB rgb1, rgb2;
	int r, g, b;

	maps = view->blendInflMap[inflbits];
	bm = &view->blend_modes[mode];
	nummasks = 0;
	for(i = 0; i < 4; i++){
		if(maps[i] != -1){
			if(maps[i] < 16)
				masks[nummasks++] = bm->tilealpha[maps[i]+variation];
			else
				masks[nummasks++] = bm->tilealpha[maps[i]];
		}
	}
//assert(nummasks);
	dstoff = ShapeGetCommandTbl(dst, dstfrm);
	src1off = ShapeGetCommandTbl(src1, src1frm);
	src2off = ShapeGetCommandTbl(src2, src2frm);
assert(dst->frames[0].width == 97);
assert(dst->frames[0].height == 49);
assert(src1->frames[0].width == 97);
assert(src1->frames[0].height == 49);
assert(src2->frames[0].width == 97);
assert(src2->frames[0].height == 49);
	width = view->baseTileLineWidth;
	for(y = 0; y < 49; y++){
		dstdata = ShapeGetCommand(dst, dstoff[y]);
		src1data = ShapeGetCommand(src1, src1off[y]);
		src2data = ShapeGetCommand(src2, src2off[y]);
		if(width[y] <= 0x3f){
			*dstdata++ = width[y]<<2;	// small write
			src1data++;
			src2data++;
		}else{
			*dstdata++ = 0x2;		// write
			*dstdata++ = width[y];
			src1data += 2;
			src2data += 2;
		}
		for(x = 0; x < width[y]; x++){
			alpha = 0x80;
			for(i = 0; i < nummasks; i++){
				if(*masks[i] < alpha)
					alpha = *masks[i];
				masks[i]++;
			}
			if(alpha == 0x80)
				*dstdata = *src1data;
			else if(alpha == 0)
				*dstdata = *src2data;
			else{
				rgb1 = view->mainPalette.col[*src1data];
				rgb2 = view->mainPalette.col[*src2data];
				r = rgb1.r * alpha + rgb2.r *(0x80 - alpha);
				g = rgb1.g * alpha + rgb2.g *(0x80 - alpha);
				b = rgb1.b * alpha + rgb2.b *(0x80 - alpha);
				*dstdata = view->icm[9]->map[r>>10][g>>10][b>>10];
			}
			dstdata++;
			src1data++;
			src2data++;
		}
		*dstdata = 0x0F;
	}
}

static void
ViewDrawTile(View *view, int tilex, int tiley, int xoff, int yoff)
{
	int i, j;
	int x, y;
	Map *map;
	Tile *tile;

	int trntype, trntype_n;
	Terrain *terr, *baseterr;
	Terrain *tn;

	Shape *shape;
	int frm;
	int slope;
	int slopes[8];
	int patterns[20];
	int numpatterns;

	int blendinfo[8];
	int levterr;
	int numBlend;
	int minblend, b;
	int inflbits;
	Shape *src1, *src2;
	int src1frm, src2frm;
	int mode;

	static int blendModes[8][8] = {
		{ 2, 3, 2, 1, 1, 6, 5, 4 },
		{ 3, 3, 3, 1, 1, 6, 5, 4 },
		{ 2, 3, 2, 1, 1, 6, 1, 4 },
		{ 1, 1, 1, 0, 7, 6, 5, 4 },
		{ 1, 1, 1, 7, 7, 6, 5, 4 },
		{ 6, 6, 6, 6, 6, 6, 5, 4 },
		{ 5, 5, 1, 5, 5, 5, 5, 4 },
		{ 4, 3, 4, 4, 4, 4, 4, 4 }
	};

	map = view->map;
	tile = &map->tiles[tiley][tilex];
	trntype = tile->terrain;
	terr = &map->terrain[trntype];
	if(terr->replacement_terrain != -1)
		trntype = terr->replacement_terrain;
	baseterr = &map->terrain[trntype];
	slope = tile->slope;

	shape = baseterr->shape;
	frm = ViewGetTerrainFrame(view, trntype, tilex, tiley);

	// find blend neighbors
	numBlend = 0;
	for(i = 0; i < 8; i++){
		x = tilex + neighbors[i].dx;
		y = tiley + neighbors[i].dy;
		if(!MapValidCoord(map, x, y))
			continue;
		trntype_n = map->tiles[y][x].terrain;
		tn = &map->terrain[trntype_n];
		if(tn->replacement_terrain != -1)
			trntype_n = tn->replacement_terrain;
		tn = &map->terrain[trntype_n];
		// Found neighbor that's blending into us,
		// add blend info
		if(trntype_n != trntype &&
		   tn->blend_level >= baseterr->blend_level){
			levterr = (tn->blend_level << 8) | trntype_n;
			for(j = 0; j < numBlend; j++){
				if(blendinfo[j] == levterr)
					goto found;
			}
			blendinfo[numBlend++] = levterr;
	found:;
		}
	}

	// do the blending
	for(i = 0; i < numBlend; i++){
		// find lowest priority blender
		minblend = INT_MAX;
		for(j = 0; j < numBlend; j++){
			if(blendinfo[j] < minblend){
				minblend = blendinfo[j];
				b = j;
			}
		}
		// get influence bits for this terrain
		inflbits = 0;
		for(j = 0; j < 8; j++){
			x = tilex + neighbors[j].dx;
			y = tiley + neighbors[j].dy;
			if(!MapValidCoord(map, x, y))
				continue;
			trntype_n = map->tiles[y][x].terrain;
			tn = &map->terrain[trntype_n];
			if(tn->replacement_terrain != -1)
				trntype_n = tn->replacement_terrain;
			if(trntype_n == (blendinfo[b]&0xFF))
				inflbits |= neighbors[j].bit;
		}
		if(i == 0){
			src1 = baseterr->shape;
			src1frm = ViewGetTerrainFrame(view, trntype, tilex, tiley);
		}else{
			src1 = view->tmpFlatTile;
			src1frm = 0;
		}
		trntype_n = blendinfo[b]&0xFF;
		src2 = map->terrain[trntype_n].shape;
		src2frm = ViewGetTerrainFrame(view, trntype_n, tilex, tiley);
		mode = blendModes[baseterr->blend_mode][map->terrain[trntype_n].blend_mode];

		ViewBlendTile(view, view->tmpFlatTile, 0,
			src1, src1frm, src2, src2frm, inflbits, mode, (tilex+tiley) & 3);

		blendinfo[b] = INT_MAX;

		shape = view->tmpFlatTile;
		frm = 0;
	}

	// light it!
	if(slope){
		for(i = 0; i < 8; i++){
			x = tilex + neighbors[i].dx;
			y = tiley + neighbors[i].dy;
			if(MapValidCoord(map, x, y))
				slopes[i] = map->tiles[y][x].slope;
			else
				slopes[i] = -1;
		}

		numpatterns = 0;
		switch(slope){
		case SLOPE_S_UP:
		case SLOPE_S_UP2:
			numpatterns = ViewLightSlope_S_UP(view, slopes, patterns);
			break;
		case SLOPE_N_UP:
		case SLOPE_N_UP2:
			numpatterns = ViewLightSlope_N_UP(view, slopes, patterns);
			break;
		case SLOPE_W_UP:
		case SLOPE_W_UP2:
			numpatterns = ViewLightSlope_W_UP(view, slopes, patterns);
			break;
		case SLOPE_E_UP:
		case SLOPE_E_UP2:
			numpatterns = ViewLightSlope_E_UP(view, slopes, patterns);
			break;
		case SLOPE_SW_UP:
			numpatterns = ViewLightSlope_SW_UP(view, slopes, patterns);
			break;
		case SLOPE_NW_UP:
			numpatterns = ViewLightSlope_NW_UP(view, slopes, patterns);
			break;
		case SLOPE_SE_UP:
			numpatterns = ViewLightSlope_SE_UP(view, slopes, patterns);
			break;
		case SLOPE_NE_UP:
			numpatterns = ViewLightSlope_NE_UP(view, slopes, patterns);
			break;
		case SLOPE_SWE_UP:
			numpatterns = ViewLightSlope_SWE_UP(view, slopes, patterns);
			break;
		case SLOPE_NWE_UP:
			numpatterns = ViewLightSlope_NWE_UP(view, slopes, patterns);
			break;
		case SLOPE_NSE_UP:
			numpatterns = ViewLightSlope_NSE_UP(view, slopes, patterns);
			break;
		case SLOPE_NSW_UP:
			numpatterns = ViewLightSlope_NSW_UP(view, slopes, patterns);
			break;
		}

		ViewFilterTile(view, view->filtermaps[slope],
			map->tempSlopeSlps[slope], shape, frm, patterns, numpatterns);
		shape = map->tempSlopeSlps[slope];
		frm = 0;
	}

	ShapeDrawFrame(shape, frm, &gGame.screen,
		xoff+tile->xcoord,
		yoff+tile->ycoord - map->tileSize[slope].z - (slope ? 12 : 0)); // wat?
}

void
ViewDraw(View *view, int xoff, int yoff)
{
	int i, j;
	Map *map;
	map = view->map;
	for(i = 0; i < map->height; i++)
		for(j = 0; j < map->width; j++)
			ViewDrawTile(view, j, i, xoff, yoff);
}




void
EdgeRelocate(Edge *edge, int ntile)
{
	int i, j;
	EdgeSlope *slope;

	for(i = 0; i < 17; i++){
		edge->slope[i] = (EdgeSlope*)((int)edge->slope[i] + (int)edge);
		slope = edge->slope[i];
		for(j = 0; j < ntile; j++)
			slope->tile[j] = (EdgeSpan*)((int)slope->tile[j] + (int)edge);
	}
}

void
BlendmodeRead(Blendmode *bm, FILE *f)
{
	int i;
	fread(&bm->tilesize, 1, 4, f);
	for(i = 0; i < NUMTILES; i++){
		fread(&bm->tileflags[i], 1, 1, f);
		bm->tilealpha[i] = NULL;
	}
	bm->tilebits = malloc(4*bm->tilesize);
	fread(bm->tilebits, 1, 4*bm->tilesize, f);
	for(i = 0; i < NUMTILES; i++){
		if(bm->tileflags[i]){
			bm->tilealpha[i] = malloc(bm->tilesize);
			fread(bm->tilealpha[i], 1, bm->tilesize, f);
		}
	}
}

int
BlendomaticRead(Blendmode *bmat, FILE *f)
{
	int ntiles, nummodes;
	int i;

	fread(&nummodes, 1, 4, f);
	assert(nummodes <= MAXNUMBLENDMODES);
	fread(&ntiles, 1, 4, f);
	assert(ntiles == NUMTILES);

	for(i = 0; i < nummodes; i++)
		BlendmodeRead(&bmat[i], f);
	return nummodes;
}

void
ViewInitBlendMaps(View *view)
{
	int i, j;
	int nummaps;
	char maps[4];
	int tileN, tileS, tileW, tileE;

	view->blendInflMap = malloc(256*4);
	memset(view->blendInflMap, 0, 256*4);
	for(i = 0; i <= 0xFF; i++){
		tileN = i & TILEBIT_N;
		tileS = i & TILEBIT_S;
		tileW = i & TILEBIT_W;
		tileE = i & TILEBIT_E;
		if(i & TILEBIT_NW){
			tileN = 0;
			tileW = 0;
		}
		if(i & TILEBIT_NE){
			tileN = 0;
			tileE = 0;
		}
		if(i & TILEBIT_SE){
			tileS = 0;
			tileE = 0;
		}
		if(i & TILEBIT_SW){
			tileS = 0;
			tileW = 0;
		}
		maps[0] = -1;
		maps[1] = -1;
		maps[2] = -1;
		maps[3] = -1;
		nummaps = 0;
		if(tileE)
			maps[nummaps++] = 16;
		if(tileS)
			maps[nummaps++] = 17;
		if(tileN)
			maps[nummaps++] = 18;
		if(tileW)
			maps[nummaps++] = 19;
		switch(i & 0xF0){
		case TILEBIT_SE:
			maps[nummaps++] = 0;
			break;
		case TILEBIT_NE:
			maps[nummaps++] = 4;
			break;
		case TILEBIT_SW:
			maps[nummaps++] = 8;
			break;
		case TILEBIT_NW:
			maps[nummaps++] = 12;
			break;
		case TILEBIT_NE | TILEBIT_SW:
			maps[nummaps++] = 20;
			break;
		case TILEBIT_NW | TILEBIT_SE:
			maps[nummaps++] = 21;
			break;
		case TILEBIT_NW | TILEBIT_SW:
			maps[nummaps++] = 22;
			break;
		case TILEBIT_NW | TILEBIT_NE:
			maps[nummaps++] = 23;
			break;
		case TILEBIT_SW | TILEBIT_SE:
			maps[nummaps++] = 24;
			break;
		case TILEBIT_NE | TILEBIT_SE:
			maps[nummaps++] = 25;
			break;
		case TILEBIT_NE | TILEBIT_SE | TILEBIT_SW:
			maps[nummaps++] = 26;
			break;
		case TILEBIT_NW | TILEBIT_SE | TILEBIT_SW:
			maps[nummaps++] = 27;
			break;
		case TILEBIT_NW | TILEBIT_NE | TILEBIT_SW:
			maps[nummaps++] = 28;
			break;
		case TILEBIT_NW | TILEBIT_NE | TILEBIT_SE:
			maps[nummaps++] = 29;
			break;
		case TILEBIT_NW | TILEBIT_NE | TILEBIT_SE | TILEBIT_SW:
			maps[nummaps++] = 30;
			break;
		}
		for(j = 0; j < 4; j++)
			view->blendInflMap[i][j] = maps[j];
	}
}

void
ViewLoadFiles(View *view)
{
	FILE *f;
	int i;
	int size;
	uchar *data;
	SlpTemplate *slope;

	assert(sizeof(void*) == 4);
	assert(sizeof(EdgeSpan) == 3);

	data = getFileContents("data/TileEdge.dat", NULL);
	view->tile_edge = (Edge*)data;
	EdgeRelocate(view->tile_edge, 94);

	data = getFileContents("data/BlkEdge.dat", NULL);
	view->blk_edge = (Edge*)data;
	EdgeRelocate(view->blk_edge, 47);

	data = getFileContents("data/view_icm.dat", NULL);
	view->icm[0] = (Icm*)data;
	for(i = 0; i < 9; i++)
		view->icm[i+1] = view->icm[i]+1;

	f = mustopen("data/FilterMaps.dat", "rb");
	for(i = 0; i < 17; i++){
		fread(&size, 1, 4, f);
		view->filtermaps[i] = malloc(size);
		fread(view->filtermaps[i], 1, size, f);
	}
	fclose(f);

	f = mustopen("data/LoQMaps.dat", "rb");
	for(i = 0; i < 17; i++){
		fread(&size, 1, 4, f);
		view->loqmaps[i] = malloc(size);
		fread(view->loqmaps[i], 1, size, f);
	}
	fclose(f);

	f = mustopen("data/STemplet.dat", "rb");
	for(i = 0; i < 17; i++){
		fread(&size, 1, 4, f);
		slope = malloc(size);
		view->stemplet[i] = slope;
		fread(slope, 1, size, f);
		slope->outlineTbl = (short*)((int)slope->outlineTbl + (int)slope);
		slope->cmdOffsetTbl = (int*)((int)slope->cmdOffsetTbl + (int)slope);
	}
	fclose(f);

	f = mustopen("data/blendomatic.dat", "rb");
	BlendomaticRead(view->blend_modes, f);
	fclose(f);

	f = mustopen("data/lightMaps.dat", "rb");
	for(i = 0; i < 18; i++){
		fread(&size, 1, 4, f);
		view->lightmaps[i] = malloc(size);
		fread(view->lightmaps[i], 1, size, f);
	}
	fclose(f);

	f = mustopen("data/PatternMasks.dat", "rb");
	for(i = 0; i < 40; i++){
		fread(&size, 1, 4, f);
		view->patternmasks[i] = malloc(size);
		fread(view->patternmasks[i], 1, size, f);
	}
	fclose(f);

	size = 1;
	for(i = 0; i < 25; i++){
		view->baseTileLineWidth[i] = size;
		view->baseTileLineWidth[48-i] = size;
		size += 4;
	}

	view->tmpFlatTile = ShapeCreateFromTemplate(view->stemplet[0]);
	ViewInitBlendMaps(view);
}
