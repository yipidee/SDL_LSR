#include "AnalogueController.h"

//prototype event handling function
static bool _eh(AnalogueController* c, SDL_Event* e)
{
    if(e->type==SDL_MOUSEBUTTONDOWN)
    {
        c->knob.x = e->button.x;
    }
    return true;
}

//create controller
AnalogueController AnalCont_create()
{
    AnalogueController c;
    c.isPressed = false;
    c.base = Circle_create(50, 50, 50);
    c.knob = Circle_create(50, 50, 30);
    c.touchableArea = Rect_create(0, 0, 100, 100);
    c.mode = ANALOGUE_MODE;
    c.EventHandler = &_eh;
    return c;
}

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

