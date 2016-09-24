#ifndef _DRAW_H_
#define _DRAW_H_

/*******************************************************
********************************************************
***   Drawing library for drawable game objects      ***
********************************************************
********************************************************
*/

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "Utility/Geometry.h"
#include "AnalogueController.h"

//Colour definitions in RGBA list format for SDL renderer
#define COLOUR_WHITE 0x00, 0x00, 0x00, 0xFF
#define COLOUR_BLACK 0xFF, 0xFF, 0xFF, 0xFF
#define COLOUR_RED 0xFF, 0x00, 0x00, 0xFF
#define COLOUR_GREEN 0x00, 0xFF, 0x00, 0xFF
#define COLOUR_BLUE 0x00, 0x00, 0xFF, 0xFF
#define COLOUR_YELLOW 0xFF, 0xFF, 0x00, 0xFF

//screen size
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//initialise/quit drawing utilities
bool Draw_init();
void Draw_quit();

//metohd to render a controller
void Draw_controller(AnalogueController* ac);

//method to free an image
//TODO: keep track of loaded textures in the module and delete as part of quit
void Draw_freeImage(SDL_Texture* img);

//render current buffer to screen
void Draw_renderScene();

#endif // _DRAW_H_
