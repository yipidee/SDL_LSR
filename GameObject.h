#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

/*******************************************************************
 *******************************************************************
 ******   Simple representation of in game object for physics ******
 *******************************************************************
 ******************************************************************/

#include <stdbool.h>
#include "Utility/Geometry.h"

#define GO_backupOneTick(go) GO_move((go), Vec3D_scalarMult(GO_getVel((go)), -1))

typedef struct GameObject
{
    int id;                 //id assigned on creation
    Vec3D pos, vel, acc;    //vectors initialised to ZERO
    double rPos, rVel, rAcc;   //rotational properties initialised to 0
    Circle BCirc;             //bounding circle for collision detection
    int mass;
    bool isStationary, isOffset;      //flag to indicate no need for physics update
    Vec3D offset;
} GameObject;

// create/destroy functions for game object
GameObject* GO_createGameObject();
void GO_destroyGameObject(GameObject* go);
void GO_destroyAllGameObjects();
int GO_getNumGO();

//setters for physics variables
void GO_setAcc(GameObject* go, Vec3D a);
void GO_setVel(GameObject* go, Vec3D v);
void GO_setPos(GameObject* go, Vec3D p);
void GO_setRPos(GameObject* go, double p);
void GO_setRVel(GameObject* go, double v);
void GO_setRAcc(GameObject* go, double a);
void GO_setStationary(GameObject* go, bool b);
void GO_setBCirc(GameObject* go, Circle c);
void GO_setMass(GameObject* go, int m);
void GO_setOffset(GameObject* go, Vec3D Offset);
void GO_setOffsetPos(GameObject* go, Vec3D p);


//getters for physics variables
Vec3D GO_getAcc(const GameObject* go);
Vec3D GO_getVel(const GameObject* go);
Vec3D GO_getPos(const GameObject* go);
double GO_getRPos(const GameObject* go);
double GO_getRVel(const GameObject* go);
double GO_getRAcc(const GameObject* go);
int GO_getMass(const GameObject* go);
bool GO_isStationary(const GameObject* go);

//collision detection function
bool GO_isInContact(GameObject* go1, GameObject* go2);
bool GO_isInBounds(GameObject* go, Rect BoundingRect);

//whether a normal physics update will result in a change in direction
bool GO_changesXDirectionInNextTick(const GameObject* go);
bool GO_changesYDirectionInNextTick(const GameObject* go);
bool GO_changesZDirectionInNextTick(const GameObject* go);
void GO_zeroReversedDirections(GameObject* go);

//movement functions
void GO_move(GameObject* go, Vec3D delta);

#endif // _GAMEOBJECT_H_
