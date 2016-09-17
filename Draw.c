#include <SDL2/SDL.h>
#include "Draw.h"

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static bool isInitialised = false;

bool Draw_init()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
			}
		}
		isInitialised = true;
	}
	return isInitialised;
}

void Draw_quit()
{
    if(isInitialised)
    {
        //Destroy window
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        //Quit SDL subsystems
        //IMG_Quit();
        SDL_Quit();
    }
}

void Draw_controller(AnalogueController* ac)
{
    if(!isInitialised) Draw_init();
    SDL_Rect cRect = {ac->knob.x-ac->knob.r, ac->knob.y-ac->knob.r, 2*ac->knob.r, 2*ac->knob.r};
    SDL_Rect dRect = {ac->base.x-ac->base.r, ac->base.y-ac->base.r, 2*ac->base.r, 2*ac->base.r};

    SDL_SetRenderDrawColor(gRenderer, COLOUR_BLUE);
    SDL_RenderFillRect(gRenderer, &dRect);

    SDL_SetRenderDrawColor(gRenderer, COLOUR_GREEN);
    SDL_RenderFillRect(gRenderer, &cRect);
}

void Draw_renderScene()
{
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, COLOUR_BLACK);
    SDL_RenderClear(gRenderer);
}

