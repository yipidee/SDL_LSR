#include <stdio.h>
#include "AnalogueController.h"

//helper meta functions to test whether mouse buttons pressed during move
#define isMouseBL(e) (((e)->motion.state&SDL_BUTTON_LMASK)==SDL_BUTTON_LMASK)
#define isMouseBR(e) (((e)->motion.state&SDL_BUTTON_RMASK)==SDL_BUTTON_RMASK)

//Protype static functions
static bool _MouseDown(AnalogueController* ac, SDL_Event* e);
static bool _MouseUp(AnalogueController* ac, SDL_Event* e);
static bool _MouseMove(AnalogueController* ac, SDL_Event* e);

//create controller
AnalogueController AnalCont_create(Controller_Mode mode)
{
    AnalogueController c;
    c.base = CIRCLE_NULL;
    c.knob = CIRCLE_NULL;
    c.touchableArea = RECT_NULL;
    c.mode = mode;
    c.pathToBImg = NULL;
    c.pathToKImg = NULL;
    c.baseImg = NULL;
    c.knobImg = NULL;
    c.evHan = &AnalCont_handleEvent;
    return c;
}

//sets the position of the controller in global coordinates
void AnalCont_setPosition(AnalogueController* ac, int x, int y)
{
    ac->base.x = x;
    ac->base.y = y;
    ac->knob.x = x;
    ac->knob.y = y;
}

// set diameter of controller view
void AnalCont_setSize(AnalogueController* ac, int d)
{
    ac->base.r = d/2;
    Rect r = {ac->base.x - ac->base.r, ac->base.y - ac->base.r, 2 * ac->base.r, 2 * ac->base.r};
    ac->touchableArea = r;
}

//set diameter of knob within view
void AnalCont_setKnobSize(AnalogueController* ac, int d)
{
    ac->knob.r = d/2;
}

//set path to base image
void AnalCont_setPathToBImg(AnalogueController* ac, char* path)
{
    ac->pathToBImg = path;
}

void AnalCont_setPathToKImg(AnalogueController* ac, char* path)
{
    ac->pathToKImg = path;
}

//returns a Rect represented touchable area of controller
Rect AnalCont_getTouchableArea(AnalogueController* ac)
{
    return ac->touchableArea;
}

//returns diameter of controller view
int AnalCont_getSize(AnalogueController* ac)
{
    return (ac->base.r * 2);
}

//returns diameter of knob within view
int AnalCont_getKnobSize(AnalogueController* ac)
{
    return (ac->knob.r * 2);
}

//get current input
Vec3D AnalCont_getCurrentInput(AnalogueController* ac)
{
    Vec3D in;
    Vec3D kp = {ac->knob.x,ac->knob.y,0};
    Vec3D bp = {ac->base.x,ac->base.y,0};
    in = Vec3D_subtract(kp, bp);
    float max = (float)(ac->base.r-ac->knob.r);
    in.i = (int)((float)in.i/max*100);
    in.j = (int)((float)in.j/max*100);
    return in;
}

//handle event
bool AnalCont_handleEvent(void* ac, SDL_Event* e)
{
    AnalogueController* a = ac;
    Uint32 eventType = e->type;
    switch(eventType){
        case SDL_MOUSEBUTTONDOWN:
            _MouseDown(a, e);
            break;
        case SDL_MOUSEBUTTONUP:
            _MouseUp(a, e);
            break;
        case SDL_MOUSEMOTION:
            _MouseMove(a, e);
            break;
    }
    return true;
}

/*****************************************************************
******************************************************************
********              Static methods                   ***********
******************************************************************
*****************************************************************/

static bool _MouseDown(AnalogueController* ac, SDL_Event* e)
{
    //immediately return if null pointer
    if(ac==NULL || e==NULL) return false;

    if(Rect_containsPoint(ac->touchableArea, e->button.x, e->button.y))
    {
        Rect inner = {ac->touchableArea.x+ac->knob.r,
                      ac->touchableArea.y+ac->knob.r,
                      ac->touchableArea.w-2*ac->knob.r,
                      ac->touchableArea.h-2*ac->knob.r};
        if(Rect_containsPoint(inner, e->button.x, e->button.y))
        {
            ac->knob.x = e->button.x;
            ac->knob.y = e->button.y;
        }else{
            Vec3D touchPos = {e->button.x, e->button.y, 0};
            Vec3D basePos = {ac->base.x, ac->base.y, 0};
            Vec3D delta = Vec3D_subtract(touchPos, basePos);
            delta = Vec3D_normalise(delta);
            delta = Vec3D_scalarMult(delta, (float)(ac->base.r-ac->knob.r)/1000);
            ac->knob.x = ac->base.x+delta.i;
            ac->knob.y = ac->base.y+delta.j;
        }
    }
    return true;
}

static bool _MouseUp(AnalogueController* ac, SDL_Event* e)
{
    //immediately return if null pointer
    if(ac==NULL || e==NULL) return false;

    ac->knob.x = ac->base.x;
    ac->knob.y = ac->base.y;
    return true;
}

static bool _MouseMove(AnalogueController* ac, SDL_Event* e)
{
    //immediately return if null pointer
    if(ac==NULL || e==NULL) return false;

    if(isMouseBL(e))
    {
        ac->knob.x = e->motion.x;
        ac->knob.y = e->motion.y;
        SDL_Event myEvent;
        if(Rect_containsPoint(ac->touchableArea, ac->knob.x, ac->knob.y))
        {
            myEvent.button.x = ac->knob.x;
            myEvent.button.y = ac->knob.y;
            _MouseDown(ac, &myEvent);
        }else
        {
            _MouseUp(ac, &myEvent);
        }

    }
    return true;
}
