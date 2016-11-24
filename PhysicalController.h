#ifndef _PHYSCONT_H
#define _PHYSCONT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "Utility/Geometry.h"

//initialise controller
bool PhysCont_init();

void PhysCont_deviceRemoved(SDL_Event* e);

void PhysCont_deviceAdded(SDL_Event* e);

//handle joystick events
bool PhysCont_handleEvent(SDL_Event* e);

//get current input of left stick as Vec3D
Vec3D PhysCont_getLeftStickInput();
Vec3D PhysCont_getRightStickInput();
bool PhysCont_getShotMask();

#endif // _PHYSCONT_H
