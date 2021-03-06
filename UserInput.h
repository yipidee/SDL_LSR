#ifndef _USERINPUT_H
#define _USERINPUT_H

#include "PhysicalController.h"
#include "AnalogueController.h"
#include "TouchscreenControl.h"
#include "Utility/Geometry.h"

typedef struct Input
{
    Vec3D direction, control, shot;
}Input;

static const Input INPUT_NULL = {{0,0,0}, {0,0,0}, {0,0,0}};

//get functions for input components
Vec3D UI_getDirVec(Input i);
Vec3D UI_getConVec(Input i);
Vec3D UI_getShotVec(Input i);
bool UI_noInput(Input i);

//get user input from control system
Input UI_getUserInput();
void UI_setOnscreenControlRef(AnalogueController* a1, AnalogueController* a2, AnalogueController* a3);

#endif // _USERINPUT_H
