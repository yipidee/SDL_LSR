#include <stdint.h>
#include "GameObject.h"

static int id_counter = 0;

// create functions for game object
GameObject GO_createGameObject()
{
    GameObject go;
    go.id = id_counter;
    ++id_counter;
    go.pos = VECTOR_ZERO;
    go.vel = VECTOR_ZERO;
    go.acc = VECTOR_ZERO;
    go.rPos = 0;
    go.rVel = 0;
    go.rAcc = 0;
    go.mass = 1;
    go.isStationary = true;
    go.BCirc = Circle_create(0, 0, 1);
    return go;
}

//collision detection function
bool GO_isInContact(GameObject go1, GameObject go2)
{
    return Circle_inCollision(go1.BCirc, go2.BCirc);
}

//returns whether a circle is completely enclosed within a rect
bool GO_isInBounds(GameObject go, Rect BoundingRect)
{
    return Rect_containsCircle(BoundingRect, go.BCirc);
}

void GO_move(GameObject* go, Vec3D delta)
{
    go->pos = Vec3D_add(go->pos, delta);
    go->BCirc.x = go->pos.i;
    go->BCirc.y = go->pos.j;
}

//setters for physics variables
void GO_setAcc(GameObject* go, Vec3D a)
{
    go->acc = a;
}

void GO_setVel(GameObject* go, Vec3D v)
{
    go->vel = v;
}

void GO_setPos(GameObject* go, Vec3D p)
{
    go->pos = p;
}

void GO_setRPos(GameObject* go, int p)
{
    go->rPos = p;
}

void GO_setRVel(GameObject* go, int v)
{
    go->rVel = v;
}

void GO_setRAcc(GameObject* go, int a)
{
    go->rAcc = a;
}

void GO_setStationary(GameObject* go, bool b)
{
    go->isStationary = b;
}

void GO_setBCirc(GameObject* go, Circle c)
{
    go->BCirc = c;
    go->BCirc.x = go->pos.i;
    go->BCirc.y = go->pos.j;
}

void GO_setMass(GameObject* go, int m)
{
    go->mass = m;
}

//getters for physics variables
Vec3D GO_getAcc(const GameObject* go)
{
    return go->acc;
}

Vec3D GO_getVel(const GameObject* go)
{
    return go->vel;
}

Vec3D GO_getPos(const GameObject* go)
{
    return go->pos;
}

int GO_getRPos(const GameObject* go)
{
    return go->rPos;
}

int GO_getRVel(const GameObject* go)
{
    return go->rVel;
}

int GO_getRAcc(const GameObject* go)
{
    return go->rAcc;
}

int GO_getMass(const GameObject* go)
{
    return go->mass;
}
