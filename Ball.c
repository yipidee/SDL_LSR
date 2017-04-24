#include "Ball.h"
#include "GameState.h"

typedef struct {
    GameObject go;
    Rect touchableArea;
}_Ball;

Ball Ball_createBall(GameObject go)
{
    Ball b;
    b.go = go;
    b.touchableArea = {};
    return b;
}

GameObject Ball_getGO(Ball* b)
{
    return b->go;
}

void Ball_move(Ball* b, Vec3D v)
{
    GO_move(b->go, v);
    b->touchableArea.i += v.i;
    b->touchableArea.j += v.j;
    b->touchableArea.k += v.k;
}

bool Ball_handleEvent(void* b, SDL_Event* e);
