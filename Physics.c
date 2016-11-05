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
