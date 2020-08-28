#include "aok.h"

uint sdlpalette[256];

void
GameMakeSDLPalette(Game *game, Palette *palette)
{
	int i;
	uint col;
	for(i = 0; i < 256; i++){
		col = 0xFF000000 |
			(int)palette->col[i].r << 16 |
			(int)palette->col[i].g << 8 |
			(int)palette->col[i].b;
		game->sdlPalette[i] = col;
	}
}

static void
ConvScreen(Game *game)
{
	int n;
	uint *dst;
	uchar *src;
	uint *pal;
	dst = game->screentmp;
	src = game->screen.data;
	pal = game->sdlPalette;
	n = game->screen.width*game->screen.height;
	while(n--)
		*dst++ = pal[*src++];
}

int
GameInitSDL(Game *game)
{
	SDL_Init(SDL_INIT_VIDEO);
	game->window = SDL_CreateWindow("AoK test", 100, 100, game->screen.width, game->screen.height, 0); 
	if(game->window == nil){
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	game->renderer = SDL_CreateRenderer(game->window, -1, 0);	// TODO: check out the flags here

	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

	game->screentex = SDL_CreateTexture(game->renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, game->screen.width, game->screen.height);

	game->screen.stride = game->screen.width;
	game->screen.data = malloc(game->screen.stride*game->screen.height);
	game->screentmp = malloc(game->screen.width*game->screen.height*4);

	return 0;
}

int lastMouseX, lastMouseY;
int offx, offy;

void
mouse(int button, int state, int x, int y)
{
	int diffX, diffY;
	diffX = x - lastMouseX;
	diffY = y - lastMouseY;
	if(state & SDL_BUTTON_MMASK){
		offx += diffX;
		offy += diffY;
	}
//	printf("%d %d %d %d | %d %d\n", button, state, x, y, diffX, diffY); fflush(stdout);
	lastMouseX = x;
	lastMouseY = y;
}

void
GameMainLoop(Game *game)
{
	SDL_Event e;
	SDL_MouseButtonEvent *mbev;
	SDL_MouseMotionEvent *mmev;

	while(1){
		if(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT)
				break;
			switch(e.type){
			case SDL_MOUSEMOTION:
				mmev = (SDL_MouseMotionEvent*)&e;
				mouse(0, mmev->state, mmev->x, mmev->y);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				mbev = (SDL_MouseButtonEvent*)&e;
				mouse(mbev->button, mbev->state, mbev->x, mbev->y);
				break;
			}
		}

		memset(game->screen.data, 100, game->screen.height*game->screen.stride);
		ViewDraw(game->view, offx, offy);

		ConvScreen(game);
		SDL_UpdateTexture(game->screentex, nil, game->screentmp, game->screen.width*4);
		SDL_RenderClear(game->renderer);
		SDL_RenderCopy(game->renderer, game->screentex, nil, nil);
		SDL_RenderPresent(game->renderer);
	}

	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}
