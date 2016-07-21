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
    Vec3D res = VECTOR_ZERO;
    res.i = v1.i + v2.i;
    res.j = v1.j + v2.j;
    res.k = v1.k + v2.k;
    return res;
}

//2d vector multiplication
Vec3D Vec3D_ScalarMult(Vec3D v, int scalar)
{
    Vec3D res = VECTOR_ZERO;
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


///////////////////////////////////////////////////
////           Rect
///////////////////////////////////////////////////

//greate circle with central coordinates and radius
Rect Rect_create(int x, int y, int w, int h)
{
    Rect r = {x, y, w, h};
    return r;
}

//set/get properties
void Rect_setX(Rect* r, int x)
{
    r->x=x;
}

void Rect_setY(Rect* r, int y)
{
    r->y=y;
}

void Rect_setW(Rect* r, int w)
{
    r->W=W;
}

void Rect_setH(Rect* r, int h)
{
    r->H=H;
}

int Rect_getX(const Rect* r)
{
    return r->x;
}

int Rect_getY(const Rect* r)
{
    return r->y;
}

int Rect_getW(const Rect* r)
{
    return r->w;
}

int Rect_getH(const Rect* r)
{
    return r->h;
}


//move Rect
void Rect_translate(Rect* r, Vec3D delta)
{
    r->x+=delta.i;
    r->y+=delta.j
}

//returns whether rect contains point P
bool Rect_containsPoint(Rect r, x, y)
{
    bool res = false;
    if((x>=r.x&&x<=(r.x+r.w))&&(y>=r.y&&y<=(r.y+r.h)))res = true;
    return res;
}

//returns whether rect contains circle
bool Rect_containsCircle(Rect r, Circle c)
{
    Rect r2 = {r.x+c.r, r.y+c.r, r.w-2*c.r, r.h-2*c.r};
    return Rect_containsPoint(r2, c.x, c.y);
}


//interRect collision
bool Rect_inCollision(Rect r1, Rect r2)
{
    bool res=false;
    if((((r2.x-r1.x)<=r1.w)||(r1.x>r2.x && r1.x<(r2.x+r2.w)))&&(((r2.y-r1.y)<=r1.h)||(r1.y>r2.y && r1.y<(r2.y+r2.h)))) res=true;
    return res;
}


///////////////////////////////////////////////////
////           Circle
///////////////////////////////////////////////////

//greate circle with central coordinates and radius
Circle Circle_create(int x, int y, int r)
{
    Circle c = {x, y, r};
    return c;
}

//set/get properties
void Circle_setX(Circle* c, int x)
{
    c->x=x;
}
void Circle_setY(Circle* c, int y)
{
    c->y=y;
}
void Circle_setR(Circle* c, int r)
{
    c->r=r;
}
int Circle_getX(const Circle* c)
{
    return c->x;
}
int Circle_getY(const Circle* c)
{
    return c->y;
}
int Circle_getR(const Circle* c)
{
    return c->r;
}

//move circle
void Circle_translate(Circle* c, Vec3D delta)
{
    c->x+=delta.i;
    c->y+=delta.j;
}

//intercircle collision
bool Circle_inCollision(Circle c1, Circle c2)
{
    bool res = false;
    int dx = c1.x-c2.x;
    int dy = c1.y-c2.y;
    int rr = c1.r+c2.r;
    if(rr*rr >= (dx*dx + dy*dy))res=true;
    return res;
}

//return whether circle contains point
bool Circle_containsPoint(Circle c, x, y)
{
    bool res = false;
    int dx = c.x-x;
    int dy = c.y-y;
    if(c.r*c.r >= (dx*dx + dy*dy))res = true;
    return res;
}

//circle-square collision
bool Circle_inCollisionRect(Circle c, Rect r)
{
    Rect r1 = {r.x-c.r, r.y, r.w+2*c.r, r.h};
    Rect r2 = {r.x, r.y-c.r, r.w, r.h+2*c.r};
    if(Rect_containsPoint(r1, c.x, c.y)||Rect_containsPoint(r2, c.x, c.y))return true;

    Circle c1 = {r.x, r.y, c.r};
    Circle c2 = {r.x+r.w, r.y, c.r};
    Circle c3 = {r.x, r.y+r.h, c.r};
    Circle c4 = {r.x+r.w, r.y+r.h, c.r};
    if(Circle_containsPoint(c1, c.x, c.y)||Circle_containsPoint(c2, c.x, c.y)||
       Circle_containsPoint(c3, c.x, c.y)||Circle_containsPoint(c4, c.x, c.y))return true;

    return false;
}
