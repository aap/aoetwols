#include "aokbase.h"
#include <SDL2/SDL.h>


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
