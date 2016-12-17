#include "SDL_Helper.h"

static bool isInitialised = false;

bool SDL_Helper_init()
{
    if(!isInitialised)
    {
        if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
        {
            printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
            isInitialised = true;
        }
    }
    return isInitialised;
}

void SDL_Helper_quit()
{
    SDL_Quit();
}
