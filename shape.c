#include "aok.h"

int gCurPlayerColor = 1;

Shape*
ShapeCreate(uchar *data)
{
	Shape *slp;
	slp = malloc(sizeof(Shape));
	slp->header = (SlpHeader*)data;
	slp->frames = (SlpFrame*)(slp->header+1);
	slp->numFrames = slp->header->numFrames;
	slp->size = 0;	// only used for writing, set manually if needed
	return slp;
}

Shape*
ShapeCreateFromTemplate(SlpTemplate *templ)
{
	Shape *slp;
	SlpFrame *f;

	slp = malloc(sizeof(Shape));
	slp->header = malloc(sizeof(SlpHeader) + sizeof(SlpFrame) + templ->height*8 + templ->dataSize);
	slp->header->numFrames = 1;
	slp->frames = (SlpFrame*)(slp->header+1);

	f = &slp->frames[0];
	f->outlineTableOffset = sizeof(SlpHeader) + sizeof(SlpFrame);
	f->cmdTableOffset = f->outlineTableOffset + templ->height*4;
	f->paletteOffset = 0;
	f->properties = 0;
	f->width = templ->width;
	f->height = templ->height;
	f->centerX = templ->centerX;
	f->centerY = templ->centerY;
	memcpy(ShapeGetOutlineTbl(slp, 0), templ->outlineTbl, 4*f->height);
	memcpy(ShapeGetCommandTbl(slp, 0), templ->cmdOffsetTbl, 4*f->height);
	return slp;
}

void
ShapeDump(Shape *slp)
{
	int i;
	SlpFrame *frm;

	frm = slp->frames;
	fprintf(stderr, "numFrames: %d\n", slp->header->numFrames);
	for(i = 0; i < slp->header->numFrames; i++){
		fprintf(stderr, "%d %d %d %d ", frm->width, frm->height, frm->centerX, frm->centerY);
		fprintf(stderr, "%x %x\n", frm->cmdTableOffset, frm->outlineTableOffset);
		frm++;
	}
}

int
ShapeClipTest(Shape *slp, int n, Surface *s, int xoff, int yoff)
{
	int xmin, ymin, xmax, ymax;
	SlpFrame *frm;

	frm = &slp->frames[n];
	xmin = xoff - frm->centerX;
	ymin = yoff - frm->centerY;
	xmax = xmin + frm->width;
	ymax = ymin + frm->height;

	if(ymax <= 0 || xmax <= 0 ||
	   xmin >= s->width || ymin >= s->height)
		return 0;	// outside
	if(ymin >= 0 && xmin >= 0 &&
	   xmax <= s->width && ymax <= s->height)
		return 2;	// inside
	return 1;	// intersect
}

#define INSIDE(x, a, b) ((x) >= (a) && (x) < (b))

void
ShapeForallPixels(Shape *slp, PixelCB cb)
{
	short *out;
	uchar *cmd;
	uchar c;
	int npx;
	int y;
	SlpFrame *frm;
	int n;

	for(n = 0; n < slp->numFrames; n++){
	frm = &slp->frames[n];
	out = ShapeGetOutlineTbl(slp, n);
	cmd = ShapeGetCommand(slp, *ShapeGetCommandTbl(slp, n));
	for(y = 0; y < frm->height; y++){
		for(;;){
			switch(*cmd & 0x3){
			case 0x0:	/* small block */
				npx = *cmd++ >> 2;
				while(npx--){
					*cmd = cb(*cmd);
					cmd++;
				}
				break;
			case 0x1:	/* small skip */
				npx = *cmd++ >> 2;
				break;
			default:
				switch(*cmd & 0xF){
				case 0x2:	/* block */
					npx = (int)(*cmd++ & 0xF0) << 4;
					npx |= *cmd++;
					while(npx--){
						*cmd = cb(*cmd);
						cmd++;
					}
					break;
				case 0x3:	/* skip */
					npx = (int)(*cmd++ & 0xF0) << 4;
					npx |= *cmd++;
					break;
				case 0x6:	/* player block */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					while(npx--){
						cmd++;
					}
					break;
				case 0x7:	/* fill */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					*cmd = cb(*cmd);
					cmd++;
					break;
				case 0xA:	/* player fill */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					c = *cmd++ + (gCurPlayerColor<<4);
					break;
				case 0xB:	/* shadow fill ?? */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					cmd++;
					break;
				case 0xF:	/* end of line */
					cmd++;
					goto endofline;
				case 0xE:	/* extended */
					switch(*cmd++ >> 4){
					case 0x0:	/* next only if not flipped */
						break;
					case 0x1:	/* next only if flipped */
						break;
					case 0x2:	/* set normal color transform */
						break;
					case 0x3:	/* set alternate color transform */
						break;
					case 0x4:	/* one outline player */
						break;
					case 0x5:	/* outline player */
						cmd++;
						break;
					case 0x6:	/* one outline black */
						break;
					case 0x7:	/* outline black */
						cmd++;
						break;
					}
				}
				break;
			}
		}
	endofline:;
		out += 2;
	}
	}
}

/* TODO: This could be optimized a lot by skipping clip tests if possible */
void
ShapeDrawFrame(Shape *slp, int n, Surface *s, int xoff, int yoff)
{
	short *out;
	uchar *cmd;
	uchar c;
	uchar *lp;
	int npx;
	int x, y;
	SlpFrame *frm;
	int clip;
	int yinside;
	int dodraw;

	clip = ShapeClipTest(slp, n, s, xoff, yoff);
	if(clip == 0)
		return;

	frm = &slp->frames[n];
	xoff -= frm->centerX;
	yoff -= frm->centerY;

	out = ShapeGetOutlineTbl(slp, n);
	cmd = ShapeGetCommand(slp, *ShapeGetCommandTbl(slp, n));
	dodraw = 1;
	for(y = 0; y < frm->height; y++){
		x = xoff+out[0];
		lp = s->data + (y+yoff)*s->stride + x;
		yinside = INSIDE(y+yoff, 0, s->height);
		for(;;){
			switch(*cmd & 0x3){
			case 0x0:	/* small block */
				npx = *cmd++ >> 2;
				while(npx--){
					if(dodraw && yinside && INSIDE(x, 0, s->width))
						*lp = *cmd;
					lp++;
					x++;
					cmd++;
				}
				break;
			case 0x1:	/* small skip */
				npx = *cmd++ >> 2;
				lp += npx;
				x += npx;
				break;
			default:
				switch(*cmd & 0xF){
				case 0x2:	/* block */
					npx = (int)(*cmd++ & 0xF0) << 4;
					npx |= *cmd++;
					while(npx--){
						if(dodraw && yinside && INSIDE(x, 0, s->width))
							*lp = *cmd;
						lp++;
						x++;
						cmd++;
					}
					break;
				case 0x3:	/* skip */
					npx = (int)(*cmd++ & 0xF0) << 4;
					npx |= *cmd++;
					lp += npx;
					x += npx;
					break;
				case 0x6:	/* player block */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					while(npx--){
						if(dodraw && yinside && INSIDE(x, 0, s->width))
							*lp = *cmd + (gCurPlayerColor<<4);
						lp++;
						x++;
						cmd++;
					}
					break;
				case 0x7:	/* fill */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					while(npx--){
						if(dodraw && yinside && INSIDE(x, 0, s->width))
							*lp = *cmd;
						lp++;
						x++;
					}
					cmd++;
					break;
				case 0xA:	/* player fill */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					c = *cmd++ + (gCurPlayerColor<<4);
					while(npx--){
						if(dodraw && yinside && INSIDE(x, 0, s->width))
							*lp = c;
						lp++;
						x++;
					}
					break;
				case 0xB:	/* shadow fill ?? */
					npx = *cmd++ >> 4;
					if(npx == 0) npx = *cmd++;
					while(npx--){
						if(dodraw && yinside && INSIDE(x, 0, s->width))
							*lp = *cmd;
						lp++;
						x++;
					}
					cmd++;
					break;
				case 0xF:	/* end of line */
					cmd++;
					goto endofline;
				case 0xE:	/* extended */
					switch(*cmd++ >> 4){
					case 0x0:	/* next only if not flipped */
						break;
					case 0x1:	/* next only if flipped */
						break;
					case 0x2:	/* set normal color transform */
						break;
					case 0x3:	/* set alternate color transform */
						break;
					case 0x4:	/* one outline player */
						break;
					case 0x5:	/* outline player */
						cmd++;
						break;
					case 0x6:	/* one outline black */
						break;
					case 0x7:	/* outline black */
						cmd++;
						break;
					}
				}
				break;
			}
		}
	endofline:;
		out += 2;
	}
}

void
ShapeDrawArea(Shape *slp, int n, uchar *data, int stride)
{
	SlpFrame *frm;
	short *out;
	uchar *lp;
	int x, y;

	frm = &slp->frames[n];
	out = ShapeGetOutlineTbl(slp, n);
	for(y = 0; y < frm->height; y++){
		lp = data + y*stride;
		for(x = out[0]; x < frm->width-out[0]; x++)
			lp[x] = 255;
		out += 2;
	}
}

void
ShapeDumpFrame(const char *path, Palette *pal, Shape *slp, int n)
{
	FILE *f;
	SlpFrame *frm;
	Surface s;

	frm = &slp->frames[n];
	s.width = frm->width;
	s.height = frm->height;
	s.stride = BmpGetStride(s.width, 8);
	s.data = malloc(s.height*s.stride);
	memset(s.data, 0, s.height*s.stride);

//	ShapeDrawArea(slp, n, data, stride);
	ShapeDrawFrame(slp, n, &s, 0, 0);

	f = mustopen(path, "wb");
	BmpWriteHeader(s.width, s.height, 8, f);
	BmpWritePalette(pal, f);
	BmpFlipVert(s.width, s.height, 8, s.data);
	fwrite(s.data, 1, s.height*s.stride, f);
	fclose(f);
	free(s.data);
}

/* vim: set ts=8 sw=8 tw=0 noexpandtab cindent softtabstop=8 :*/
