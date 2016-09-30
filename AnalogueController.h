#ifndef _ANALOGUE_CONTROLLER_H
#define _ANALOGUE_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "Utility/Geometry.h"
#include "EventHandler.h"

typedef enum Controller_Mode
{
    ANALOGUE_MODE,
    DIGITAL_MODE
}Controller_Mode;

typedef struct AnalogueController
{
    Circle base, knob;
    Rect touchableArea;
    Controller_Mode mode;
    char* pathToBImg;
    char* pathToKImg;
    SDL_Texture* baseImg;
    SDL_Texture* knobImg;
    EventHandler evHan;

}AnalogueController;

//create controller
AnalogueController AnalCont_create(Controller_Mode mode);

//get current input
Vec3D AnalCont_getCurrentInput(AnalogueController* ac);

//handle event
bool AnalCont_handleEvent(void* ac, SDL_Event* e);

//set and get
void AnalCont_setPosition(AnalogueController* ac, int x, int y);
void AnalCont_setSize(AnalogueController* ac, int d);
void AnalCont_setKnobSize(AnalogueController* ac, int r);
void AnalCont_setPathToBImg(AnalogueController* ac, char* path);
void AnalCont_setPathToKImg(AnalogueController* ac, char* path);
int AnalCont_getSize(AnalogueController* ac);
int AnalCont_getKnobSize(AnalogueController* ac);
Rect AnalCont_getTouchableArea(AnalogueController* ac);

#endif // _ANALOGUE_CONTROLLER_H
