#ifndef PHYSICS_H_
#define PHYSICS_H_

bool Phys_inCollision(GameObject go1, GameObject go2);

Vec3D Phys_getClosestPointFromPointOnLine(Line l, Vec3D p);

double Phys_getDistanceOfPointFromLine(Line l, Vec3D p);

bool Phys_inCollisionWithLine(GameObject go, Line l);

void Phys_conservationMomentumCollision1D(GameObject* go1, GameObject* go2, float COR);

#endif // PHYSICS_H_
