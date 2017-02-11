#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "GameObject.h"
#include "Utility/Geometry.h"

//Game object physics
bool Phys_inCollision(GameObject* go1, GameObject* go2);
void Phys_conservationMomentumCollision1D(GameObject* go1, GameObject* go2, float COR);
void Phys_conservationMomentumCollision2D(GameObject* go1, GameObject* go2, float COR);
void Phys_appliedImpulse2D(GameObject* go, Vec3D impulse);
void Phys_boundaryCollision(GameObject* go, Rect r);
void Phys_boundaryAdjust(GameObject* go, Rect r);
void Phys_adjustForCollisionWithStatObject(GameObject* go1, GameObject* go2);

//Game object geometry
bool Phys_inCollisionWithBoundary(GameObject* go, Rect r);

#endif // PHYSICS_H_
