#include <stdint.h>
#include "Constants.h"
#include "PhysicalController.h"

static SDL_Joystick* controller = NULL;
static bool isInitialised = false;
static int numControllers = 0;

//storage of controller inputs
static Vec3D leftStick, rightStick;
static bool shotMask;

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
            if( controller == NULL )
			{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}else
			{
			    leftStick = VECTOR_ZERO;
			    rightStick = VECTOR_ZERO;
			    shotMask = false;
			    isInitialised = true;
			}
        }
    }
    return isInitialised;
}

void PhysCont_deviceRemoved(SDL_Event* e)
{
    if(e->jdevice.which == 0)
    {
        SDL_JoystickClose(controller);
        controller = NULL;
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
        if(e->jaxis.which == 0)
        {
            Sint16 x = e->jaxis.value;
            if (x < CONS_CONTROLLER_DEADZONE) x = 0;
            leftStick.i = (double)x/32768.0;
            handled = true;
        }
        //y axis left stick
        else if(e->jaxis.which == 1)
        {
            Sint16 y = e->jaxis.value;
            if (y < CONS_CONTROLLER_DEADZONE) y = 0;
            leftStick.j = (double)y/32768.0;
            handled = true;
        }
        //x axis right stick
        else if(e->jaxis.which == 2)
        {
            Sint16 x = e->jaxis.value;
            if (x < CONS_CONTROLLER_DEADZONE) x = 0;
            rightStick.i = (double)x/32768.0;
            handled = true;
        }
        //y axis right stick
        else if(e->jaxis.which == 3)
        {
            Sint16 y = e->jaxis.value;
            if (y < CONS_CONTROLLER_DEADZONE) y = 0;
            rightStick.j = (double)y/32768.0;
            handled = true;
        }
    }else if(e->type == SDL_JOYBUTTONDOWN)
    {
        if(e->jbutton.which == 7)
        {
            shotMask = true;
            handled = true;
        }
    }else if(e->type == SDL_JOYBUTTONUP)
    {
        if(e->jbutton.which == 7)
        {
            shotMask = false;
            handled = true;
        }
    }
    return handled;
}

//get current input of left stick as Vec3D
Vec3D PhysCont_getLeftStickInput()
{
    return leftStick;
}

Vec3D PhysCont_getRightStickInput()
{
    return rightStick;
}

bool PhysCont_getShotMask()
{
    return shotMask;
}
