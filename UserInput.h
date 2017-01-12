#ifndef _USERINPUT_H
#define _USERINPUT_H

#include "PhysicalController.h"
#include "AnalogueController.h"
#include "Utility/Geometry.h"

typedef struct Input
{
    Vec3D direction, control, shot;
}Input;

//get functions for input components
Vec3D UI_getDirVec(Input i);
Vec3D UI_getConVec(Input i);
Vec3D UI_getShotVec(Input i);

//get user input from control system
Input UI_getUserInput();

#endif // _USERINPUT_H
