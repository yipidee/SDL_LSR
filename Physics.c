#include <stdbool.h>
#include <math.h>
#include "Utility/Geometry.h"
#include "GameObject.h"
#include "Constants.h"
#include "Physics.h"


bool Phys_inCollision(GameObject go1, GameObject go2)
{
    return Circle_inCollision(go1.BCirc, go2.BCirc);
}

Vec3D Phys_getClosestPointFromPointOnLine(Line l, Vec3D p)
{
    Vec3D closestP = VECTOR_ZERO;
    if(Line_isVertical(l))
    {
        closestP.i = l.p1.i;
        closestP.j = p.j;
        closestP.k = p.k;
        return closestP;
    }else if(Line_isHorizontal(l))
    {
        closestP.j = l.p1.j;
        closestP.i = p.i;
        closestP.k = p.k;
        return closestP;
    }else
    {
        double grad = Line_getGradient(l);
        double closestYonLine = 0;
        double closestXonLine = 0;
        closestXonLine = (p.j-l.p1.j)/2/grad + l.p1.i - p.i;
        closestYonLine = p.j-grad*(closestXonLine-p.i);
        closestP.i = closestXonLine;
        closestP.j = closestYonLine;
        closestP.k = p.k;
        return closestP;
    }
}

double Phys_getDistanceOfPointFromLine(Line l, Vec3D p)
{
    if(Line_isVertical(l))
    {
        return (fabs(l.p1.i-p.i));
    }else if(Line_isHorizontal(l))
    {
        return (fabs(l.p1.j-p.j));
    }else
    {
        Vec3D closestP = Phys_getClosestPointFromPointOnLine(l, p);
        Vec3D closestVec = Vec3D_subtract(closestP, p);
        return Vec3D_getMagnitude(closestVec);
    }
}

bool Phys_inCollisionWithLine(GameObject go, Line l)
{
    Circle c = go.BCirc;
    Vec3D p = {c.x,c.y,0};
    return (c.r >= Phys_getDistanceOfPointFromLine(l, p));
}

void Phys_conservationMomentumCollision1D(GameObject* go1, GameObject* go2, float COR)
{
    Vec3D u1, u2, v1, v2, initialMomentum;
    float m1, m2;
    //Mv1+mv2
    u1 = GO_getVel(go1);
    m1 = GO_getMass(go1);
    u2 = GO_getVel(go2);
    m2 = GO_getMass(go2);
    initialMomentum = Vec3D_add(Vec3D_scalarMult(u1, m1),Vec3D_scalarMult(u2, m2));
    //printf("initial momentum ");Vec3D_print(initialMomentum);
    float totalMass = m1 + m2;
    float ebym2 = COR * m2;
    Vec3D tmp = Vec3D_subtract(u2, u1);
    tmp = Vec3D_scalarMult(tmp, ebym2);
    tmp = Vec3D_add(initialMomentum, tmp);
    v1 = Vec3D_scalarMult(tmp, (1.0/totalMass));
    tmp = Vec3D_subtract(u1, u2);
    tmp = Vec3D_scalarMult(tmp, COR);
    v2 = Vec3D_add(v1, tmp);
    GO_setVel(go1, v1);
    //printf("ball initial vel ");Vec3D_print(go2->vel);printf("\n");
    GO_setVel(go2, v2);
    //printf("ball final vel ");Vec3D_print(go2->vel);printf("\n");
}

void Phys_conservationMomentumCollision2D(GameObject* go1, GameObject* go2, float COR)
{
    if(!(Vec3D_equal(go1->vel, VECTOR_ZERO)&&Vec3D_equal(go2->vel, VECTOR_ZERO)))
    {
        Vec3D dirOfCollision = Vec3D_subtract(go2->pos, go1->pos);
        dirOfCollision = Vec3D_normalise(dirOfCollision);
        Vec3D perpToCollision = Vec3D_getUnitNormal(dirOfCollision);

        //get component of vel in direction of collision
        double  alpha1;
        alpha1 = Vec3D_equal(go1->vel, VECTOR_ZERO) ? 1 : Vec3D_getCosAlpha(go1->vel, dirOfCollision);
        double  alpha2;
        alpha2 = Vec3D_equal(go2->vel, VECTOR_ZERO) ? 1 : Vec3D_getCosAlpha(go2->vel, Vec3D_scalarMult(dirOfCollision, -1));

        //velocities precollision in cirection of collision
        Vec3D u1DOC = Vec3D_scalarMult(dirOfCollision, Vec3D_getMagnitude(go1->vel) * alpha1);
        Vec3D u2DOC = Vec3D_scalarMult(dirOfCollision, Vec3D_getMagnitude(go2->vel) * alpha2);

        alpha1 = Vec3D_equal(go1->vel, VECTOR_ZERO) ? 1 : Vec3D_getCosAlpha(go1->vel, perpToCollision);
        alpha2 = Vec3D_equal(go2->vel, VECTOR_ZERO) ? 1 : Vec3D_getCosAlpha(go2->vel, Vec3D_scalarMult(perpToCollision, -1));

        //velocities precollision perp to cirection of collision
        Vec3D u1PTC = Vec3D_scalarMult(perpToCollision, Vec3D_getMagnitude(go1->vel) * alpha1);
        Vec3D u2PTC = Vec3D_scalarMult(perpToCollision, Vec3D_getMagnitude(go2->vel) * alpha2);

        go1->vel = u1DOC;
        go2->vel = u2DOC;

        //collision in dir of collision
        Phys_conservationMomentumCollision1D(go1, go2, COR);

        //resolve into global coords again
        GO_setVel(go1, Vec3D_add(go1->vel, u1PTC));
        GO_setVel(go2, Vec3D_add(go2->vel, u2PTC));
        GO_setAcc(go2, Vec3D_scalarMult(Vec3D_normalise(GO_getVel(go2)),CONS_BALL_COURT_DEACC));
    }else
    {
        //Vec3D origin = {25, 25, 0};
        //go2->vel = VECTOR_ZERO;
        //go2->acc = VECTOR_ZERO;
        //go2->pos=origin;
        //printf("both stopped\n");
        Vec3D dirOfContact = Vec3D_subtract(go2->pos, go1->pos);
        dirOfContact = Vec3D_scalarMult(Vec3D_normalise(dirOfContact), Vec3D_getMagnitude(dirOfContact)+1.0);
        GO_setPos(go2, Vec3D_add(GO_getPos(go1), dirOfContact));
        GO_setVel(go2, VECTOR_ZERO);
        GO_setAcc(go2, VECTOR_ZERO);
    }
    //Vec3D_print(go2->vel);printf("\n"); */
}
