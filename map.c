#include "aok.h"

void
MapInit(Map *map)
{
	int i;
	Terrain *t;

	for(t = map->terrain; t < &map->terrain[NUMTERRAINS]; t++){
		if(!t->enabled)
			continue;
		t->shape = ShapeCreate(DrsLoadFile(DrsSlp, t->resource_id));
		assert(t->shape);
	}

	map->tileWidth = 96;
	map->tileHeight = 48;
	map->tileHalfWidth = map->tileWidth/2;
	map->tileHalfHeight = map->tileHeight/2;
	map->elevHeight = 24;

	for(i = 0; i < 17; i++)
		map->tempSlopeSlps[i] = ShapeCreateFromTemplate(map->view->stemplet[i]);
}

static void
MapInitTiles(Map *map)
{
	int i, j;
	Tile *t;
	for(i = 0; i < map->height; i++)
		for(j = 0; j < map->width; j++){
			t = &map->tiles[i][j];
			t->xcoord = (i + j)*48;
			t->ycoord = (i - j)*24 - t->elevation*map->elevHeight;
		}
}

static void
MapClear(Map *map, int terrain, int elevation)
{
	int i, j;
	for(i = 0; i < map->height; i++)
		for(j = 0; j < map->width; j++){
			map->tiles[i][j].xcoord = 0;
			map->tiles[i][j].ycoord = 0;
			map->tiles[i][j].terrain = terrain;
			map->tiles[i][j].elevation = elevation;
			map->tiles[i][j].slope = 0;
			map->tiles[i][j].shape = nil;
		}
	MapInitTiles(map);
}

void
MapNew(Map *map, int width, int height)
{
	int i;
	int numtiles;

	map->width = width;
	map->height = height;
	// How does this make sense even? Tile sizes are diagonal, map dimensions not
	map->worldWidth = width*map->tileWidth;
	map->worldHeight = height*map->tileHeight;

	numtiles = width*height;
	map->tile_data = calloc(numtiles, sizeof(Tile));
	map->tiles = calloc(height, PTRSZ);	// game allocates height+1, why?
	for(i = 0; i < height; i++)
		map->tiles[i] = &map->tile_data[i*width];
	MapClear(map, 0, 1);
}

void
MapSetTerrain(Map *map, int x0, int y0, int x1, int y1, int terrain)
{
	int i, j;
	for(i = y0; i < y1; i++)
		for(j = x0; j < x1; j++)
			map->tiles[i][j].terrain = terrain;
}

void
MapSetElev(Map *map, int x, int y, int elev)
{
	map->tiles[y][x].elevation = elev;
	MapInitTiles(map);
}

int
MapValidCoord(Map *map, int x, int y)
{
	return x >= 0 && x < map->width &&
	       y >= 0 && y < map->height;
}
