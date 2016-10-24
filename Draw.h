#ifndef _DRAW_H_
#define _DRAW_H_

/*******************************************************
********************************************************
***   Drawing library for drawable game objects      ***
********************************************************
********************************************************
*/

#define USE_FULL_IMAGE_WIDTH 0
#define USE_FULL_IMAGE_HEIGHT 0

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "Utility/Geometry.h"
#include "AnalogueController.h"
#include "Constants.h"

typedef struct _Spritesheet* Spritesheet;
typedef struct _Sprite* Sprite;

//initialise/quit drawing utilities
bool Draw_init();
void Draw_quit();

//Sprite methods
Sprite Sprite_createSprite(char* pathToSpriteSheet, int sW, int sH, int numStates, int* framesPerState);
void Sprite_destroySprite(Sprite s);
void Sprite_isFullscreen(Sprite s, bool fullscreen);
void Sprite_posByCentre(Sprite s, bool PBC);
void Sprite_setSpriteInWorldDims(Sprite s, int w, int h);
void Sprite_setSpriteInWorldPosRef(Sprite s, int* x, int* y, int* z);
void Sprite_setSpriteStateRef(Sprite s, int* state);
void Sprite_renderSprite(Sprite s);

//render all visible sprites to screen in order of creation
void Draw_renderScene();

#endif // _DRAW_H_
