#include "aok.h"



void
dumpTerrainTables(FILE *f, World *world)
{
	int i, j;
	int n;
	int print[100];
	fprintf(f, "%d\n%d\n\n", world->num_terrain_settings, world->num_terrains);
	for(i = 0; i < world->num_terrain_settings; i++){
		n = 0;
		for(j = 0; j < world->num_terrains; j++){
			if(world->terrain_settings1[i][j] ||
			   world->terrain_settings2[i][j].enter ||
			   world->terrain_settings2[i][j].exit ||
			   world->terrain_settings2[i][j].walk ||
			   world->terrain_settings2[i][j].rate){
				print[j] = 1;
				n++;
			}else
				print[j] = 0;
		}

		fprintf(f, "%d %d\n", i, n);
		for(j = 0; j < world->num_terrains; j++){
			if(print[j]){
				fprintf(f, "      %d %f %d %d %d %d\n", j,
					world->terrain_settings1[i][j],
					world->terrain_settings2[i][j].enter,
					world->terrain_settings2[i][j].exit,
					world->terrain_settings2[i][j].walk,
					world->terrain_settings2[i][j].rate);
			}
		}
	}
}

void
readTerrainTables(Zfile *zf, World *world)
{
	int i;

	zread(zf, &world->num_terrain_settings, 2);
	zread(zf, &world->num_terrains, 2);
	world->terrain_settings1 = calloc(world->num_terrain_settings, PTRSZ);
	world->terrain_settings2 = calloc(world->num_terrain_settings, PTRSZ);
	zread(zf, world->terrain_settings1, world->num_terrain_settings*PTRSZ);
	zread(zf, world->terrain_settings2, world->num_terrain_settings*PTRSZ);

	for(i = 0; i < world->num_terrain_settings; i++){
		if(world->terrain_settings1[i]){
			world->terrain_settings1[i] = calloc(world->num_terrains, 4);
			world->terrain_settings2[i] = calloc(world->num_terrains, sizeof(Terrain_Setting2));
			zread(zf, world->terrain_settings1[i], world->num_terrains*4);
			zread(zf, world->terrain_settings2[i], world->num_terrains*sizeof(Terrain_Setting2));

		}
	}
}

void
dumpColorTables(FILE *f, World *world)
{
	int i;
	fprintf(f, "%d %d\n", world->num_color_tables, world->num_color_tables);
	for(i = 0; i < world->num_color_tables; i++){
		fprintf(f, "%-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d\n",
			world->color_tables[i]->id,
			world->color_tables[i]->palette_base,
			world->color_tables[i]->outline_color,
			world->color_tables[i]->unused1,
			world->color_tables[i]->unused2,
			world->color_tables[i]->minimap_color,
			world->color_tables[i]->unused3,
			world->color_tables[i]->unused4,
			world->color_tables[i]->statistics_text);
	}
}

void
readColorTable(Zfile *zf, Color_Table *coltab)
{
	zread(zf, &coltab->id, 4);
	zread(zf, &coltab->palette_base, 4);
	zread(zf, &coltab->outline_color, 4);
	zread(zf, &coltab->unused1, 4);
	zread(zf, &coltab->unused2, 4);
	zread(zf, &coltab->minimap_color, 4);
	zread(zf, &coltab->unused3, 4);
	zread(zf, &coltab->unused4, 4);
	zread(zf, &coltab->statistics_text, 4);
}

void
readColorTables(Zfile *zf, World *world)
{
	int i;

	zread(zf, &world->num_color_tables, 2);
	world->color_tables = calloc(world->num_color_tables, PTRSZ);
	for(i = 0; i < world->num_color_tables; i++){
		world->color_tables[i] = calloc(1, sizeof(Color_Table));
		readColorTable(zf, world->color_tables[i]);
	}
}

void
dumpSounds(FILE *f, World *world)
{
	int i, j;
	Sound_File *s;
	char filename[13];
	fprintf(f, "%d %d\n", world->num_sounds, world->num_sounds);
	for(i = 0; i < world->num_sounds; i++){
		fprintf(f, "%d %d\n",
			world->sounds[i]->id,
			world->sounds[i]->num_files);
		s = world->sounds[i]->file_list;
		for(j = 0; j < world->sounds[i]->num_files; j++){
			memcpy(filename, s->filename, 13);
			filename[strlen(filename)-4] = 0;	// remove .wav
			fprintf(f, "      %d %s %d %d %d\n",
				s->res_id,
				filename,
				s->probability,
				s->civilisation,
				s->playerid);
			s++;
		}
	}
}

void
readSound(Zfile *zf, Sound *s)
{
	int i;
	zread(zf, &s->id, 2);
	zread(zf, &s->play_delay, 2);
	zread(zf, &s->num_files, 2);
	zread(zf, &s->cache_time, 4);
	s->file_list = calloc(s->num_files, sizeof(Sound_File));
	for(i = 0; i < s->num_files; i++){
		zread(zf, &s->file_list[i].filename, 13);
		zread(zf, &s->file_list[i].res_id, 4);
		zread(zf, &s->file_list[i].probability, 2);
		zread(zf, &s->file_list[i].civilisation, 2);
		zread(zf, &s->file_list[i].playerid, 2);
	}
}

void
readSounds(Zfile *zf, World *world)
{
	int i;
	zread(zf, &world->num_sounds, 2);
	world->sounds = calloc(world->num_sounds, PTRSZ);
	for(i = 0; i < world->num_sounds; i++){
		world->sounds[i] = calloc(1, sizeof(Sound));
		readSound(zf, world->sounds[i]);
	}
}

void
dumpSprite(FILE *f, Sprite *spr)
{
	int i, j;
	int soundid, num_anglesounds;

	soundid = spr->sound ? spr->sound->id : -1;
	num_anglesounds = 0;
	if(spr->has_angle_sounds)
		for(i = 0; i < spr->num_angles; i++)
			for(j = 0; j < 3; j++)
				if(spr->angle_sounds[i].sound[j])
					num_anglesounds++;
	fprintf(f, "%-7d %-13s %-13s %-7d %-7d %-7d "
		"%-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d "
		"%-7d %-7d %-7d %-7d"
		"%-15f %-15f %-15f"
		"%-7d %-7d %-7d %-7d\n",
		spr->id,
		spr->name,
		spr->filename,
		spr->resource_id,
		spr->frames_per_angle,
		spr->num_angles,

		spr->mirror_mode != 0,
		spr->color_flag,
		spr->layer,
		spr->color_map,
		!!(spr->sequence_type & 4),
		spr->replay,
		!!(spr->sequence_type & 1),
		!!(spr->sequence_type & 2),
		!!(spr->sequence_type & 8),
		spr->bounding_box[0],
		spr->bounding_box[1],
		spr->bounding_box[2],
		spr->bounding_box[3],
		spr->speed_adjust,
		spr->duration,
		spr->replay_delay,
		spr->editor_flag,
		spr->num_hooks,
		soundid,
		num_anglesounds		// num sound angles
	);
	for(i = 0; i < spr->num_hooks; i++){
		fprintf(f, "  %-7d %-7d %-7d %-7d\n",
			spr->hooks[i].sprite_id,
			spr->hooks[i].off_x,
			spr->hooks[i].off_y,
			spr->hooks[i].angle);
	}
	if(spr->has_angle_sounds)
		for(i = 0; i < spr->num_angles; i++){
			for(j = 0; j < 3; j++){
				soundid = spr->angle_sounds[i].sound[j] ? spr->angle_sounds[i].sound[j]->id : -1;
				if(soundid < 0)
					continue;
				fprintf(f, "    %-7d %-7d %-7d\n", i, spr->angle_sounds[i].delay[j], soundid);
			}
		}
}

void
dumpSprites(FILE *f, World *world)
{
	int i;
	int num_sprites;
	uchar *dowrite;
	Sprite *spr;

	dowrite = malloc(world->num_sprites);
	num_sprites = 0;
	for(i = 0; i < world->num_sprites; i++){
		spr = world->sprites[i];
		if(spr == nil ||
			spr->resource_id == 0 &&
			spr->name[0] == 0 &&
			spr->filename[0] == 0)
			dowrite[i] = 0;
		else{
			dowrite[i] = 1;
			num_sprites++;
		}
	}
	fprintf(f, "%d\n%d\n", world->num_sprites, num_sprites);
	for(i = 0; i < world->num_sprites; i++){
		if(dowrite[i])
			dumpSprite(f, world->sprites[i]);
	}
	free(dowrite);
}

void
readSprite(Zfile *zf, Sprite *spr, Sound **sounds, Color_Table **coltab)
{
	short tmpid;
	int i, j;
	spr->color_tables = coltab;
	zread(zf, spr->name, 21);
	zread(zf, spr->filename, 13);
	zread(zf, &spr->resource_id, 4);
	zread(zf, &spr->loaded, 1);
	zread(zf, &spr->color_flag, 1);
	zread(zf, &spr->layer, 1);
	zread(zf, &spr->color_map, 2);
	zread(zf, &spr->replay, 1);
	zread(zf, &spr->bounding_box[0], 2);
	zread(zf, &spr->bounding_box[1], 2);
	zread(zf, &spr->bounding_box[2], 2);
	zread(zf, &spr->bounding_box[3], 2);
	zread(zf, &spr->num_hooks, 2);
	zread(zf, &tmpid, 2);
	zread(zf, &spr->has_angle_sounds, 1);
	zread(zf, &spr->frames_per_angle, 2);
	zread(zf, &spr->num_angles, 2);
	zread(zf, &spr->speed_adjust, 4);
	zread(zf, &spr->duration, 4);
	zread(zf, &spr->replay_delay, 4);
	zread(zf, &spr->sequence_type, 1);
	zread(zf, &spr->id, 2);
	zread(zf, &spr->mirror_mode, 1);
	// if version >= 10.72:
	zread(zf, &spr->editor_flag, 1);
	spr->sound = tmpid >= 0 ? sounds[tmpid] : nil;
	if(spr->num_hooks > 0){
		spr->hooks = calloc(spr->num_hooks, sizeof(Sprite_Hook));
		zread(zf, spr->hooks, spr->num_hooks*sizeof(Sprite_Hook));
	}else
		spr->hooks = nil;
	if(spr->has_angle_sounds){
		spr->angle_sounds = calloc(spr->num_angles, sizeof(Angle_Sound));
		for(i = 0; i < spr->num_angles; i++)
			for(j = 0; j < 3; j++){
				zread(zf, &spr->angle_sounds[i].delay[j], 2);
				zread(zf, &tmpid, 2);
				spr->angle_sounds[i].sound[j] = tmpid >= 0 ? sounds[tmpid] : nil;
			}
	}else
		spr->angle_sounds = nil;
}

void
readSprites(Zfile *zf, World *world)
{
	int i;
	zread(zf, &world->num_sprites, 2);
	world->sprites = calloc(world->num_sprites, PTRSZ);
	zread(zf, world->sprites, world->num_sprites*PTRSZ);
	for(i = 0; i < world->num_sprites; i++){
		if(world->sprites[i] == nil)
			continue;
		world->sprites[i] = calloc(1, sizeof(Sprite));
		readSprite(zf, world->sprites[i], world->sounds, world->color_tables);
	}
}

void
dump_terrain_types(FILE *f, Map *map)
{
	int i, j;
	int nterr;
	int soundid;
	int nborders;
	Terrain *t;
	char name[13];

	nterr = 0;
	for(i = 0; i < NUMTERRAINS; i++)
		if(map->terrain[i].enabled)
			nterr++;
	fprintf(f, "%d\n", nterr);
	for(i = 0; i < nterr; i++){
		t = &map->terrain[i];
		if(!t->enabled)
			continue;
		soundid = t->sound ? t->sound->id : -1;
		nborders = 0;
		for(j = 0; j < NUMTERRAINS; j++)
			if(t->borders[j] != 0)
				nborders++;
		for(j = 0; j < 13; j++)
			name[j] = t->name[j] == ' ' ? '_' : t->name[j];
		fprintf(f, "%-7d %-14.13s %-14.13s "
			"%-7d %-7d %-7d %-7d "
			"%-7d %-7d %-7d "
			"%-7d %-7d %-7d %-15f %-15f %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d ",
			i, name, t->filename,

			t->resource_id,
			t->blend_level,
			t->blend_mode,
			0, 	// unused, even in aoe1

			t->map_col_hi,
			t->map_col_mid,
			t->map_col_low,

			soundid,
			t->is_animated,
			t->anim_frame_count,
			t->interval,
			t->pause_after_loop,
			t->replacement_terrain,
			t->num_tiles_x,
			t->num_tiles_y,
			t->map_col_cliff_left,
			t->map_col_cliff_right,
			t->impassable,
			t->passable,
			nborders
			);
		for(j = 0; j < 19; j++)
			fprintf(f, "%-7d %-7d ", t->slopeinfo[j].num_frames, t->slopeinfo[j].num_angles);
		for(j = 0; j < 42; j++)
			if(t->borders[j] != 0)
				fprintf(f, "%-7d %-7d ", j, t->borders[j]);
		fprintf(f, "\n");
	}
}

// pretty useless after the new terrain system was introduced after aoe1
// only outputs "0" as no borders are used
void
dump_border_types(FILE *f, Map *map)
{
	int i, j;
	int soundid;
	int nborders;
	Terrain_Border *b;
	char name[13];

	nborders = 0;
	for(i = 0; i < 16; i++)
		if(map->borders[i].enabled)
			nborders++;
	fprintf(f, "%d\n", nborders);
	for(i = 0; i < nborders; i++){
		b = &map->borders[i];
		if(!b->enabled)
			continue;
		soundid = b->sound ? b->sound->id : -1;
		for(j = 0; j < 13; j++)
			name[j] = b->name[j] == ' ' ? '_' : b->name[j];
		fprintf(f, "%-7d %-14.13s %-14.13s "
			"%-7d %-7d "
			"%-7d %-7d %-7d "
			"%-7d %-7d %-7d %-15f %-15f "
			"%-7d %-7d ",
			i, name, b->filename,

			b->resource_id,
			0, 	// unused, even in aoe1

			b->map_col_hi,
			b->map_col_mid,
			b->map_col_low,

			soundid,
			b->is_animated,
			b->anim_frame_count,
			b->interval,
			b->pause_after_loop,

			b->base_terrain,
			b->border_style
			);
		for(j = 0; j < 12*19; j++)
			fprintf(f, "%-7d %-7d ", b->slopeinfo[j].num_frames, b->slopeinfo[j].num_angles);
		fprintf(f, "\n");
	}
}

void
dump_terrain_obj_types(FILE *f, Map *map)
{
	int i, j;
	int nobjs;
	nobjs = 0;
	for(i = 0; i < 42; i++)
		if(map->terrain[i].enabled)
			nobjs += map->terrain[i].num_objects;
	fprintf(f, "%d\n", nobjs);
	for(i = 0; i < 42; i++){
		if(!map->terrain[i].enabled)
			continue;
		for(j = 0; j < map->terrain[i].num_objects; j++)
			fprintf(f, "%-7d %-7d %-7d %-7d\n", i, 
				map->terrain[i].object_id[j],
				map->terrain[i].object_density[j],
				map->terrain[i].object_flag[j]);
	}
}

void
readMap(Zfile *zf, World *world)
{
	int i;
	Terrain *t;
	Terrain_Border *b;
	Map *map;

	assert(sizeof(Map) == 0xa4e0);
	map = malloc(sizeof(Map));
	world->map = map;
	zread(zf, map, sizeof(Map));
	for(i = 0; i < 42; i++){
		t = &map->terrain[i];
		if(t->enabled)
			t->sound = (int)t->sound < 0 ? nil : world->sounds[(int)t->sound];
	}
	for(i = 0; i < 16; i++){
		b = &map->borders[i];
		if(b->enabled)
			b->sound = (int)b->sound < 0 ? nil : world->sounds[(int)b->sound];
	}
}

void
readWorld(Zfile *zf, World *world)
{
	readTerrainTables(zf, world);
	readColorTables(zf, world);
	readSounds(zf, world);
	readSprites(zf, world);
	readMap(zf, world);
printf("%x\n", ztell(zf));

}

void
WorldDump(World *world)
{
	FILE *f;
	f = mustopen("data/tr_tset.txt", "w");
	dumpTerrainTables(f, world);
	fclose(f);
	f = mustopen("data/tr_colr.txt", "w");
	dumpColorTables(f, world);
	fclose(f);
	f = mustopen("data/tr_snd.txt", "w");
	dumpSounds(f, world);
	fclose(f);
	f = mustopen("data/tr_spr.txt", "w");
	dumpSprites(f, world);
	fclose(f);
	f = mustopen("data/tr_terr.txt", "w");
	dump_terrain_types(f, world->map);
	fclose(f);
	f = mustopen("data/tr_bdr.txt", "w");
	dump_border_types(f, world->map);
	fclose(f);
	f = mustopen("data/tr_tobj.txt", "w");
	dump_terrain_obj_types(f, world->map);
	fclose(f);
}

World*
WorldCreate(const char *datpath)
{
	Zfile *zf;
	char buf[8];
	World *world;

	world = malloc(sizeof(World));

	zf = zfopen(datpath, "rb");
	zread(zf, buf, 8);
	if(strcmp(buf, "VER 5.7") != 0){
		fprintf(stderr, "wrong version\n");
		zfclose(zf);
		return nil;
	}
	readWorld(zf, world);
	zfclose(zf);
	return world;
}
