#ifndef _BALL_H_
#define _BALL_H_

#include "GameObject.h"
#include "Utility/Geometry"

typedef struct _Ball Ball;

Ball Ball_createBall(GameObject go);
GameObject Ball_getGO(Ball* b);
void Ball_move(Ball* b, Vec3D v);

bool Ball_handleEvent(void* b, SDL_Event* e);

#endif