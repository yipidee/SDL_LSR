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
#ifdef __ANDROID__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <stdbool.h>
#include "Utility/Geometry.h"
#include "AnalogueController.h"
#include "Constants.h"

typedef struct _Spritesheet* Spritesheet;
typedef struct _Sprite* Sprite;
typedef struct _TextLabel* TextLabel;

//initialise/quit drawing utilities
bool Draw_init();
void Draw_quit();
double Draw_getScalingFactor();

//Sprite methods
Sprite Sprite_createSprite(char* pathToSpriteSheet, int sW, int sH, int numStates, int* framesPerState);
void Sprite_destroySprite(Sprite s);
void Sprite_isFullscreen(Sprite s, bool fullscreen);
void Sprite_posByCentre(Sprite s, bool PBC);
void Sprite_setSpriteInWorldDims(Sprite s, int w, int h);
void Sprite_setSpriteInWorldPosRef(Sprite s, double* x, double* y, double* z);
void Sprite_setSpriteRotationRef(Sprite s, double* angle);
void Sprite_setSpriteStateRef(Sprite s, int* state);
void Sprite_renderSprite(Sprite s);
void Sprite_tickFrame(Sprite s);
int Sprite_getCurrFrame(Sprite s);
int* Sprite_getRateSetAddress(Sprite s);
void Sprite_setFrameRate(int* rate, int r);
#define Sprite_setFR(s, r) Sprite_setFrameRate(Sprite_getRateSetAddress((s)), (r))

//text label methods
TextLabel TL_createTextLabel(char* text, int x, int y);  //Creates a label with text at pos x and y
void TL_destroyTextLabel(TextLabel tl);  //Releases resources used by text label
void TL_renderTextLabel(TextLabel tl);  //Render label to screen
void TL_setText(TextLabel tl, char* text);  //Sets tl's text data
void TL_setFont(TextLabel tl, char* pathToFont);  //Set the font to be used for the TextLabel
SDL_Rect TL_getBoundRect(TextLabel tl);  //Get pointer to bounding rect
void TL_setFontSize(TextLabel tl, int fSize);  //Set font size
void TL_setX(TextLabel tl, int x);  //set x
void TL_setY(TextLabel tl, int y);  //set y
int TL_getX(TextLabel tl);  //get x
int TL_getY(TextLabel tl);  //get y
int TL_getWidth(TextLabel tl);  //get width of text label
int TL_getHeight(TextLabel tl);  //get Height of text label

//render all visible sprites to the buffer in order of creation
void Draw_renderScene();
void Draw_line(Vec3D p1, Vec3D p2);
void Draw_drawSceneBuffer();  //draw buffer to screen and clear content

//viewport functions
int Viewport_getWidth();
int Viewport_getHeight();

#endif // _DRAW_H_
