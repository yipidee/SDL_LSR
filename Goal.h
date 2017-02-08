#ifndef _GOAL_H
#define _GOAL_H

#include "Utility/Geometry.h"
#include "GameObject.h"

typedef struct _Goal* Goal;

// greate a goal object
Goal Goal_createGoal(Vec3D p1, Vec3D p2, int d);

// destroy and existing goal object
void Goal_destroyGoal(Goal g);  //Must be called after Gameobjects destroyed!!

// get the width of a goal object, adjusted for post diameter
int Goal_getWidth(Goal g);

// get a vector normal to the goal
Vec3D Goal_getNormal(Goal g);

//determine if goal scored
bool Goal_scored(Goal g, GameObject* ball);

// get the gameobject representing left post
GameObject* Goal_getLPost(Goal g);

// get the gameobject representing right post
GameObject* Goal_getRPost(Goal g);

#endif // _GOAL_H
