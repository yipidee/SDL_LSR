#ifndef _ANALOGUE_CONTROLLER_H
#define _ANALOGUE_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "Utility/Geometry.h"

typedef bool (*EventHandler)(AnalogueController*, SDL_Event*);

typedef enum Controller_Mode
{
    ANALOGUE_MODE,
    DIGITAL_MODE
}Controller_Mode;

typedef struct AnalogueController
{
    Circle base, knob;
    Rect touchableArea;
    EventHandler evHand;
    Controller_Mode mode;
    bool isPressed;
}AnalogueController;

//create controller
AnalogueController AnalCont_create();

//get current input
uint32_t AnalCont_getCurrentInput(AnalogueController* ac);

//set and get
void AnalCont_setPosition(AnalogueController* ac, int x, int y);
void AnalCont_setSize(AnalogueController* ac, int r);
void AnalCont_setKnobSize(AnalogueController* ac, int r);
void AnalCont_setTouchableArea(AnalogueController* ac, Rect rect);
void AnalCont_setTouchHandler(AnalogueController* ac, EventHandler handler);
void AnalCont_setPressed(AnalogueController* ac, bool isPressed);
int AnalCont_getSize(AnalogueController* ac);
int AnalCont_getKnobSize(AnalogueController* ac);
Rect AnalCont_getTouchableArea(AnalogueController* ac);
bool AnalCont_getPressed(AnalogueController* ac);

#endif // _ANALOGUE_CONTROLLER_H
