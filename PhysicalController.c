#include <stdint.h>
#include <math.h>
#include "Constants.h"
#include "PhysicalController.h"

static SDL_Joystick* controller = NULL;
static SDL_Joystick* controller1 = NULL;
static bool isInitialised = false;
static int numControllers = 0;

//storage of controller inputs
static Vec3D leftStick, rightStick;
static Vec3D leftStick1, rightStick1;
static bool shotMask, shotMask1;

//initialise controller
bool PhysCont_init()
{
    if(!isInitialised)
    {
        if(SDL_NumJoysticks() < 1)
        {
            printf( "Warning: No joysticks connected!\n" );
        }else
        {
            numControllers = SDL_NumJoysticks();
            controller = SDL_JoystickOpen(0);
            controller1 = SDL_JoystickOpen(1);
            if( controller == NULL )
			{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}else
			{
			    leftStick = VECTOR_ZERO;
			    rightStick = VECTOR_ZERO;
			    shotMask = false;
			    leftStick1 = VECTOR_ZERO;
			    rightStick1 = VECTOR_ZERO;
			    shotMask1 = false;
			    isInitialised = true;
			}
        }
    }
    return isInitialised;
}

void PhysCont_deviceRemoved(SDL_Event* e)
{    if(e->jdevice.which == 0)
    {
        SDL_JoystickClose(controller);
        controller = NULL;
        numControllers--;
        if(numControllers < 1) isInitialised = false;
    }
    if(e->jdevice.which == 1)
    {
        SDL_JoystickClose(controller1);
        controller1 = NULL;
        numControllers--;
        if(numControllers < 1) isInitialised = false;
    }
}

void PhysCont_deviceAdded(SDL_Event* e)
{
    if(!isInitialised) PhysCont_init();
}

//handle joystick events
bool PhysCont_handleEvent(SDL_Event* e)
{
    bool handled = false;
    if(e->type == SDL_JOYAXISMOTION)
    {
        //x axis left stick
        if(e->jaxis.axis == 0)
        {
            Sint16 x = e->jaxis.value;
            if (abs(x) < CONS_CONTROLLER_DEADZONE) x = 0;
            if(e->jdevice.which == 0)leftStick.i = (double)x/32768.0;
            if(e->jdevice.which == 1)leftStick1.i = (double)x/32768.0;
            handled = true;
        }
        //y axis left stick
        else if(e->jaxis.axis == 1)
        {
            Sint16 y = e->jaxis.value;
            if (abs(y) < CONS_CONTROLLER_DEADZONE) y = 0;
            if(e->jdevice.which == 0)leftStick.j = (double)y/32768.0;
            if(e->jdevice.which == 1)leftStick1.j = (double)y/32768.0;
            //leftStick.j = (double)y/32768.0;
            handled = true;
        }
        //x axis right stick
        else if(e->jaxis.axis == 4)
        //else if(e->jaxis.axis == 2)
        {
            Sint16 x = e->jaxis.value;
            if (abs(x) < CONS_CONTROLLER_DEADZONE) x = 0;
            if(e->jdevice.which == 0)rightStick.j = (double)x/32768.0;
            if(e->jdevice.which == 1)rightStick1.j = (double)x/32768.0;
            //rightStick.j = (double)x/32768.0;
            handled = true;
        }
        //y axis right stick
        else if(e->jaxis.axis == 3)
        //else if(e->jaxis.axis == 3)
        {
            Sint16 y = e->jaxis.value;
            if (abs(y) < CONS_CONTROLLER_DEADZONE) y = 0;
            //rightStick.i = (double)y/32768.0;
            if(e->jdevice.which == 0)rightStick.i = (double)y/32768.0;
            if(e->jdevice.which == 1)rightStick1.i = (double)y/32768.0;
            handled = true;
        }
    }else if(e->type == SDL_JOYBUTTONDOWN)
    {
        if(e->jbutton.button == 5)
        //if(e->jbutton.button == 7)
        {
           if(e->jdevice.which == 0) shotMask = true;
           if(e->jdevice.which == 1) shotMask1 = true;
           handled = true;
        }
    }else if(e->type == SDL_JOYBUTTONUP)
    {
        //if(e->jbutton.button == 7)
        if(e->jbutton.button == 5)
        {
           if(e->jdevice.which == 0) shotMask = false;
           if(e->jdevice.which == 1) shotMask1 = false;
           //shotMask = false;
           handled = true;
        }
    }else if(e->type == SDL_JOYDEVICEREMOVED)
    {
        PhysCont_deviceRemoved(e);
    }else if(e->type == SDL_JOYDEVICEADDED)
    {
        PhysCont_deviceAdded(e);
    }
    return handled;
}

//get current input of left stick as Vec3D
Vec3D PhysCont_getLeftStickInput(int i)
{
    Vec3D res = VECTOR_ZERO;
    if(i==0)
    {
        res = Vec3D_getMagnitude(leftStick) > 1 ? Vec3D_normalise(leftStick) : leftStick;
    }else
    {
        res = Vec3D_getMagnitude(leftStick1) > 1 ? Vec3D_normalise(leftStick1) : leftStick1;
    }
    return res;
}

Vec3D PhysCont_getRightStickInput(int i)
{
    Vec3D res = VECTOR_ZERO;
    if(i==0)
    {
        res = Vec3D_getMagnitude(rightStick) > 1 ? Vec3D_normalise(rightStick) : rightStick;
    }else
    {
        res = Vec3D_getMagnitude(rightStick1) > 1 ? Vec3D_normalise(rightStick1) : rightStick1;
    }//   res = Vec3D_getMagnitude(rightStick) > 1 ? Vec3D_normalise(rightStick) : rightStick;
    return res;
}

bool PhysCont_getShotMask(int i)
{
    if(i == 0)
    {
        return shotMask;
    }else
    {
        return shotMask1;
    }
}
