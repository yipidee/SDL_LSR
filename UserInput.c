#include "UserInput.h"

typedef Input (*inputSource)(void);
inputSource getInput = NULL;

static bool initialized = false;
static bool onscreenRefsSet = false;
bool usingPhysicalController = false;

//local function prototypes
static Input getInputFromPhysCont();
static Input getInputFromAnalCont();
static void init();

//static refs to onscreen controls if being used
static AnalogueController *ac1, *ac2, *ac3;

Input UI_getUserInput()
{
    if(!initialized)init();
    return getInput();
}

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

///////////////////////////////////////////////////
////////     local functions
///////////////////////////////////////////////////

static Input getInputFromPhysCont()
{
    if(!initialized) init();
    Input i = {PhysCont_getLeftStickInput(),VECTOR_ZERO,VECTOR_ZERO};
    if(PhysCont_getShotMask())
    {
        i.shot = PhysCont_getRightStickInput();
        i.control = VECTOR_ZERO;
    }else
    {
        i.control = PhysCont_getRightStickInput();
        i.shot = VECTOR_ZERO;
    }
    return i;
}

static Input getInputFromAnalCont()
{
    if(!initialized) init();
    Input i = {VECTOR_ZERO,VECTOR_ZERO,VECTOR_ZERO};
    if(onscreenRefsSet)
    {
        i.direction = AnalCont_getCurrentInput(ac1);
        Vec3D cont = AnalCont_getCurrentInput(ac2);
        if(Vec3D_equal(cont, VECTOR_ZERO))
        {
            i.control = cont;
            i.shot = VECTOR_ZERO;
        }else
        {
            i.shot = cont;
            i.control = VECTOR_ZERO;
        }
    }else
    {
        printf("no onscreen control referenced");
    }
    return i;
}

static void init()
{
    if(PhysCont_PhysicalControllerPresent())
    {
        getInput = &getInputFromPhysCont;
        usingPhysicalController = true;
    }else
    {
        getInput = &getInputFromAnalCont;
    }
}
