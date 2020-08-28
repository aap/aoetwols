#include "aokbase.h"
#include <unistd.h>

OldDrs*
OldDrsOpen(const char *path)
{
	FILE *f;
	OldDrsHeader tmpHead;
	uchar *header;
	OldDrs *drs;

	f = mustopen(path, "rb");
	fread(&tmpHead, sizeof(OldDrsHeader), 1, f);
	rewind(f);
	header = malloc(sizeof(OldDrsHeader) + sizeof(OldDrsTable) * tmpHead.numFiles);
	fread(header, sizeof(OldDrsHeader) + sizeof(OldDrsTable) * tmpHead.numFiles, 1, f);
	drs = malloc(sizeof(OldDrs));
	drs->header = (OldDrsHeader*)header;
	drs->tables = (OldDrsTable*)(drs->header+1);
	drs->f = f;
	return drs;
}

void
OldDrsDump(OldDrs *drs, const char *out_path)
{
	FILE *f;
	OldDrsFile file;
	uchar *data;
	char out_name[100];
	char ext[4];

	chdir(out_path);
	*out_name = '\0';
	data = nil;
	f = nil;

	for (int i = 0; i < drs->header->numFiles; i++){
		switch (drs->tables[i].type)
		{
		case DrsSlp:
			strcpy(ext, "slp");
			break;
		case DrsWav:
			strcpy(ext, "wav");
			break;
		case DrsBin:
		default:
			strcpy(ext, "bin");
			break;
		}

		if (f)
			fclose(f);
		free(data);

		sprintf(out_name, "%05d.%s", drs->tables[i].id, ext);
		fseek(drs->f, drs->tables[i].offset, SEEK_SET);
		fread(&file, sizeof(OldDrsFile), 1, drs->f);

		data = malloc(file.len);
		fread(data, file.len, 1, drs->f);

		f = mustopen(out_name, "wb");
		fwrite(data, file.len, 1, f);
	}

	if (f)
		fclose(f);
	free(data);
}

int
main(int argc, char *argv[])
{
	OldDrs *drs;

	if (argc < 3)
		return 1;

	drs = OldDrsOpen(argv[1]);
	OldDrsDump(drs, argv[2]);

	if (drs->f)
		fclose(drs->f);

	return 0;
}
