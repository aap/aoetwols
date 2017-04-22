#include "aok.h"

Drs *gResourceList;

Drs*
DrsOpen(const char *path)
{
	FILE *f;
	DrsHeader tmpHead;
	uchar *header;
	Drs *drs;
	Drs **i;

	f = mustopen(path, "rb");
	fread(&tmpHead, 1, sizeof(DrsHeader), f);
	rewind(f);
	header = malloc(tmpHead.headerSize);
	fread(header, 1, tmpHead.headerSize, f);
	drs = malloc(sizeof(Drs));
	drs->header = (DrsHeader*)header;
	drs->tables = (DrsTable*)(drs->header+1);
	drs->file = f;
	drs->next = NULL;
	for(i = &gResourceList; *i; i = &(*i)->next);
	*i = drs;
	return drs;
}

static DrsFile*
findFile(uint type, int id, Drs **outdrs)
{
	int i, j;
	Drs *drs;
	DrsFile *f;

	for(drs = gResourceList; drs; drs = drs->next){
		for(i = 0; i < drs->header->numTables; i++){
			if(drs->tables[i].type != type)
				continue;
			f = DrsGetFileTable(drs, i);
			for(j = 0; j < drs->tables[i].numFiles; j++){
				if(f->id == id){
					*outdrs = drs;
					return f;
				}
				f++;
			}
		}
	}
	return nil;
}

uchar*
DrsLoadFile(uint type, int id)
{
	uchar *data;
	Drs *drs;
	DrsFile *f;

	f = findFile(type, id, &drs);
	if(f){
		data = malloc(f->size);
		fseek(drs->file, f->offset, SEEK_SET);
		fread(data, 1, f->size, drs->file);
		return data;
	}
	return nil;
}

FILE*
DrsGetFilePointer(uint type, int id)
{
	Drs *drs;
	DrsFile *f;

	f = findFile(type, id, &drs);
	if(f){
		fseek(drs->file, f->offset, SEEK_SET);
		return drs->file;
	}
	return nil;
}

void
DrsDump(Drs *drs)
{
	DrsFile *f;
	int i, j;
	for(i = 0; i < drs->header->numTables; i++){
		printf("table %.4s %d\n",
			(char*)&drs->tables[i].type, drs->tables[i].numFiles);
		f = DrsGetFileTable(drs, i);
		for(j = 0; j < drs->tables[i].numFiles; j++){
			printf("%d %X %X\n", f->id, f->offset, f->size);
			f++;
		}
	}
}
