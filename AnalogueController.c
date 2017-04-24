#include <stdio.h>
#include "AnalogueController.h"
#include "SDL_Helper.h"
//#include <android/log.h>

//Protype static functions
static bool _FingerDown(AnalogueController* ac, SDL_Event* e);
static bool _FingerUp(AnalogueController* ac, SDL_Event* e);
static bool _FingerMove(AnalogueController* ac, SDL_Event* e);

//create controller
AnalogueController AnalCont_create(Controller_Mode mode)
{
    AnalogueController c;
    c.base = CIRCLE_NULL;
    c.knob = CIRCLE_NULL;
    c.touchableArea = RECT_NULL;
    c.mode = mode;
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
    ac->touchableArea.x = ac->base.x-ac->base.r;
    ac->touchableArea.y = ac->base.y-ac->base.r;
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

//returns a Rect represented touchable area of controller
Rect AnalCont_getTouchableArea(AnalogueController* ac)
{
    return ac->touchableArea;
}

//returns diameter of controller view
int AnalCont_getSize(AnalogueController* ac)
{
    return (int)(ac->base.r * 2);
}

//returns diameter of knob within view
int AnalCont_getKnobSize(AnalogueController* ac)
{
    return (int)(ac->knob.r * 2);
}

//get current input
Vec3D AnalCont_getCurrentInput(AnalogueController* ac)
{
    Vec3D in;
    Vec3D kp = {ac->knob.x,ac->knob.y,0};
    Vec3D bp = {ac->base.x,ac->base.y,0};
    in = Vec3D_subtract(kp, bp);
    double max = ac->base.r-ac->knob.r;
    in.i = in.i/max;
    in.j = in.j/max;
    in.k = 0;
    return in;
}

//handle event
bool AnalCont_handleEvent(void* ac, SDL_Event* e)
{
    AnalogueController* a = ac;
    Uint32 eventType = e->type;
    switch(eventType){
        case SDL_FINGERDOWN:
            _FingerDown(a, e);
            break;
        case SDL_FINGERUP:
            _FingerUp(a, e);
            break;
        case SDL_FINGERMOTION:
            _FingerMove(a, e);
            break;
        default:
            return false;
    }
    //if(a->base.x==55)__android_log_print(ANDROID_LOG_VERBOSE, "TT_LSR", "touch at: %i, %i\n", (int)e->tfinger.x, (int)e->tfinger.y);
    //if(a->base.x==55)__android_log_print(ANDROID_LOG_VERBOSE, "TT_LSR", "knob at: %i, %i\n", (int)a->knob.x, (int)a->knob.y);
    return true;
}

/*****************************************************************
******************************************************************
********              Static methods                   ***********
******************************************************************
*****************************************************************/

static bool _FingerDown(AnalogueController* ac, SDL_Event* e)
{
    //immediately return if null pointer
    if(ac==NULL || e==NULL) return false;
    int touchx = (int)e->tfinger.x;
    int touchy = (int)e->tfinger.y;
    if(Rect_containsPoint(ac->touchableArea, touchx, touchy))
    {
        ac->currFinger = e->tfinger.fingerId;
        Vec3D centre = {ac->base.x,ac->base.y,0};
        Vec3D touch = {touchx,touchy,0};
        Vec3D a = Vec3D_subtract(touch, centre);
        double r = Vec3D_getMagnitude(a);
        if(r<=(ac->base.r-ac->knob.r))
        {
            ac->knob.x = touchx;
            ac->knob.y = touchy;
        }else{
            a = Vec3D_normalise(a);
            a = Vec3D_scalarMult(a, (float)(ac->base.r-ac->knob.r));
            ac->knob.x = ac->base.x+a.i;
            ac->knob.y = ac->base.y+a.j;
        }
    }
    return true;
}

static bool _FingerUp(AnalogueController* ac, SDL_Event* e)
{
    //immediately return if null pointer
    if(ac==NULL || e==NULL) return false;
    if(e->tfinger.fingerId == ac->currFinger)
    {
        ac->knob.x = ac->base.x;
        ac->knob.y = ac->base.y;
        return true;
    }else
    {
        return false;
    }
}

static bool _FingerMove(AnalogueController* ac, SDL_Event* e)
{
    //immediately return if null pointer
    if(ac==NULL || e==NULL) return false;

    if(e->tfinger.fingerId == ac->currFinger)
    {
        if(Rect_containsPoint(ac->touchableArea, (int)e->tfinger.x, (int)e->tfinger.y))
        {
            _FingerDown(ac, e);
        }else
        {
            //myEvent.type = SDL_FINGERUP;
            //myEvent.tfinger.fingerId = e->tfinger.fingerId;
            _FingerUp(ac, e);
        }
        return true;
    }else
    {
        return false;
    }
}
