#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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

//static counter for animation
unsigned long animation_count = 0;

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
    printf("texture destroyed here\n");
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
    int gW, gH, angle;                 //global width and height of sprite
    bool isVisible, posRefByCentre; //flag to mark a sprite for drawing
    Spritesheet spriteSheet;   //pointer to texture
    int* state;                 //state
    int frame, frameRate;      //current frame
    bool isFullscreen;     //whether exists in global space or directly on screen
};

Sprite Sprite_createSprite(char* pathToSpriteSheet, int sW, int sH, int numStates, int* framesPerState)
{
    Sprite s = malloc(sizeof(struct _Sprite));
    Spritesheet ss = Spritesheet_create(pathToSpriteSheet, sW, sH, numStates, framesPerState);
    s->spriteSheet = ss;
    s->frame = 0;
    s->angle = 0;
    s->frameRate = 5;
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
    free(s);

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
    if(s->spriteSheet->h == 0 && s->spriteSheet->w == 0)
    {
        pSrcRect = NULL;
    }else{
        int localW, localH;
        if(s->spriteSheet->w == USE_FULL_IMAGE_WIDTH)
        {
            SDL_QueryTexture(s->spriteSheet->spritesheet, NULL, NULL, &localW, NULL);
            localH = s->spriteSheet->h;
        }else  // using full image height
        {
            SDL_QueryTexture(s->spriteSheet->spritesheet, NULL, NULL, NULL, &localH);
            localW = s->spriteSheet->w;
        }
        SDL_Rect tmpRect= {
               s->frame*localW,         //x
               currState*localH,        //y
               localW,                  //w
               localH                   //h
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
    SDL_RenderCopyEx(gRenderer, s->spriteSheet->spritesheet, pSrcRect, pDstRect, s->angle, NULL, 0);
}

void Sprite_tickFrame(Sprite s)
{
    if(s->spriteSheet->framesPerState)
    {
        ++s->frame;
        int state_l = s->state ? *s->state : 0;
        if(s->frame >= s->spriteSheet->framesPerState[state_l]) s->frame = 0;
    }
}

int Sprite_getCurrFrame(Sprite s)
{
    return s->frame;
}

int* Sprite_getRateSetAddress(Sprite s)
{
    return &s->frameRate;
}

void Sprite_setFrameRate(int* addr, int rate)
{
    *addr = rate;
}

int* Sprite_getAngleSetAddress(Sprite s)
{
    return &s->angle;
}

void Sprite_setAngle(int* addr, int angle)
{
    *addr = angle;
}

/********************************************************************
*********************************************************************
****                    Draw functions                            ***
*********************************************************************
********************************************************************/

bool Draw_init()
{
    if(!isInitialised){
        if(!SDL_Helper_init())
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
        List_destroy(&loadedTextures);
        List_destroy(&sprites);

        //Destroy window
        if(!gRenderer)SDL_DestroyRenderer( gRenderer );
        gRenderer = NULL;
        if(!gWindow)SDL_DestroyWindow( gWindow );
        gWindow = NULL;

        //Quit SDL subsystems
        TTF_Quit();
        IMG_Quit();
        SDL_Helper_quit();
    }
}

//Image handling functions
SDL_Texture* Draw_loadTexture(char* pathToImage)
{
    if(!isInitialised) Draw_init();
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
    // tick count
    ++animation_count;
    if(animation_count >= MAX_LONG_VALUE) animation_count = 0;
    
    // render all sprites
    ListNode* curr = sprites.head;
    while(curr)
    {
        Sprite s = curr->data;
        if(s->isVisible)Sprite_renderSprite(s);
        if((s->frameRate != MAX_LONG_VALUE)&&((animation_count % s->frameRate)==0)) Sprite_tickFrame(s);
        curr = curr->next;
    }
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, COLOUR_BLACK);
    SDL_RenderClear(gRenderer);
}

void Draw_line(Vec3D p1, Vec3D p2)
{
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(gRenderer, p1.i, p1.j, p2.i, p2.j);
}

/********************************************************************
*********************************************************************
****                    Text Label methods                        ***
*********************************************************************
********************************************************************/

static bool TTF_INITIALIZED = false;

struct _TextLabel
{
    char mText[TEXT_LABEL_MAX_LENGTH]; //The actual label text
    int x, y, w, h;             //The position and size of label
    int mCurrLength;            //length of stored text
    TTF_Font* mFont;            //Font used for rendering text
    int mTextSize;              //Text point size
    SDL_Color mFontColour;      //Text colour
    bool _initialised;          //flag to show whether initialised
    SDL_Surface* textSurf;
};

void TL_setFont(TextLabel tl, char* pathToFont)
{
    if(!TTF_INITIALIZED) {TTF_Init(); TTF_INITIALIZED = true;}
    if(pathToFont==NULL)
    {
        tl->mFont = TTF_OpenFont(DEFAULT_FONT, tl->mTextSize);
    }else{
        tl->mFont = TTF_OpenFont(pathToFont, tl->mTextSize);
    }
}

void _initTL(TextLabel tl)
{
    tl->mCurrLength = 0;
    SDL_Color c = RGB_BLACK;
    tl->mFontColour = c;
    strcpy(tl->mText, "\0");
    tl->mCurrLength = strlen(tl->mText);
    tl->mTextSize = DEFAULT_TEXT_SIZE;
    TL_setFont(tl, NULL);
    tl->textSurf = NULL;
    tl->x = 0; tl->y = 0; tl->w = 0; tl->h = 0;
    tl->_initialised = true;
}

void _renderTextToSurface(TextLabel tl)
{
    if(!TTF_INITIALIZED) {TTF_Init(); TTF_INITIALIZED = true;}
    if(tl->textSurf)
    {
        SDL_FreeSurface(tl->textSurf);
        tl->textSurf = NULL;
    }
    if(strcmp(tl->mText, "\0")!=0)
    {
        tl->textSurf = TTF_RenderText_Blended(tl->mFont, tl->mText, tl->mFontColour);
        tl->w = tl->textSurf->w; tl->h = tl->textSurf->h;
    }
}

//Get pointer to bounding rect
SDL_Rect TL_getBoundRect(TextLabel tl)
{
    SDL_Rect bb = {tl->x, tl->y, tl->w, tl->h};
    return bb;
}

void TL_renderTextLabel(TextLabel tl)
{
    if(tl->textSurf)
    {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, tl->textSurf);
        SDL_Rect bb = TL_getBoundRect(tl);
        SDL_RenderCopy(gRenderer, textTexture, NULL, &bb);
        SDL_DestroyTexture(textTexture);
    }
}

//Creates a label with text at pos x and y
TextLabel TL_createTextLabel(char* text, int x, int y)
{
    TextLabel tl = malloc(sizeof(struct _TextLabel));
    _initTL(tl);
    if(text && strlen(text) < TEXT_LABEL_MAX_LENGTH)
    {
        int length = strlen(text);
        strcpy(tl->mText, text);
        tl->mCurrLength = length;
    }
    tl->x = x; tl->y = y;
    _renderTextToSurface(tl);
    return tl;
}

void TL_destroyTextLabel(TextLabel tl)
{
    TTF_CloseFont(tl->mFont);
    if(tl->textSurf != NULL) {SDL_FreeSurface(tl->textSurf); tl->textSurf=NULL;}
    if(tl)free(tl);
    tl = NULL;
}

//Sets tl's text data
void TL_setText(TextLabel tl, char* text)
{
    if(tl->_initialised)
    {
        if(!text)
        {
            strcpy(tl->mText, "\0");
            tl->mCurrLength = strlen(tl->mText);
        }else
        {
            tl->mCurrLength = strlen(text);
            if(tl->mCurrLength<=TEXT_LABEL_MAX_LENGTH) strcpy(tl->mText, text);
        }
       _renderTextToSurface(tl);
    }
}

void _resizeFont(TextLabel tl)
{
    if(tl->mFont != NULL) TTF_CloseFont(tl->mFont);
    tl->mFont = TTF_OpenFont(DEFAULT_FONT, tl->mTextSize);
}

//Set font size
void TL_setFontSize(TextLabel tl, int fSize)
{
    tl->mTextSize = fSize;
    _resizeFont(tl);
    _renderTextToSurface(tl);
}

//set x
void TL_setX(TextLabel tl, int x)
{
    tl->x = x;
}

//set y
void TL_setY(TextLabel tl, int y)
{
    tl->y = y;
}

//get x
int TL_getX(TextLabel tl)
{
    return tl->x;
}

//get y
int TL_getY(TextLabel tl)
{
    return tl->y;
}

//get width of text label
int TL_getWidth(TextLabel tl)
{
    return tl->w;
}

//get Height of text label
int TL_getHeight(TextLabel tl)
{
    return tl->h;
}


/********************************************************************
*********************************************************************
****                  Local static methods                        ***
*********************************************************************
********************************************************************/

static void freeListedTexture(void* data)
{
    struct textureListItem* img = data;
    if(img->texture)SDL_DestroyTexture(img->texture);
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
