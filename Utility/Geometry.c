/*****************************************************************************
*** Elementry geometry library
******************************************************************************
**/

#include <math.h>
#include "Geometry.h"

/////////////////////////////////////
////  3D Vectors
/////////////////////////////////////

//make vector from input
Vec3D Vec3D_makeVector(double i, double j, double k)
{
    Vec3D v;
    v.i = i;
    v.j = j;
    v.k = k;
    return v;
}

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
    double coeff1, coeff2, coeff3;
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
    // acos returns value between 0~PI, no indication of direction
    double angle = acos(Vec3D_getCosAlpha(v1, v2));
    short dirFix = 1;
    Vec3D v2Inv1 = Vec3D_transposeIntoRefCoordSystem(v2, v1);
    if(v2Inv1.j < 0) dirFix=-1;
    return angle*dirFix;
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
    if(mag == 0)return VECTOR_ZERO;
    Vec3D res = {v.i/mag,
                 v.j/mag,
                 v.k/mag
                };
    return res;
}

void Vec3D_print(Vec3D v)
{
    printf("%0.3fi %0.3fj %0.3fk", v.i, v.j, v.k);
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

Vec3D Vec3D_transposeIntoRefCoordSystem(Vec3D v, Vec3D refXaxis)
{
    Vec3D res = VECTOR_ZERO;
    double cosAlpha = Vec3D_getCosAlpha(refXaxis, v);
    res.i = Vec3D_getMagnitude(v) * cosAlpha;
    cosAlpha = Vec3D_getCosAlpha(Vec3D_negate(Vec3D_getUnitNormal(refXaxis)), v);
    res.j = Vec3D_getMagnitude(v) * cosAlpha;
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

bool Circle_inCollisionWithLine(Circle c, Line l, double offset)
{
    Vec3D p = {c.x,c.y,0};
    bool res = ((c.r + offset) >= Line_getDistanceOfPointFromLine(l, p));
    return res;
}

///////////////////////////////////////////////////
////                   Line
///////////////////////////////////////////////////

Line Line_makeLineFromPoints(Vec3D P1, Vec3D P2)
{
    Line l = {P1, P2};
    return l;
}

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

Vec3D Line_getClosestPointFromPointOnLine(Line l, Vec3D p)
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
        double m1 = Line_getGradient(l);
        double m2 = -1 / m1;
        double closestYonLine = 0;
        double closestXonLine = 0;
        closestXonLine = (m1*l.p1.i - m2*p.i + p.j - l.p1.j)/(m1 - m2);
        closestYonLine = m2*(closestXonLine - p.i) + p.j;
        closestP.i = closestXonLine;
        closestP.j = closestYonLine;
        closestP.k = p.k;
        return closestP;
    }
}

double Line_getDistanceOfPointFromLine(Line l, Vec3D p)
{
    if(Line_isVertical(l))
    {
        return (fabs(l.p1.i-p.i));
    }else if(Line_isHorizontal(l))
    {
        return (fabs(l.p1.j-p.j));
    }else
    {
        Vec3D closestP = Line_getClosestPointFromPointOnLine(l, p);
        Vec3D closestVec = Vec3D_subtract(closestP, p);
        return Vec3D_getMagnitude(closestVec);
    }
}

bool Line_lineCrossesLineInNormalDirection(Line l1, Line l2)
{
    bool res = false;

    //set a line to mark as line to be crossed
    Vec3D goalLine = Vec3D_subtract(l1.p2, l1.p1);
    Vec3D glN = Vec3D_getNormal(goalLine, VECTOR_UP);

    //treat start and end of 2nd line as points
    Vec3D lastPos = l2.p1;
    Vec3D newPos = l2.p2;

    //transpose into new coords
    // 1. position of points of 2nd line rel to first
    lastPos = Vec3D_subtract(lastPos, l1.p1);
    newPos =  Vec3D_subtract(newPos, l1.p1);
    // 2. rotate into same axis as line 1
    double alpha = Vec3D_getAngle(glN, VECTOR_S);
    lastPos = Vec3D_rotateVectorByAlphaRad(lastPos, alpha);
    newPos = Vec3D_rotateVectorByAlphaRad(newPos, alpha);

    if(
        (lastPos.j >= 0 && newPos.j < 0) &&
        (newPos.i >= 0 && newPos.i <= Vec3D_getMagnitude(goalLine))
    )res = true;

    return res;
}

bool Line_lineCrossesLine(Line l1, Line l2)
{
    bool res = false;

    //set a line to mark as line to be crossed
    Vec3D goalLine = Vec3D_subtract(l1.p2, l1.p1);
    Vec3D glN = Vec3D_getNormal(goalLine, VECTOR_UP);

    //treat start and end of 2nd line as points
    Vec3D lastPos = l2.p1;
    Vec3D newPos = l2.p2;

    //transpose into new coords
    // 1. position of points of 2nd line rel to first
    lastPos = Vec3D_subtract(lastPos, l1.p1);
    newPos =  Vec3D_subtract(newPos, l1.p1);
    // 2. rotate into same axis as line 1
    double alpha = Vec3D_getAngle(glN, VECTOR_S);
    lastPos = Vec3D_rotateVectorByAlphaRad(lastPos, alpha);
    newPos = Vec3D_rotateVectorByAlphaRad(newPos, alpha);

    if(
        ((lastPos.j >= 0 && newPos.j < 0) || (lastPos.j <= 0 && newPos.j > 0)) &&
        (newPos.i >= 0 && newPos.i <= Vec3D_getMagnitude(goalLine))
    )res = true;

    return res;
}

bool Line_pointOnPositiveSideofLine(Line l, Vec3D p)
{
    bool res = false;

    //set a line to mark as line to be crossed
    Vec3D goalLine = Vec3D_subtract(l.p2, l.p1);

    //transpose into new coords
    // 1. position of points of 2nd line rel to first
    p = Vec3D_subtract(p, l.p1);
    // 2. rotate into same axis as line 1

    p = Vec3D_transposeIntoRefCoordSystem(p, goalLine);
    if(p.j > 0)res = true;

    return res;
}
