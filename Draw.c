#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "SDL_Helper.h"
#include "Draw.h"
#include "Utility/List.h"
#include "Constants.h"
#include "SDL_Helper.h"

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static SDL_Rect viewport = {0,0,0,0};
static bool isInitialised = false;

//list for tracking loaded textures
static List loadedTextures;
static void freeListedTexture(void* data);

struct textureListItem
{
    char* name;
    SDL_Texture* texture;
};

//List of sprites to render
static List sprites;

//function prototypes
void Viewport_init(int w, int h);
SDL_Texture* Draw_loadTexture(char* pathToImage);

//static function prototypes
static SDL_Texture* getTextureRef(char* pathToImg);

/********************************************************************
*********************************************************************
****                         Spritesheet                          ***
*********************************************************************
********************************************************************/

struct _Spritesheet
{
    SDL_Texture* spritesheet;
    int w, h;   //width and height of single frame
    int noStates;   //how many states represented in sheet
    int* framesPerState; //array of ints specifying how many frames per state exist
};

Spritesheet Spritesheet_create(char* pathToImg, int w, int h, int noStates, int* framesPerState)
{
    Spritesheet res = malloc(sizeof(struct _Spritesheet));
    if(pathToImg){res->spritesheet = Draw_loadTexture(pathToImg);} else {res = NULL; return NULL;}
    res->w = w;
    res->h = h;
    res->noStates = noStates;
    res->framesPerState = framesPerState;
    return res;
}

void Spritesheet_destroy(Spritesheet s)
{
    SDL_DestroyTexture(s->spritesheet);
    free(s);
}

/********************************************************************
*********************************************************************
****                         Sprite                               ***
*********************************************************************
********************************************************************/

//the sprite structure used for rendering game objects
struct _Sprite
{
    double* gX, * gY, * gZ;        //global x, y and z of sprite
    int gW, gH;                 //global width and height of sprite
    bool isVisible, posRefByCentre; //flag to mark a sprite for drawing
    Spritesheet spriteSheet;   //pointer to texture
    int* state;                 //state
    int frame;                  //current frame
    bool isFullscreen;     //whether exists in global space or directly on screen
};

Sprite Sprite_createSprite(char* pathToSpriteSheet, int sW, int sH, int numStates, int* framesPerState)
{
    Sprite s = malloc(sizeof(struct _Sprite));
    Spritesheet ss = Spritesheet_create(pathToSpriteSheet, sW, sH, numStates, framesPerState);
    s->spriteSheet = ss;
    s->frame=0;
    s->gH = 0;
    s->gW = 0;
    s->isFullscreen = false;
    s->isVisible = true;
    s->posRefByCentre = false;
    s->gX=NULL;
    s->gY=NULL;
    s->gZ=NULL;
    s->state=NULL;

    List_append(&sprites, s);
    Sprite_destroySprite(s);

    return sprites.tail->data;
}

void Sprite_destroySprite(Sprite s)
{
    Spritesheet_destroy(s->spriteSheet);
    free(s);
}

void Sprite_isFullscreen(Sprite s, bool fullscreen)
{
    s->isFullscreen = fullscreen;
}

void Sprite_posByCentre(Sprite s, bool PBC)
{
    s->posRefByCentre = PBC;
}

void Sprite_setSpriteInWorldDims(Sprite s, int w, int h)
{
    s->gH = h;
    s->gW = w;
}

void Sprite_setSpriteInWorldPosRef(Sprite s, double* x, double* y, double* z)
{
    if(x)s->gX = x;
    if(y)s->gY = y;
    if(z)s->gZ = z;
}

void Sprite_setSpriteStateRef(Sprite s, int* state)
{
    s->state = state;
}

void Sprite_renderSprite(Sprite s)
{
    int currState = (s->state==NULL)?0:*(s->state);

    //source rect on spritesheet from where to read the image
    SDL_Rect srcRect;
    SDL_Rect* pSrcRect;
    if(s->spriteSheet->h == 0 || s->spriteSheet->w == 0)
    {
        pSrcRect = NULL;
    }else{
         SDL_Rect tmpRect= {
                s->frame*s->spriteSheet->w,         //x
                currState*s->spriteSheet->h,        //y
                s->spriteSheet->w,                  //w
                s->spriteSheet->h                   //h
        };
        srcRect = tmpRect;
        pSrcRect = &srcRect;
    }

    //the dest rect on screen to which the image should be drawn
    Vec3D relPos, gPos, vPos;
    SDL_Rect dstRect;
    SDL_Rect* pDstRect;
    if(s->isFullscreen)
    {
        pDstRect = NULL;
    }else
    {
        if(s->gX) gPos.i = *(s->gX);
        if(s->gY) gPos.j = *(s->gY);
        if(s->gZ) gPos.k = *(s->gZ);

        if(s->posRefByCentre)
        {
            gPos.i -= (s->gW/2);
            gPos.j -= (s->gH/2);
        }

        vPos.i = viewport.x;
        vPos.j = viewport.y;
        vPos.k = 0;

        relPos = Vec3D_subtract(gPos, vPos);

        SDL_Rect tmpRect = {
                (int)relPos.i,       //x
                (int)relPos.j,       //y
                s->gW,          //w
                s->gH           //h
        };
        dstRect = tmpRect;
        pDstRect = &dstRect;
    }

    //copy the image into the renderer for render to screen at next step
    SDL_RenderCopyEx(gRenderer, s->spriteSheet->spritesheet, pSrcRect, pDstRect, 0, NULL, 0);
}

/********************************************************************
*********************************************************************
****                    Draw functions                            ***
*********************************************************************
********************************************************************/

bool Draw_init()
{
    if(!isInitialised){
        if(SDL_Helper_init())
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
                gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
                        List_new(&loadedTextures, sizeof(struct textureListItem), &freeListedTexture);
                        List_new(&sprites, sizeof(struct _Sprite), NULL);
                        Viewport_init(SCREEN_WIDTH, SCREEN_HEIGHT);
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


        List_destroy(&loadedTextures);
        List_destroy(&sprites);

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Helper_quit();
    }
}

//Image handling functions
SDL_Texture* Draw_loadTexture(char* pathToImage)
{
    if(gRenderer == NULL) Draw_init();
    SDL_Surface* s = NULL;
    SDL_Texture* t = getTextureRef(pathToImage);

    if(t==NULL)
    {
        s = IMG_Load(pathToImage);
        if(s!=NULL)
        {
            t = SDL_CreateTextureFromSurface(gRenderer, s);
            struct textureListItem toBeListed;
            toBeListed.name = pathToImage;
            toBeListed.texture = t;
            List_append(&loadedTextures, &toBeListed);
            SDL_FreeSurface(s);
        }
    }
    return t;
}

void Draw_freeImage(SDL_Texture* img)
{
    if(img!=NULL) SDL_DestroyTexture(img);
}


/********************************************************************
*********************************************************************
****                          View Port                           ***
*********************************************************************
********************************************************************/

void Viewport_init(int w, int h)
{
    viewport.w = w;
    viewport.h = h;
    viewport.x = 0;
    viewport.y = 0;
}

void Viewport_setPos(Vec3D p)
{
    viewport.x = (int)p.i;
    viewport.y = (int)p.j;
}

void Viewport_setPosByCentre(Vec3D p)
{
    viewport.x = (int)p.i-(viewport.w/2);
    viewport.y = (int)p.j-(viewport.h/2);
}

/********************************************************************
*********************************************************************
****                        Actual Render                         ***
*********************************************************************
********************************************************************/


void Draw_renderScene()
{
    ListNode* curr = sprites.head;
    while(curr)
    {
        Sprite s = curr->data;
        if(s->isVisible)Sprite_renderSprite(s);
        curr = curr->next;
    }
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, COLOUR_BLACK);
    SDL_RenderClear(gRenderer);
}


/********************************************************************
*********************************************************************
****                  Local static methods                        ***
*********************************************************************
********************************************************************/

static void freeListedTexture(void* data)
{
    struct textureListItem* img = data;
    SDL_DestroyTexture(img->texture);
}

static SDL_Texture* getTextureRef(char* pathToImg)
{
    if(!loadedTextures.head) return NULL;
    ListNode* current = loadedTextures.head;
    struct textureListItem* currentItem;
    while(current)
    {
        currentItem = current->data;
        if(strcmp(pathToImg, currentItem->name) == 0) return currentItem->texture;
        current = current->next;
    }
    return NULL;
}
