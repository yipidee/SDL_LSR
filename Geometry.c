/*****************************************************************************
*** Elementry geometry library
******************************************************************************
**/

#include <math.h>
#include "Geometry.h"

/////////////////////////////////////
////  3D Vectors
/////////////////////////////////////

//2d vector addition
Vec3D Vec3D_add(Vec3D v1, Vec3D v2)
{
    Vec3D res = ZERO_VECTOR;
    res.i = v1.i + v2.i;
    res.j = v1.j + v2.j;
    res.k = v1.k + v2.k;
    return res;
}

//2d vector multiplication
Vec3D Vec3D_ScalarMult(Vec3D v, int scalar)
{
    Vec3D res = ZERO_VECTOR;
    res.i = v.i * scalar;
    res.j = v.j * scalar;
    res.k = v.k * scalar;
    return res;
}

int Vec3D_dotProduct(Vec3D v1, Vec3D v2)
{
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

Vec3D Vec3D_crossProduct(Vec3D v1, Vec3D v2)
{
    int coeff1, coeff2, coeff3;
    coeff1 = v1.y*v2.z-v2.y*v1.z;
    coeff2 = v1.x*v2.z-v2.x*v1.z;
    coeff3 = v1.x*v2.y-v2.x*v1.y;
    Vec3D res = {coeff1, coeff2, coeff3};
    return res;
}

//get cosine of angle between two vetors
double Vec3D_getCosAlpha(Vec3D v1, Vec3D v2)
{
    int DP = Vec3D_dotProduct(v1, v2);
    return ((double)DP/(Vec3D_getMagnitude(v1) * Vec3D_getMagnitude(v2)));
}

//get angle between vectors
double Vec3D_getAngle(Vec3D v1, Vec3D v2)
{
    return acos(Vec3D_getCosAlpha(v1, v2));
}

//get magnitude
double Vec3D_getMagnitude(Vec3D v)
{
    return sqrt(pow(v.x, 2)+pow(v.y, 2)+pow(v.z,2));
}

//check for parallel
bool Vec3D_isParallel(Vec3D v1, Vec3D v2)
{
    return (fabs(Vec3D_getCosAlpha(v1, v2))>0.995);
}

//check for perpendicular
bool Vec3D_isPerpendicular(Vec3D v1, Vec3D v2)
{
    return (fabs(Vec3D_getCosAlpha(v1, v2))<0.005);
}

//check for vector equality
bool Vec3D_equal(Vec3D v1, Vec3D v2)
{
    return ((v1.i==v2.i)&&(v1.j==v2.j)&&(v1.k==v2.k));
}

//is zero
bool Vec3D_isZero(Vec3D v)
{
    return Vec3D_equal(v, ZERO_VECTOR);
}

//normalise
Vec3D Vec3D_normalise(Vec3D v)
{
    double mag = Vec3D_getMagnitude(v);
    Vec3D res = {(int)((double)v.x/mag),
                 (int)((double)v.y/mag),
                 (int)((double)v.z/mag)
                };
    return res;
}
