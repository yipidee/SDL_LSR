#include "SDL_Helper.h"

static bool isInitialised = false;

bool SDL_Helper_init()
{
    if(!isInitialised)
    {
        if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0 )
        {
            printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        }else
        {
            Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
            if((Mix_Init(MIX_INIT_MP3)) != MIX_INIT_MP3)
            {
                printf("Audio setup failed!\n");
            }else
            {
            isInitialised = true;
            }
        }
    }
    return isInitialised;
}

void SDL_Helper_quit()
{
    SDL_Quit();
}

bool SDL_Helper_isInitialized()
{
    return isInitialised;
}
