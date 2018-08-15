#include "UserInput.h"

typedef Input (*inputSource)(int i);
inputSource getInput = NULL;

static bool initialized = false;
static bool onscreenRefsSet = false;
bool usingPhysicalController = false;

//local function prototypes
static Input getInputFromPhysCont();
//static Input getInputFromAnalCont();
static Input getInputFromTouchscreen();
static void init();

//static refs to onscreen controls if being used
static AnalogueController *ac1, *ac2, *ac3;

Input UI_getUserInput(int i)
{
    if(!initialized)init();
    return getInput(i);
}

/***DOES NOT BELONG HERE!!!!!!!!!***/
void UI_setOnscreenControlRef(AnalogueController* a1, AnalogueController* a2, AnalogueController* a3)
{
    ac1 = a1;
    ac2 = a2;
    ac3 = a3;
    onscreenRefsSet = true;
}

Vec3D UI_getDirVec(Input i)
{
    return i.direction;
}

Vec3D UI_getConVec(Input i)
{
    return i.control;
}

Vec3D UI_getShotVec(Input i)
{
    return i.shot;
}

bool UI_noInput(Input i)
{
    return ((Vec3D_equal(i.direction, INPUT_NULL.direction))&&(Vec3D_equal(i.control, INPUT_NULL.control))
            &&(Vec3D_equal(i.shot, INPUT_NULL.shot)));
}


///////////////////////////////////////////////////
////////     local functions
///////////////////////////////////////////////////

static Input getInputFromPhysCont(int x)
{
    if(!initialized) init();
    Input i = {PhysCont_getLeftStickInput(x),VECTOR_ZERO,VECTOR_ZERO};
    if(PhysCont_getShotMask(x))
    {
        i.shot = PhysCont_getRightStickInput(x);
        i.control = VECTOR_ZERO;
    }else
    {
        i.control = PhysCont_getRightStickInput(x);
        i.shot = VECTOR_ZERO;
    }
    return i;
}
/*
static Input getInputFromAnalCont()
{
    if(!initialized) init();
    Input i = {VECTOR_ZERO,VECTOR_ZERO,VECTOR_ZERO};
    if(onscreenRefsSet)
    {
        i.direction = AnalCont_getCurrentInput(ac1);
        i.control = AnalCont_getCurrentInput(ac3);
        if(Vec3D_isZero(i.control))i.shot = AnalCont_getCurrentInput(ac2);
    }else
    {
        printf("no onscreen control referenced");
    }
    return i;
}
*/
static Input getInputFromTouchscreen()
{
    if(!initialized) init();
    Input i = INPUT_NULL;
    
    //get direction input
    i.direction = TS_getDirInput();
    if(TS_notInit(i.direction)||TS_noTarget(i.direction)) i.direction = VECTOR_ZERO;
    
    //get kick input
    Vec3D kick = TS_getKickInput();
    if(!(TS_notInit(kick)||TS_noTarget(kick)))
    {
        if(Vec3D_getMagnitude(kick)>0.5)
        {
            i.shot = kick;
            i.control = VECTOR_ZERO;
        }else
        {
            i.control = Vec3D_scalarMult(kick, 1.5);
            i.shot = VECTOR_ZERO; 
        }
    }else
    {
        i.shot = VECTOR_ZERO;
        i.control = VECTOR_ZERO;
    }
    return i;
}

static Input dummy()
{
    Input i = INPUT_NULL;
    return i;
}

static void init()
{
//force on screen control for touch devices
#ifndef __ANDROID__
    if(PhysCont_PhysicalControllerPresent())
    {
        getInput = &getInputFromPhysCont;
        usingPhysicalController = true;
    }else
    {
        getInput = &dummy;/*
        printf("No Physical controllers connected. Exiting.\n");
        exit(-1);*/
    }/*else
    {
        getInput = &getInputFromAnalCont;
    }*/
#else
    getInput = &getInputFromTouchscreen;
#endif
}
