#include "UserInput.h"

typedef Input (*inputSource)(void);
inputSource getInput = NULL;

static bool initialized = false;

//local function prototypes
static Input getInputFromPhysCont();
static Input getInputFromAnalCont();

void UI_init()
{
    getInput = PhysCont_PhysicalControllerPresent() ? &getInputFromPhysCont : &getInputFromAnalCont;
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

///////////////////////////////////////////////////
////////     local functions
///////////////////////////////////////////////////

static Input getInputFromPhysCont()
{
    Input i = {PhysCont_getLeftStickInput(),VECTOR_ZERO,VECTOR_ZERO};
    if(PhysCont_getShotMask())
    {
        i.shot = PhysCont_getRightStickInput();
    }else
    {
        i.control = PhysCont_getRightStickInput();
    }
    return i;
}

static Input getInputFromAnalCont()
{
    Input i = {VECTOR_ZERO,VECTOR_ZERO,VECTOR_ZERO};
    //i.direction = AnalCont_getCurrentInput();
    return i;
}
