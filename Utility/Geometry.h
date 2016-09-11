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

#define Vec3D_subtract(a,b) Vec3D_add((a),Vec3D_scalarMult((b),-1))
#define Vec3D_getNormal(a,b) Vec3D_normalise(Vec3D_crossProduct((a),(b)))
#define Vec3D_isZero(v) Vec3D_isEqual((v),VECTOR_ZERO)

//////////////////////////////////////////////////
////   Vector related code
//////////////////////////////////////////////////

typedef struct Vec3D
{
    int i;
    int j;
    int k;
} Vec3D;

static const Vec3D VECTOR_ZERO = {0,0,0};
static const Vec3D VECTOR_N = {0,-1,0};
static const Vec3D VECTOR_E = {1,0,0};
static const Vec3D VECTOR_S = {0,1,0};
static const Vec3D VECTOR_W = {-1,0,0};
static const Vec3D VECTOR_UP = {0,0,1};
static const Vec3D VECTOR_DOWN = {0,0,-1};

//3d vector addition
Vec3D Vec3D_add(Vec3D v1, Vec3D v2);

//3d vector multiplication
Vec3D Vec3D_scalarMult(Vec3D v, float scalar);
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

//normalise
Vec3D Vec3D_normalise(Vec3D v);     //magnitude of result is 1000

//get magnitude
double Vec3D_getMagnitude(Vec3D v);

//prints string rep of vec to stout
void Vec3D_print(Vec3D v);


///////////////////////////////////////////////////
////           Rect
///////////////////////////////////////////////////

#define Rect_inCollisionCirc(r,c) Circle_inCollisionRect((c),(r))

//Rect struct
typedef struct Rect
{
    int x, y, w, h;
} Rect;

//Circle struct
typedef struct Circle
{
    int x, y, r;
} Circle;

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

//returns whether rect contains point P
bool Rect_containsPoint(Rect r, int x, int y);

//returns whether rect contains circle
bool Rect_containsCircle(Rect r, Circle c);

//interRect collision
bool Rect_inCollision(Rect r1, Rect r2);

//converts to SDL_Rect
SDL_Rect Rect_getSDLRect(Rect r);


///////////////////////////////////////////////////
////           Circle
///////////////////////////////////////////////////

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

//return whether circle contains point
bool Circle_containsPoint(Circle c, int x, int y);

//intercircle collision
bool Circle_inCollision(Circle c1, Circle c2);

//circle-square collision
bool Circle_inCollisionRect(Circle c, Rect r);

#endif // _GEOMETRY_H
