#ifndef PHYSICS_H_
#define PHYSICS_H_

bool Phys_inCollision(GameObject go1, GameObject go2);

Vec3D Phys_getClosestPointFromPointOnLine(Line l, Vec3D p);

double Phys_getDistanceOfPointFromLine(Line l, Vec3D p);

bool Phys_inCollisionWithLine(const GameObject* go, Line l);

void Phys_conservationMomentumCollision1D(GameObject* go1, GameObject* go2, float COR);

void Phys_conservationMomentumCollision2D(GameObject* go1, GameObject* go2, float COR);

void Phys_appliedImpulse2D(GameObject* go, Vec3D impulse);

bool Phys_inCollisionWithBoundary(GameObject* go, Rect r);

void Phys_boundaryCollision(GameObject* go, Rect r);

void Phys_boundaryAdjust(GameObject* go, Rect r);

#endif // PHYSICS_H_
