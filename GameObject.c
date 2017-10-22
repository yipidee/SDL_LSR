#include <stdint.h>
#include <stdlib.h>
//#include <stdbool.h>
#include "GameObject.h"
#include "Utility/List.h"

static int id_counter = 0;
static List GO_LIST;
static bool GO_LIST_INITIALIZED = false;

// create functions for game object
GameObject* GO_createGameObject()
{
    if(!GO_LIST_INITIALIZED)
    {
        List_new(&GO_LIST, sizeof(GameObject), NULL);
        GO_LIST_INITIALIZED = true;
    }

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
    go.isStationary = false;
    go.BCirc = Circle_create(0, 0, 1);
    go.offset = VECTOR_ZERO;
    go.isOffset = false;

    List_append(&GO_LIST, &go);
    return (GameObject*)GO_LIST.tail->data;
}

void GO_destroyGameObject(GameObject* go)
{
    free(go);
}

void GO_destroyAllGameObjects()
{
    List_destroy(&GO_LIST);
    GO_LIST_INITIALIZED = false;
}

int GO_getNumGO()
{
    return id_counter;
}

//collision detection function
bool GO_isInContact(GameObject* go1, GameObject* go2)
{
    return Circle_inCollision(go1->BCirc, go2->BCirc);
}

//returns whether a circle is completely enclosed within a rect
bool GO_isInBounds(GameObject* go, Rect BoundingRect)
{
    return Rect_containsCircle(BoundingRect, go->BCirc);
}

//fix an overlap between 2 objects, first object gets moved
void GO_fixOverlap(GameObject* go1, GameObject* go2)
{
    if(GO_isInContact(go1, go2))
    {
        Vec3D dirOfCol = Vec3D_subtract(GO_getPos(go1), GO_getPos(go2));
        double dist = Vec3D_getMagnitude(dirOfCol);
        dirOfCol = Vec3D_normalise(dirOfCol);
        double distToAdjust = go1->BCirc.r + go2->BCirc.r - dist;
        GO_setPos(go1, Vec3D_add(GO_getPos(go1), Vec3D_scalarMult(dirOfCol, distToAdjust)));
    }
}

void GO_move(GameObject* go, Vec3D delta)
{
    go->pos = Vec3D_add(go->pos, delta);
    go->BCirc.x = go->pos.i;
    go->BCirc.y = go->pos.j;
}

bool GO_inContactWithBoundary(GameObject* go, Rect r)
{
    return !Rect_containsCircle(r, go->BCirc);
}

bool GO_changesXDirectionInNextTick(const GameObject* go)
{
    bool res = false;
    if(Vec3D_equal(go->acc, VECTOR_ZERO))
    {
        res = false;
    }else
    {
        Vec3D delta = Vec3D_add(go->vel, go->acc);
        if(((go->vel.i >= 0) && (delta.i <= 0)) || ((go->vel.i <= 0) && (delta.i >= 0))) res = true;
    }
    return res;
}

bool GO_changesYDirectionInNextTick(const GameObject* go)
{
    bool res = false;
    if(Vec3D_equal(go->acc, VECTOR_ZERO))
    {
        res = false;
    }else
    {
        Vec3D delta = Vec3D_add(go->vel, go->acc);
        if(((go->vel.j >= 0) && (delta.j <= 0)) || ((go->vel.j <= 0) && (delta.j >= 0))) res = true;
    }
    return res;
}

bool GO_changesZDirectionInNextTick(const GameObject* go)
{
    bool res = false;
    if(Vec3D_equal(go->acc, VECTOR_ZERO))
    {
        res = false;
    }else
    {
        Vec3D delta = Vec3D_add(go->vel, go->acc);
        if(((go->vel.k >= 0) && (delta.k <= 0)) || ((go->vel.k <= 0) && (delta.k >= 0))) res = true;
    }
    return res;
}

void GO_zeroXvel(GameObject* go)
{
    go->vel.i=0;
}

void GO_zeroYvel(GameObject* go)
{
    go->vel.j=0;
}

void GO_zeroZvel(GameObject* go)
{
    go->vel.k=0;
}

void GO_zeroXacc(GameObject* go)
{
    go->acc.i=0;
}

void GO_zeroYacc(GameObject* go)
{
    go->acc.j=0;
}

void GO_zeroZacc(GameObject* go)
{
    go->acc.k=0;
}

void GO_zeroReversedDirections(GameObject* go)
{
    if(go!=NULL)
    {
        if(GO_changesXDirectionInNextTick(go)){GO_zeroXvel(go);GO_zeroXacc(go);}
        if(GO_changesYDirectionInNextTick(go)){GO_zeroYvel(go);GO_zeroYacc(go);}
        if(GO_changesZDirectionInNextTick(go)){GO_zeroZvel(go);GO_zeroZacc(go);}
    }
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
    go->BCirc.x = go->pos.i;
    go->BCirc.y = go->pos.j;
}

void GO_setOffsetPos(GameObject* go, Vec3D p)
{
    go->pos = p;
    if(go->isOffset)
    {
        GO_move(go, go->offset);
    }
    go->BCirc.x = go->pos.i;
    go->BCirc.y = go->pos.j;
}

void GO_setRPos(GameObject* go, double p)
{
    go->rPos = p;
}

void GO_setRVel(GameObject* go, double v)
{
    go->rVel = v;
}

void GO_setRAcc(GameObject* go, double a)
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

void GO_setOffset(GameObject* go, Vec3D Offset)
{
    go->isOffset = true;
    go->offset = Offset;
    GO_setOffsetPos(go, go->pos);
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

double GO_getRPos(const GameObject* go)
{
    return go->rPos;
}

double GO_getRVel(const GameObject* go)
{
    return go->rVel;
}

double GO_getRAcc(const GameObject* go)
{
    return go->rAcc;
}

int GO_getMass(const GameObject* go)
{
    return go->mass;
}

bool GO_isStationary(const GameObject* go)
{
    return go->isStationary;
}
