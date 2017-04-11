#ifndef _ANALOGUE_CONTROLLER_H
#define _ANALOGUE_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#ifdef __ANDROID__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "Utility/Geometry.h"
#include "EventHandler.h"
#include "Constants.h"

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
    EventHandler evHan;
    SDL_FingerID currFinger;
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
int AnalCont_getSize(AnalogueController* ac);
int AnalCont_getKnobSize(AnalogueController* ac);
Rect AnalCont_getTouchableArea(AnalogueController* ac);

#endif // _ANALOGUE_CONTROLLER_H
