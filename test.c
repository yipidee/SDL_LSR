#include <stdbool.h>
#include <SDL2/SDL.h>
#include "AnalogueController.h"

//Colour definitions in RGBA list format for SDL renderer
#define COLOUR_WHITE 0x00, 0x00, 0x00, 0xFF
#define COLOUR_BLACK 0xFF, 0xFF, 0xFF, 0xFF
#define COLOUR_RED 0xFF, 0x00, 0x00, 0xFF
#define COLOUR_GREEN 0x00, 0xFF, 0x00, 0xFF
#define COLOUR_BLUE 0x00, 0x00, 0xFF, 0xFF
#define COLOUR_YELLOW 0xFF, 0xFF, 0x00, 0xFF

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

int main(int argc, char* argv[])
{
    //Initialize SDL
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
	}

	/****************************************************************
	*****************************************************************
	**********************                ***************************
	**********************    TEST AREA   ***************************
	**********************                ***************************
	*****************************************************************
	*****************************************************************/
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    AnalogueController controller = AnalCont_create();

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else
            {
                AnalCont_handleEvent(&controller, &e);
            }
        }

        SDL_Rect cRect = {controller.knob.x-controller.knob.r, controller.knob.y-controller.knob.r, 2*controller.knob.r, 2*controller.knob.r};
        SDL_Rect dRect = {controller.base.x-controller.base.r, controller.base.y-controller.base.r, 2*controller.base.r, 2*controller.base.r};

        SDL_SetRenderDrawColor(gRenderer, COLOUR_BLUE);
        SDL_RenderFillRect(gRenderer, &dRect);

        SDL_SetRenderDrawColor(gRenderer, COLOUR_GREEN);
        SDL_RenderFillRect(gRenderer, &cRect);

        SDL_RenderPresent(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, COLOUR_BLACK);
        SDL_RenderClear(gRenderer);

    }

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    //IMG_Quit();
    SDL_Quit();
}
