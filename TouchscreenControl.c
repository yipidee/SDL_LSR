#include "TouchscreenControl.h"

//constants
static const Vec3D NOTARGET = {-1,-1,-1};
static const Vec3D NOTINIT = {-2,-2,-2};
static const SDL_FingerID UNTOUCHED = -1;

// globally viewable everywhere including header file
bool TS_resetKickSwitch = false;

// module globals
static bool initialized = false;
static int MaxMag;
static GameState* gamestate;
static Vec3D target;
static Vec3D ballTarget;
// holders for current finger IDs
static SDL_FingerID currFinger;
static SDL_FingerID ballFinger;

// static func defs
static bool _FingerDown(SDL_Event* e);
static bool _FingerUp(SDL_Event* e);
static bool _FingerMove(SDL_Event* e);

Rect getBallTouchArea()
{
    Rect res;
    Vec3D pos = GO_getPos(gamestate->ball);
    res.x = pos.i - (gamestate->ball->BCirc.r + 10);
    res.y = pos.j - (gamestate->ball->BCirc.r + 10);
    res.w = (gamestate->ball->BCirc.r + 10) * 2;
    res.h = res.w;
    return res;
}

// init touchscreen control module
void TS_init(GameState* gs, int maxMag)
{
    currFinger = UNTOUCHED;
    ballFinger = UNTOUCHED;
    target = NOTARGET;
    ballTarget = NOTARGET;
    gamestate = gs;
    MaxMag = maxMag;
    initialized = true;
}

// get vectors representing current inputs
Vec3D TS_getDirInput()
{
    if(currFinger == UNTOUCHED)return VECTOR_ZERO;
    target.k = 0;
    Vec3D res = VECTOR_ZERO;
    if(initialized)
    {
        Vec3D playerPos;
        playerPos = Player_getPos(gamestate->players[0]);
        res = Vec3D_subtract(target, playerPos);
        /*if(Vec3D_getMagnitude(res)>MaxMag)Vec3D_scalarMult(*/res = Vec3D_normalise(res);/*, MaxMag);
        res.i /= (float)MaxMag;
        res.j /= (float)MaxMag;*/
    }else
    {
        res = NOTINIT;
    }
    return res;
}

Vec3D TS_getKickInput()
{
    if(ballFinger == UNTOUCHED)return VECTOR_ZERO;
    ballTarget.k = 0;
    Vec3D res = VECTOR_ZERO;
    if(initialized)
    {
        Vec3D ballPos;
        ballPos = GO_getPos(gamestate->ball);
        res = Vec3D_subtract(ballTarget, ballPos);
        if(Vec3D_getMagnitude(res)>MaxMag)Vec3D_scalarMult(Vec3D_normalise(res), MaxMag);
        res.i /= (float)MaxMag;
        res.j /= (float)MaxMag;
    }else
    {
        res = NOTINIT;
    }
    return res;
}
void TS_resetKickInput()
{
    ballTarget = NOTARGET;
    ballFinger = UNTOUCHED;
    TS_resetKickSwitch = false;
}

bool TS_notInit(Vec3D v)
{
    if((v.i == -2) && (v.j == -2) && (v.k == -2))
    {
        return true;
    }else return false;
}

bool TS_noTarget(Vec3D v)
{
    if((v.i == -1) && (v.j == -1) && (v.k == -1))
    {
        return true;
    }else return false;
}

// event handling
bool TS_handleEvent(void* tc, SDL_Event* e)
{
    if(!initialized)return false;

    bool res = false;
    if(!e)return res;

    // reset kick if necessary
    if(TS_resetKickSwitch)TS_resetKickInput();

    switch(e->type){
        case SDL_FINGERDOWN:
            res = _FingerDown(e);
            break;
        case SDL_FINGERUP:
            res = _FingerUp(e);
            break;
        case SDL_FINGERMOTION:
            res = _FingerMove(e);
            break;
        default:
            break;
    }
    return res;
}

/////////////////////////////////////////////////////
///////             static funcs
/////////////////////////////////////////////////////

static bool _FingerDown(SDL_Event* e)
{
    bool res = false;
    SDL_FingerID touchF = e->tfinger.fingerId;

    // check if touch is ball touch
    if(Rect_containsPoint(getBallTouchArea(), e->tfinger.x, e->tfinger.y) /*&& ballFinger == UNTOUCHED*/)
    {
        //ball touch
        ballFinger = e->tfinger.fingerId;
        ballTarget.i = e->tfinger.x;
        ballTarget.j = e->tfinger.y;
        res = true;
    }
    else if(currFinger == UNTOUCHED || touchF == currFinger)
    {
        currFinger = e->tfinger.fingerId;
        target.i = e->tfinger.x;
        target.j = e->tfinger.y;
        res = true;
    }
    return res;
}

static bool _FingerUp(SDL_Event* e)
{
    bool res = false;
    if(e->tfinger.fingerId == currFinger)
    {
        target = NOTARGET;
        currFinger = UNTOUCHED;
        res = true;
    }else if(e->tfinger.fingerId == ballFinger)
    {
        ballTarget.i = e->tfinger.x;
        ballTarget.j = e->tfinger.y;
        res = true;
    }
    return res;
}

static bool _FingerMove(SDL_Event* e)
{
    bool res = false;
    if(e->tfinger.fingerId == currFinger)
    {
        target.i = e->tfinger.x;
        target.j = e->tfinger.y;
        res = true;
    }else if(e->tfinger.fingerId == ballFinger)
    {
        ballTarget.i = e->tfinger.x;
        ballTarget.j = e->tfinger.y;
        res = true;
    }
    return res;
}