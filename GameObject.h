#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

/*******************************************************************
 *******************************************************************
 ******   Simple representation of in game object for physics ******
 *******************************************************************
 ******************************************************************/

#include <stdbool.h>
#include "Utility/Geometry.h"

typedef struct GameObject
{
    int id;                 //id assigned on creation
    Vec3D pos, vel, acc;    //vectors initialised to ZERO
    int rPos, rVel, rAcc;   //rotational properties initialised to 0
    Circle BCirc;             //bounding circle for collision detection
    int mass;
    bool isStationary;      //flag to indicate no need for physics update
} GameObject;

// create/destroy functions for game object
GameObject GO_createGameObject();

//setters for physics variables
void GO_setAcc(GameObject* go, Vec3D a);
void GO_setVel(GameObject* go, Vec3D v);
void GO_setPos(GameObject* go, Vec3D p);
void GO_setRPos(GameObject* go, int p);
void GO_setRVel(GameObject* go, int v);
void GO_setRAcc(GameObject* go, int a);
void GO_setStationary(GameObject* go, bool b);
void GO_setBCirc(GameObject* go, Circle c);
void GO_setMass(GameObject* go, int m);

//getters for physics variables
Vec3D GO_getAcc(const GameObject* go);
Vec3D GO_getVel(const GameObject* go);
Vec3D GO_getPos(const GameObject* go);
int GO_getRPos(const GameObject* go);
int GO_getRVel(const GameObject* go);
int GO_getRAcc(const GameObject* go);
int GO_getMass(const GameObject* go);

//collision detection function
bool GO_isInContact(GameObject go1, GameObject go2);
bool GO_isInBounds(GameObject go, Rect BoundingRect);

//movement functions
void GO_move(GameObject* go, Vec3D delta);

#endif // _GAMEOBJECT_H_
