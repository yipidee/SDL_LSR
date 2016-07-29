#include <stdio.h>
#include "AnalogueController.h"

//create controller
AnalogueController AnalCont_create()
{
    AnalogueController c;
    c.isPressed = false;
    c.base = Circle_create(50, 50, 50);
    c.knob = Circle_create(50, 50, 30);
    c.touchableArea = Rect_create(0, 0, 100, 100);
    c.mode = ANALOGUE_MODE;
    return c;
}

//get current input
uint32_t AnalCont_getCurrentInput(AnalogueController* ac);

//handle event
bool AnalCont_handleEvent(AnalogueController* ac, SDL_Event* e)
{
    Uint32 eventType = e->type;
    switch(eventType){
        case SDL_MOUSEBUTTONDOWN:
            ac->knob.x = e->button.x;
            ac->knob.y = e->button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            ac->knob.x = ac->base.x;
            ac->knob.y = ac->base.y;
            break;
        case SDL_MOUSEMOTION:
            if((e->motion.state&SDL_BUTTON_LMASK)==SDL_BUTTON_LMASK)
            {
                ac->knob.x += e->motion.xrel;
                ac->knob.y += e->motion.yrel;
            }
            break;
    }
    return true;
}

//set and get
void AnalCont_setPosition(AnalogueController* ac, int x, int y);
void AnalCont_setSize(AnalogueController* ac, int r);
void AnalCont_setKnobSize(AnalogueController* ac, int r);
void AnalCont_setTouchableArea(AnalogueController* ac, Rect rect);
void AnalCont_setPressed(AnalogueController* ac, bool isPressed);
int AnalCont_getSize(AnalogueController* ac);
int AnalCont_getKnobSize(AnalogueController* ac);
Rect AnalCont_getTouchableArea(AnalogueController* ac);
bool AnalCont_getPressed(AnalogueController* ac);

