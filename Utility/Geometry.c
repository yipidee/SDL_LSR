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
Vec3D Vec3D_scalarMult(Vec3D v, float scalar)
{
    Vec3D res = VECTOR_ZERO;
    res.i = v.i * scalar;
    res.j = v.j * scalar;
    res.k = v.k * scalar;
    return res;
}

double Vec3D_dotProduct(Vec3D v1, Vec3D v2)
{
    return (v1.i*v2.i + v1.j*v2.j + v1.k*v2.k);
}

Vec3D Vec3D_crossProduct(Vec3D v1, Vec3D v2)
{
    int coeff1, coeff2, coeff3;
    coeff1 = v1.j*v2.k-v2.j*v1.k;
    coeff2 = v1.i*v2.k-v2.i*v1.k;
    coeff3 = v1.i*v2.j-v2.i*v1.j;
    Vec3D res = {coeff1, coeff2, coeff3};
    return res;
}

//get cosine of angle between two vetors
double Vec3D_getCosAlpha(Vec3D v1, Vec3D v2)
{
    double DP = Vec3D_dotProduct(v1, v2);
    return (DP/(Vec3D_getMagnitude(v1) * Vec3D_getMagnitude(v2)));
}

//get sine of angle between two vetors
double Vec3D_getSinAlpha(Vec3D v1, Vec3D v2)
{
    double DP = Vec3D_dotProduct(v1, v2);
    double productOfMagnitudes = Vec3D_getMagnitude(v1) * Vec3D_getMagnitude(v2);
    double opposite = sqrt(pow(productOfMagnitudes, 2)-pow(DP,2));
    return opposite/DP;
}

//get angle between vectors
double Vec3D_getAngle(Vec3D v1, Vec3D v2)
{
    return acos(Vec3D_getCosAlpha(v1, v2));
}

//get magnitude
double Vec3D_getMagnitude(Vec3D v)
{
    return sqrt(pow(v.i, 2)+pow(v.j, 2)+pow(v.k,2));
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
    Vec3D res = {v.i/mag,
                 v.j/mag,
                 v.k/mag
                };
    return res;
}

void Vec3D_print(Vec3D v)
{
    printf("%0.3fi %0.3fj %0.3fk\n", v.i, v.j, v.k);
}

Vec3D Vec3D_getUnitNormal(Vec3D v)
{
    Vec3D res = VECTOR_ZERO;
    v = Vec3D_normalise(v);
    res.i = v.j;
    res.j = -v.i;
    return res;
}

Vec3D Vec3D_rotateVectorByAlphaRad(Vec3D v, double alpha)
{
    Vec3D res = VECTOR_ZERO;
    double cosA = cos(alpha);
    double sinA = sin(alpha);
    res.i = v.i * cosA + v.j * sinA;
    res.j = v.i * -sinA + v.j * cosA;
    return res;
}

Vec3D Vec3D_rotateVectorByCosAlpha(Vec3D v, double cosAlpha)
{
    Vec3D res = VECTOR_ZERO;
    double sinA = sqrt(1 - cosAlpha*cosAlpha);
    res.i = v.i * cosAlpha + v.j * sinA;
    res.j = v.i * -sinA + v.j * cosAlpha;
    return res;
}


///////////////////////////////////////////////////
////           Rect
///////////////////////////////////////////////////

//greate circle with central coordinates and radius
Rect Rect_create(double x, double y, double w, double h)
{
    Rect r = {x, y, w, h};
    return r;
}

//set/get properties
void Rect_setX(Rect* r, double x)
{
    r->x=x;
}

void Rect_setY(Rect* r, double y)
{
    r->y=y;
}

void Rect_setW(Rect* r, double w)
{
    r->w=w;
}

void Rect_setH(Rect* r, double h)
{
    r->h=h;
}

double Rect_getX(const Rect* r)
{
    return r->x;
}

double Rect_getY(const Rect* r)
{
    return r->y;
}

double Rect_getW(const Rect* r)
{
    return r->w;
}

double Rect_getH(const Rect* r)
{
    return r->h;
}


//move Rect
void Rect_translate(Rect* r, Vec3D delta)
{
    r->x+=delta.i;
    r->y+=delta.j;
}

//returns whether rect contains point P
bool Rect_containsPoint(Rect r, double x, double y)
{
    bool res = false;
    if((x>=r.x)&&(x<=(r.x+r.w))&&(y>=r.y)&&(y<=(r.y+r.h)))
        res = true;
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

//converts to SDL_Rect
SDL_Rect Rect_getSDLRect(Rect r)
{
    SDL_Rect R = {r.x, r.y, r.w, r.h};
    return R;
}

bool Rect_equals(Rect r1, Rect r2)
{
    return ((r1.x==r2.x) && (r1.y==r2.y) && (r1.w==r2.w) && (r1.h==r2.h));
}


///////////////////////////////////////////////////
////           Circle
///////////////////////////////////////////////////

//greate circle with central coordinates and radius
Circle Circle_create(double x, double y, double r)
{
    Circle c = {x, y, r};
    return c;
}

//set/get properties
void Circle_setX(Circle* c, double x)
{
    c->x=x;
}
void Circle_setY(Circle* c, double y)
{
    c->y=y;
}
void Circle_setR(Circle* c, double r)
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
    double dx = c1.x-c2.x;
    double dy = c1.y-c2.y;
    double rr = c1.r+c2.r;
    if(rr*rr >= (dx*dx + dy*dy))res=true;
    return res;
}

//return whether circle contains point
bool Circle_containsPoint(Circle c, double x, double y)
{
    bool res = false;
    double dx = c.x-x;
    double dy = c.y-y;
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


///////////////////////////////////////////////////
////           Circle
///////////////////////////////////////////////////

double Line_getLength(Line l)
{
    Vec3D dVec = Vec3D_subtract(l.p2, l.p1);
    return Vec3D_getMagnitude(dVec);
}

double Line_getGradient(Line l)
{
    double m = LINE_IS_HORIZ_OR_VERT;
    if((l.p1.i == l.p2.i)||(l.p1.j == l.p2.j))return m;
    m = (l.p2.j-l.p1.j)/(l.p2.i - l.p1.i);
    return m;
}

bool Line_isVertical(Line l)
{
    return (l.p1.i == l.p2.i);
}

bool Line_isHorizontal(Line l)
{
    return (l.p1.j == l.p2.j);
}
