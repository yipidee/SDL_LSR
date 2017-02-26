#ifndef _GEOMETRY_H
#define _GEOMETRY_H

/*****************************************************************************
*** Elementry geometry library
******************************************************************************
**/

/////////////////////////////////////
////  3D Vectors
/////////////////////////////////////
#include <stdbool.h>
#include <SDL2/SDL.h>

#define Vec3D_subtract(a,b) Vec3D_add((a),Vec3D_scalarMult((b),-1.0))
#define Vec3D_getNormal(a,b) Vec3D_normalise(Vec3D_crossProduct((a),(b)))
#define Vec3D_isZero(v) Vec3D_equal((v),VECTOR_ZERO)
#define Vec3D_negate(v) Vec3D_scalarMult((v), -1)

//////////////////////////////////////////////////
////   Vector related code
//////////////////////////////////////////////////

typedef struct Vec3D
{
    double i;
    double j;
    double k;
} Vec3D;

static const Vec3D VECTOR_ZERO = {0,0,0};
static const Vec3D VECTOR_N = {0,-1,0};
static const Vec3D VECTOR_E = {1,0,0};
static const Vec3D VECTOR_S = {0,1,0};
static const Vec3D VECTOR_W = {-1,0,0};
static const Vec3D VECTOR_UP = {0,0,1};
static const Vec3D VECTOR_DOWN = {0,0,-1};

//make vector from input
Vec3D Vec3D_makeVector(double i, double j, double k);

//3d vector addition
Vec3D Vec3D_add(Vec3D v1, Vec3D v2);

//3d vector multiplication
Vec3D Vec3D_scalarMult(Vec3D v, float scalar);
double Vec3D_dotProduct(Vec3D v1, Vec3D v2);
Vec3D Vec3D_crossProduct(Vec3D v1, Vec3D v2);

//get cosine of angle between two vetors
double Vec3D_getCosAlpha(Vec3D v1, Vec3D v2);

//get sine of angle between two vetors
double Vec3D_getSinAlpha(Vec3D v1, Vec3D v2);

//get angle between vectors
double Vec3D_getAngle(Vec3D v1, Vec3D v2);

//check for parallel
bool Vec3D_isParallel(Vec3D v1, Vec3D v2);

//check for perpendicular
bool Vec3D_isPerpendicular(Vec3D v1, Vec3D v2);

//equals
bool Vec3D_equal(Vec3D v1, Vec3D v2);

//normalise
Vec3D Vec3D_normalise(Vec3D v);

//get magnitude
double Vec3D_getMagnitude(Vec3D v);

//prints string rep of vec to stout
void Vec3D_print(Vec3D v);

//get unit vector normal to speified vector (2D only)
Vec3D Vec3D_getUnitNormal(Vec3D v);

//rotate a vector anticlockwise through alpha radians
Vec3D Vec3D_rotateVectorByAlphaRad(Vec3D v, double alpha);

//rotate a vector anticlockwise through alpha radians, pass cos alpha as arg
Vec3D Vec3D_rotateVectorByCosAlpha(Vec3D v, double cosAlpha);


///////////////////////////////////////////////////
////           Rect
///////////////////////////////////////////////////

#define Rect_inCollisionCirc(r,c) Circle_inCollisionRect((c),(r))

//Rect struct
typedef struct Rect
{
    double x, y, w, h;
} Rect;

//Circle struct
typedef struct Circle
{
    double x, y, r;
} Circle;

static const Rect RECT_NULL = {0,0,0,0};

//greate circle with central coordinates and radius
Rect Rect_create(double x, double y, double w, double h);

//set/get properties
void Rect_setX(Rect* r, double x);
void Rect_setY(Rect* r, double y);
void Rect_setW(Rect* r, double w);
void Rect_setH(Rect* r, double h);
double Rect_getX(const Rect* r);
double Rect_getY(const Rect* r);
double Rect_getW(const Rect* r);
double Rect_getH(const Rect* r);

//move Rect
void Rect_translate(Rect* r, Vec3D delta);

//returns whether rect contains point P
bool Rect_containsPoint(Rect r, double x, double y);

//returns whether rect contains circle
bool Rect_containsCircle(Rect r, Circle c);

//interRect collision
bool Rect_inCollision(Rect r1, Rect r2);

//converts to SDL_Rect
SDL_Rect Rect_getSDLRect(Rect r);

//compares two rectangles for equivalency
bool Rect_equals(Rect r1, Rect r2);


///////////////////////////////////////////////////
////           Circle
///////////////////////////////////////////////////

//final cirlces
static const Circle CIRCLE_NULL = {0,0,0};

//greate circle with central coordinates and radius
Circle Circle_create(double x, double y, double r);

//set/get properties
void Circle_setX(Circle* c, double x);
void Circle_setY(Circle* c, double y);
void Circle_setR(Circle* c, double r);
int Circle_getX(const Circle* c);
int Circle_getY(const Circle* c);
int Circle_getR(const Circle* c);

//move circle
void Circle_translate(Circle* c, Vec3D delta);

//return whether circle contains point
bool Circle_containsPoint(Circle c, double x, double y);

//intercircle collision
bool Circle_inCollision(Circle c1, Circle c2);

//circle-square collision
bool Circle_inCollisionRect(Circle c, Rect r);

///////////////////////////////////////////////////
////           Line
///////////////////////////////////////////////////

#define LINE_IS_HORIZ_OR_VERT 0

typedef struct Line
{
    Vec3D p1;
    Vec3D p2;
}Line;

bool Circle_inCollisionWithLine(Circle c, Line l, double offset);

double Line_getLength(Line l);

double Line_getGradient(Line l);

bool Line_isVertical(Line l);

bool Line_isHorizontal(Line l);

Vec3D Line_getClosestPointFromPointOnLine(Line l, Vec3D p);

double Line_getDistanceOfPointFromLine(Line l, Vec3D p);


#endif // _GEOMETRY_H
