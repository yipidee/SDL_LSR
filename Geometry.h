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

#define Vec3D_subtract((a),(b)) Vec3D_add((a),Vec2D_scalarMult((b),-1))
#define Vec3D_getNormal((a),(b)) Vec3D_normalise(Vec3D_crossProduct((a),(b)))

//////////////////////////////////////////////////
////   Vector related code
//////////////////////////////////////////////////

typedef struct Vec3D
{
    int i;
    int j;
    int k;
} Vec3D;

static const Vec3D ZERO_VECTOR = {0,0,0};

//3d vector addition
Vec3D Vec3D_add(Vec3D v1, Vec3D v2);

//3d vector multiplication
Vec3D Vec3D_scalarMult(Vec3D v, int scalar);
int Vec3D_dotProduct(Vec3D v1, Vec3D v2);
Vec3D Vec3D_crossProduct(Vec3D v1, Vec3D v2);

//get cosine of angle between two vetors
double Vec3D_getCosAlpha(Vec3D v1, Vec3D v2);

//get angle between vectors
double Vec3D_getAngle(Vec3D v1, Vec3D v2);

//check for parallel
bool Vec3D_isParallel(Vec3D v1, Vec3D v2);

//check for perpendicular
bool Vec3D_isPerpendicular(Vec3D v1, Vec3D v2);

//equals
bool Vec3D_equal(Vec3D v1, Vec3D v2);

//is zero
bool Vec3D_isZero(Vec3D v);

//normalise
Vec3D Vec3D_normalise(Vec3D v);

//get magnitude
double Vec3D_getMagnitude(Vec3D v);


///////////////////////////////////////////////////
////           Rect
///////////////////////////////////////////////////

#define Rect_inCollisionCirc((r),(c)) Circle_inCollisionRect((c),(r))

typedef struct Rect
{
    int x, y, w, h;
} Rect

//greate circle with central coordinates and radius
Rect Rect_create(int x, int y, int w, int h);

//set/get properties
void Rect_setX(Rect* r, int x);
void Rect_setY(Rect* r, int y);
void Rect_setW(Rect* r, int w);
void Rect_setH(Rect* r, int h);
int Rect_getX(const Rect* r);
int Rect_getY(const Rect* r);
int Rect_getW(const Rect* r);
int Rect_getH(const Rect* r);

//move Rect
void Rect_translate(Rect* r, Vec3D delta);

//interRect collision
bool Rect_inCollision(Rect r1, Rect r2);


///////////////////////////////////////////////////
////           Circle
///////////////////////////////////////////////////

typedef struct Circle
{
    int x, y, r;
} Circle

//greate circle with central coordinates and radius
Circle Circle_create(int x, int y, int r);

//set/get properties
void Circle_setX(Circle* c, int x);
void Circle_setY(Circle* c, int y);
void Circle_setR(Circle* c, int r);
int Circle_getX(const Circle* c);
int Circle_getY(const Circle* c);
int Circle_getR(const Circle* c);

//move circle
void Circle_translate(Circle* c, Vec3D delta);

//intercircle collision
bool Circle_inCollision(Circle c1, Circle c2);

//circle-square collision
bool Circle_inCollisionRect(Circle c, SDL_Rect r);

#endif // _GEOMETRY_H
