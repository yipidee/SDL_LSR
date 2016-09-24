#include <SDL2/SDL_image.h>
#include "Draw.h"

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static bool isInitialised = false;

bool Draw_init()
{
    if(!isInitialised){
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

                    int img_flags = IMG_INIT_PNG;
                    if(!(IMG_Init(img_flags)&&img_flags))
                    {
                        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    }
                    else
                    {
                        isInitialised = true;
                    }
                }
            }
        }
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
        IMG_Quit();
        SDL_Quit();
    }
}

//Image handling functions
SDL_Texture* Draw_loadImage(char* pathToImage)
{
    if(gRenderer == NULL) Draw_init();
    SDL_Surface* s = NULL;
    SDL_Texture* t = NULL;

    s = IMG_Load(pathToImage);
    if(s!=NULL)
    {
        t = SDL_CreateTextureFromSurface(gRenderer, s);
        SDL_FreeSurface(s);
    }

    return t;
}

void Draw_freeImage(SDL_Texture* img)
{
    if(img!=NULL) SDL_DestroyTexture(img);
}

void Draw_controller(AnalogueController* ac)
{
    if(!isInitialised) Draw_init();
    if(ac->baseImg == NULL)
    {
        ac->knobImg = Draw_loadImage(ac->pathToKImg);
        ac->baseImg = Draw_loadImage(ac->pathToBImg);
    }

    SDL_Rect cRect = {ac->knob.x-ac->knob.r, ac->knob.y-ac->knob.r, 2*ac->knob.r, 2*ac->knob.r};
    SDL_Rect dRect = {ac->base.x-ac->base.r, ac->base.y-ac->base.r, 2*ac->base.r, 2*ac->base.r};

    SDL_RenderCopy(gRenderer, ac->baseImg, NULL, &dRect);
    SDL_RenderCopy(gRenderer, ac->knobImg, NULL, &cRect);
}

void Draw_renderScene()
{
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, COLOUR_BLACK);
    SDL_RenderClear(gRenderer);
}

