#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <SDL2/SDL.h>

#define nil NULL
// pointer size. only works with 32 bit pointers!!
#define PTRSZ 4

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct Zfile Zfile;

typedef struct RGB RGB;
typedef struct HSV HSV;
typedef struct Palette Palette;
typedef struct Icm Icm;
typedef struct Drs Drs;
typedef struct DrsHeader DrsHeader;
typedef struct DrsTable DrsTable;
typedef struct DrsFile DrsFile;
typedef struct Shape Shape;
typedef struct SlpHeader SlpHeader;
typedef struct SlpFrame SlpFrame;
typedef struct SlpTemplate SlpTemplate;
typedef struct FilterMap FilterMap;
typedef struct Blendmode Blendmode;
typedef struct Edge Edge;
typedef struct EdgeSlope EdgeSlope;
typedef struct EdgeSpan EdgeSpan;
typedef struct Surface Surface;
typedef struct View View;
typedef struct Game Game;


#define NUMTERRAINS 42

#define MAXNUMBLENDMODES 9
#define NUMTILES 31

#define TILEWIDTH 97
#define TILEHEIGHT 49

Zfile *zfopen(const char *path, const char *mode);
void zfclose(Zfile *zf);
int zread(Zfile *zf, void *dst, int n);
int ztell(Zfile *zf);

/* An 8 bit surface */
struct Surface
{
	int width, height, stride;
	uchar *data;
};
Surface SurfaceSubsurf(Surface *s, int x, int y);

struct RGB
{
	uchar r, g, b;
};

struct HSV
{
	double h, s, v;
};

HSV Rgb2Hsv(RGB rgb);
RGB Hsv2Rgb(HSV hsv);

struct Palette
{
	RGB col[256];
};
void PaletteMakeGreyscale(Palette *pal);
void PaletteReadJASC(Palette *pal, FILE *f);
void PaletteWriteBmp(Palette *pal, FILE *f);
uchar PaletteFindColor(Palette *pal, RGB col);

struct Icm
{
	uchar map[32][32][32];
};
void IcmReadBin(Icm *icm, FILE *f);

void BmpWriteHeader(int w, int h, int d, FILE *f);
void BmpWritePalette(Palette *pal, FILE *f);
void BmpFlipVert(int w, int h, int d, uchar *data);
#define BmpGetStride(w, d) \
	((((w)*(d)/8)+3) & ~3)

FILE *mustopen(const char *path, const char *mode);
uchar *getFileContents(const char *path, int *length);

#define CHECKARGC(_n_) \
	if(argc < (_n_)+1){\
		fprintf(stderr, "need %d args\n", _n_);\
		exit(1);\
	}

struct DrsHeader
{
	char comment[40];
	char version[4];
	char game[12];
	int numTables;
	int headerSize;
};

struct DrsTable
{
	uint type;
	int fileOffset;
	int numFiles;
};

struct DrsFile
{
	int id;
	int offset;
	int size;
};

struct Drs
{
	DrsHeader *header;
	DrsTable *tables;
	FILE *file;
	Drs *next;
};
Drs *DrsOpen(const char *path);
void DrsDump(Drs *drs);
uchar *DrsLoadFile(uint type, int id);
FILE *DrsGetFilePointer(uint type, int id);
#define DrsGetFileTable(drs, n) ((DrsFile*)((char*)drs->header + drs->tables[n].fileOffset))

enum {
	DrsSlp = 0x736C7020,	// 'slp '
	DrsBin = 0x62696e61,	// 'bina'
	DrsWav = 0x77617620,	// 'wav '
};

struct SlpHeader
{
	char version[4];
	int numFrames;
	char comment[24];
};

struct SlpFrame
{
	int cmdTableOffset;
	int outlineTableOffset;
	int paletteOffset;
	int properties;
	int width;
	int height;
	int centerX;
	int centerY;
};

struct Shape
{
	SlpHeader *header;
	SlpFrame *frames;
};

#define ShapeGetOutlineTbl(slp, n) (short*)((char*)slp->header + slp->frames[n].outlineTableOffset)
#define ShapeGetCommandTbl(slp, n) (int*)((char*)slp->header + slp->frames[n].cmdTableOffset)
#define ShapeGetCommand(slp, n) (uchar*)((char*)slp->header + n)

Shape *ShapeCreate(uchar *data);
Shape *ShapeCreateFromTemplate(SlpTemplate *templ);
void ShapeDump(Shape *slp);
//void SlpDrawFrame(Shape *slp, int n, uchar *data, int stride);
void ShapeDrawFrame(Shape *slp, int n, Surface *s, int x, int y);
void ShapeDrawArea(Shape *slp, int n, uchar *data, int stride);
void ShapeDumpFrame(const char *path, Palette *pal, Shape *slp, int n);

struct FilterMap
{
	int height;
	uchar cmds[1];
};

struct Blendmode
{
	int tilesize;
	int *tilebits;	// int tilebits[tilesize]

	uchar *tilealpha[NUMTILES];	// uchar tilealpha[31][tilesize]
	uchar tileflags[NUMTILES];	// hasAlpha ^
};

struct Edge
{
	EdgeSlope *slope[17];
};

struct EdgeSlope
{
	EdgeSpan *tile[94];	// or 47
};

struct EdgeSpan
{
	char y, x0, x1;
};

struct SlpTemplate
{
	int width;
	int height;
	int centerX;
	int centerY;
	int dataSize;
	short *outlineTbl;
	int *cmdOffsetTbl;
};


















typedef struct Terrain_Setting2 Terrain_Setting2;
typedef struct Color_Table Color_Table;
typedef struct Sound_File Sound_File;
typedef struct Sound Sound;
typedef struct Sprite Sprite;
typedef struct Sprite_Hook Sprite_Hook;
typedef struct Angle_Sound Angle_Sound;
typedef struct Tile_Size Tile_Size;
typedef struct List_Link List_Link;
typedef struct Tile Tile;
typedef struct TerrainSlopeInfo TerrainSlopeInfo;
typedef struct Terrain Terrain;
typedef struct Terrain_Border Terrain_Border;
typedef struct Map Map;
typedef struct World World;

typedef struct TShape TShape;


struct Terrain_Setting2
{
	int exit;
	int enter;
	int walk;
	int rate;
};

struct Color_Table
{
//	int vtbl;
	int id;
	int statistics_text;
	uchar *colormap;
	int palette_base;
	int outline_color;
	int unused1;
	int unused2;
	int minimap_color;
	int unused3;
	int unused4;
//	int field_2C;
//	int field_30;
//	int field_34;
};

struct Sound_File
{
	char filename[13];
	int res_id;
	short probability;
	short civilisation;
	short playerid;	// wat?
	// more runtime things...
};

struct Sound
{
	short play_delay;	// ??
	short num_files;
	Sound_File *file_list;
	int cache_time;		// ??
	short id;
};

struct Sprite_Hook
{
	short sprite_id;
	Sprite *sprite;
	short off_x;
	short off_y;
	short angle;
};

struct Angle_Sound
{
	Sound *sound[3];
	short delay[3];
};

// RGE_Sprite
struct Sprite
{
	char filename[13];
	int resource_id;
	uchar facet_thing;
	Color_Table **color_tables;
	int facet_thing2;
	int field_20;
	int field_24;
	uchar unk2;
	uchar layer;
	short color_map;
	uchar replay;
	short bounding_box[4];
	short num_hooks;
	Sprite_Hook *hooks;
	Sound *sound;
	uchar has_angle_sounds;
	Angle_Sound *angle_sounds;
	char name[21];
	short frames_per_angle;
	short num_angles;
	float speed_adjust;
	float duration;
	float replay_delay;
	uchar sequence_type;
	short id;
	uchar mirror_mode;
	uchar unk3;
};

struct Tile_Size
{
	short width;
	short height;
	short z;
};

struct List_Link
{
	void *data;
	List_Link *next;
};

// RGE_Tile
struct Tile
{
	short xcoord;
	short ycoord;
	uchar slope;
	uchar terrain;
	uchar elevation;
	char field_7;
	char field_8;
	char field_9;
	char field_A;
	char field_B;
	char field_C;
	uchar num_foo;
	char field_E;
	char field_F;
	Shape *shape;
	int *foo_tbl;
	char field_18;
	char field_19;
	char field_1A;
	char field_1B;
	char field_1C;
	char field_1D;
	char field_1E;
	char field_1F;
};

struct TerrainSlopeInfo
{
	short num_frames;
	short num_angles;
	short start_frame;
};

struct Terrain
{
	uchar enabled;
	uchar unk0;
	char name[13];
	char filename[13];
	int resource_id;
	Shape *shape;
	Sound *sound;	// saved as id in .dat
	int blend_level;
	int blend_mode;
	uchar map_col_hi;
	uchar map_col_mid;
	uchar map_col_low;
	uchar map_col_cliff_left;
	uchar map_col_cliff_right;
	char passable;
	char impassable;

	uchar is_animated;
	short anim_frame_count;
	short pauseFrameCount;
	float interval;
	float pause_after_loop;
	short frame;
	short drawFrame;
	float animateLast;
	uchar frameChanged;
	uchar drawn;

	TerrainSlopeInfo slopeinfo[19];
	short replacement_terrain;
	short num_tiles_x;
	short num_tiles_y;
	short borders[NUMTERRAINS];
	short object_id[30];
	short object_density[30];
	char object_flag[30];
	short num_objects;
	short unk2;
};

struct Terrain_Border
{
	uchar enabled;
	uchar unk0;
	char name[13];
	char filename[13];
	int resource_id;
	TShape *slp;
	Sound *sound;	// saved as id in .dat
	uchar map_col_hi;
	uchar map_col_mid;
	uchar map_col_low;

	uchar is_animated;
	short anim_frame_count;
	short pauseFrameCount;
	float interval;
	float pause_after_loop;
	short frame;
	short drawFrame;
	float animateLast;
	uchar frameChanged;
	uchar drawn;

	TerrainSlopeInfo slopeinfo[12*19];
	uchar draw_tile;
	short base_terrain;
	short border_style;
};

// RGE_Map
struct Map
{
	int vtbl;
	Tile *tile_data;
	int width;
	int height;
	int worldWidth;
	int worldHeight;
	Tile_Size tileSize[19];
	Terrain terrain[NUMTERRAINS];
	Terrain_Border borders[16];
	Tile **tiles;
	float minx;
	float miny;
	float maxx;
	float maxy;
	float sizex;
	float sizey;
	short terrainsUsed;
	short bordersUsed;
	short maxTerrain;
	short tileWidth;
	short tileHeight;
	short tileHalfWidth;
	short tileHalfHeight;
	short elevHeight;
	short curRow;
	short curCol;
	short blockBeginRow;
	short blockEndRow;
	short blockBeginCol;
	short blockEndCol;
	uchar *tileflag_data;
	uchar **tileflags;
	char field_A254;
	uchar visible;
	uchar fog;
	char field_A257;
	int field_A258;
	int field_A25C;
	int field_A260;
	int field_A264;
	int someObject4;
	void *land_maker;	//?
	World *world;
	int someObject1;
	int someObject2;
	int someObject3;
	int field_A280;
	int field_A284;
	int field_A288;
	int field_A28C;
	int field_A290;
	int field_A294;
	int field_A298;
	int field_A29C;
	int field_A2A0;
	int field_A2A4;
	int field_A2A8;
	int field_A2AC;
	int field_A2B0;
	int field_A2B4;
	int field_A2B8;
	int field_A2BC;
	int field_A2C0;
	int field_A2C4;
	int field_A2C8;
	int field_A2CC;
	int field_A2D0;
	int field_A2D4;
	int field_A2D8;
	int field_A2DC;
	int field_A2E0;
	int field_A2E4;
	int field_A2E8;
	int field_A2EC;
	int field_A2F0;
	int field_A2F4;
	int field_A2F8;
	int field_A2FC;
	int field_A300;
	int field_A304;
	int field_A308;
	int field_A30C;
	int field_A310;
	int field_A314;
	int field_A318;
	int field_A31C;
	int field_A320;
	int field_A324;
	int field_A328;
	int field_A32C;
	int field_A330;
	int field_A334;
	int field_A338;
	int field_A33C;
	int field_A340;
	int field_A344;
	int field_A348;
	int field_A34C;
	int field_A350;
	int field_A354;
	int field_A358;
	int field_A35C;
	int field_A360;
	int field_A364;
	int field_A368;
	int field_A36C;
	int field_A370;
	int field_A374;
	int field_A378;
	int field_A37C;
	int field_A380;
	int field_A384;
	int field_A388;
	int field_A38C;
	int field_A390;
	int field_A394;
	int field_A398;
	int field_A39C;
	int field_A3A0;
	int field_A3A4;
	int field_A3A8;
	int field_A3AC;
	int field_A3B0;
	int field_A3B4;
	int field_A3B8;
	View *view;
	int useTempSlopeSlp;
	List_Link *freeLinkList;
	int listLinkLimit;
	int numListLinks;
	List_Link *slopeTileListHead[17];
	List_Link *slopeTileListTail[17];
	List_Link *slopeSlpList[17];
	Shape *tempSlopeSlps[17];
};
void MapInit(Map *map);
void MapNew(Map *map, int width, int height);
void MapSetTerrain(Map *map, int x0, int y0, int x1, int y1, int terrain);
void MapSetElev(Map *map, int x, int y, int elev);
int MapValidCoord(Map *map, int x, int y);

// Not RGE_World
struct World
{
	Map *map;

	short num_sounds;
	Sound **sounds;

	short num_sprites;
	Sprite **sprites;

	short num_terrain_settings;
	short num_terrains;
	float **terrain_settings1;
	Terrain_Setting2 **terrain_settings2;

	short num_color_tables;
	Color_Table **color_tables;
};
World *WorldCreate(const char *datpath);
void WorldDump(World *world);

// Not RGE_View
struct View
{
	Map *map;

	Edge *tile_edge;
	Edge *blk_edge;
	Icm *icm[10];
	SlpTemplate *stemplet[17];
	FilterMap *filtermaps[17];
	FilterMap *loqmaps[17];
	uchar *lightmaps[18];
	uchar *patternmasks[40];
	Blendmode blend_modes[9];
	int baseTileLineWidth[49];
	int *blendInflBits;
	char (*blendInflMap)[4];
	Shape *tmpFlatTile;

	Palette mainPalette;
};
void ViewLoadFiles(View*);
void ViewDraw(View *view, int xoff, int yoff);

// Not RGE_Base_Game
struct Game
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *screentex;
	Surface screen;
	uint *screentmp;
	uint sdlPalette[256];
	View *view;
	World *world;
};
int GameInitSDL(Game*);
void GameMainLoop(Game*);
void GameMakeSDLPalette(Game *game, Palette *palette);

extern Game gGame;