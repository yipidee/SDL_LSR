#include <stdbool.h>
#include <math.h>
#include "Utility/Geometry.h"
#include "GameObject.h"
#include "Constants.h"
#include "Physics.h"

//static method prototype
static void makeWalls(Line* L, Line* T, Line* R, Line* B, Rect r);

bool Phys_inCollision(GameObject* go1, GameObject* go2)
{
    return Circle_inCollision(go1->BCirc, go2->BCirc);
}

bool Phys_inCollisionWithBoundary(GameObject* go, Rect r)
{
    if(GO_isInBounds(go, r)) return false;

    Line T, R, B, L;
    r.x += go->BCirc.r;
    r.y += go->BCirc.r;
    r.w -= 2 * go->BCirc.r;
    r.x -= 2 * go->BCirc.r;
    makeWalls(&T, &R, &B, &L, r);
 
    Vec3D x = GO_getPos(go);

    bool res = false;

    if(
        Line_pointOnPositiveSideofLine(T, x) ||
        Line_pointOnPositiveSideofLine(R, x) ||
        Line_pointOnPositiveSideofLine(B, x) ||
        Line_pointOnPositiveSideofLine(L, x)
    )res = true;
    return res;
}

void Phys_boundaryCollision(GameObject* go, Rect r)
{
    if(!GO_isInBounds(go, r))
    {
        Line T, R, B, L;
        r.x += go->BCirc.r;
        r.y += go->BCirc.r;
        r.w -= 2 * go->BCirc.r;
        r.h -= 2 * go->BCirc.r;
        makeWalls(&T, &R, &B, &L, r);

        //position fix
        Vec3D P = GO_getPos(go);
        Vec3D delta = VECTOR_ZERO;
        // check for collision with top wall
        if(!Line_pointOnPositiveSideofLine(T, P))
        {
            //adjust position
            delta.j = T.p1.j - P.j;
            //adjust movement
            go->vel.i *= CONS_BALL_WALL_COR;
            go->vel.j *= -CONS_BALL_WALL_COR;
            go->acc.j *= -1;
        }
        if(!Line_pointOnPositiveSideofLine(R, P))
        {
            //adjust position
            delta.i = R.p1.i - P.i;
            //adjust movement
            go->vel.i *= -CONS_BALL_WALL_COR;
            go->vel.j *= CONS_BALL_WALL_COR;
            go->acc.i *= -1;
        }
        if(!Line_pointOnPositiveSideofLine(B, P))
        {
            //adjust position
            delta.j = B.p1.j - P.j;
            //adjust movement
            go->vel.i *= CONS_BALL_WALL_COR;
            go->vel.j *= -CONS_BALL_WALL_COR;
            go->acc.j *= -1;
        }
        if(!Line_pointOnPositiveSideofLine(L, P))
        {
            //adjust position
            delta.i = L.p1.i - P.i;
            //adjust movement
            go->vel.i *= -CONS_BALL_WALL_COR;
            go->vel.j *= CONS_BALL_WALL_COR;
            go->acc.i *= -1;
        }
        GO_move(go, delta);
    }
}

void Phys_boundaryAdjust(GameObject* go, Rect r)
{
    Line T, R, B, L;
    makeWalls(&T, &R, &B, &L, r);

    //determine struck wall and adjust accordingly
    if(Circle_inCollisionWithLine(go->BCirc, T, 0)||Circle_inCollisionWithLine(go->BCirc, B, 0))
    {
        if(go->pos.j < T.p1.j + go->BCirc.r)
        {
            Vec3D delta = VECTOR_ZERO;
            delta.j = T.p1.j + go->BCirc.r - go->pos.j;
            GO_move(go, delta);
        }
        if(go->pos.j > (B.p1.j - go->BCirc.r))
        {
            Vec3D delta = VECTOR_ZERO;
            delta.j = B.p1.j - go->BCirc.r - go->pos.j;
            GO_move(go, delta);
        }
        go->vel.j = 0;
        go->acc.j = 0;
    }if(Circle_inCollisionWithLine(go->BCirc, R, 0)||Circle_inCollisionWithLine(go->BCirc, L, 0))
    {
        if(go->pos.i < L.p1.i + go->BCirc.r)
        {
            Vec3D delta = VECTOR_ZERO;
            delta.i = L.p1.i + go->BCirc.r - go->pos.i;
            GO_move(go, delta);
        }
        if(go->pos.i > R.p1.i - go->BCirc.r)
        {
            Vec3D delta = VECTOR_ZERO;
            delta.i = R.p1.i - go->BCirc.r - go->pos.i;
            GO_move(go, delta);
        }
        go->vel.i = 0;
        go->acc.i = 0;
    }
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
    GO_setVel(go2, v2);
}

void Phys_conservationMomentumCollision2D(GameObject* go1, GameObject* go2, float COR)
{
    Vec3D dirOfCollision = Vec3D_subtract(go2->pos, go1->pos);
    dirOfCollision = Vec3D_normalise(dirOfCollision);
    Vec3D perpToCollision = Vec3D_getUnitNormal(dirOfCollision);

    if(go1->isStationary && !Vec3D_isZero(GO_getVel(go2)))
    {
        double alpha;
        alpha = Vec3D_getCosAlpha(go2->vel, dirOfCollision);
        Vec3D uDOC = Vec3D_scalarMult(dirOfCollision, Vec3D_getMagnitude(go2->vel) * alpha);
        alpha = Vec3D_getCosAlpha(go2->vel, perpToCollision);
        Vec3D uPTC = Vec3D_scalarMult(perpToCollision, Vec3D_getMagnitude(go2->vel) * alpha);

        uDOC = Vec3D_scalarMult(uDOC, -1.0);
        GO_setVel(go2, Vec3D_add(uDOC, uPTC));
        GO_setAcc(go2, Vec3D_scalarMult(Vec3D_normalise(GO_getVel(go2)),CONS_BALL_COURT_DEACC));
    }else if(go2->isStationary && !Vec3D_isZero(GO_getVel(go1)))
    {
        double alpha;
        alpha = Vec3D_getCosAlpha(go1->vel, dirOfCollision);
        Vec3D uDOC = Vec3D_scalarMult(dirOfCollision, Vec3D_getMagnitude(go1->vel) * alpha);
        alpha = Vec3D_getCosAlpha(go1->vel, perpToCollision);
        Vec3D uPTC = Vec3D_scalarMult(perpToCollision, Vec3D_getMagnitude(go1->vel) * alpha);

        uDOC = Vec3D_scalarMult(uDOC, -1.0);
        GO_setVel(go1, Vec3D_add(uDOC, uPTC));
        GO_setAcc(go1, Vec3D_scalarMult(Vec3D_normalise(GO_getVel(go1)),CONS_BALL_COURT_DEACC));
    }else if((!Vec3D_isZero(go1->vel))&&(!Vec3D_isZero(go2->vel)))
    {
        //get component of vel in direction of collision
        double  alpha1;
        alpha1 = Vec3D_getCosAlpha(go1->vel, dirOfCollision);
        double  alpha2;
        alpha2 = Vec3D_getCosAlpha(go2->vel, Vec3D_scalarMult(dirOfCollision, -1));

        //velocities precollision in cirection of collision
        Vec3D u1DOC = Vec3D_scalarMult(dirOfCollision, Vec3D_getMagnitude(go1->vel) * alpha1);
        Vec3D u2DOC = Vec3D_scalarMult(dirOfCollision, Vec3D_getMagnitude(go2->vel) * alpha2);

        alpha1 = Vec3D_getCosAlpha(go1->vel, perpToCollision);
        alpha2 = Vec3D_getCosAlpha(go2->vel, Vec3D_scalarMult(perpToCollision, -1));

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
        // determine stationary and moving objects
        GameObject* stationary = NULL;
        GameObject* moving = NULL;
        stationary = Vec3D_isZero(GO_getVel(go1)) ? go1 : go2;
        moving = stationary == go1 ? go2 : go1;
        dirOfCollision = Vec3D_subtract(GO_getPos(stationary), GO_getPos(moving));
        dirOfCollision = Vec3D_normalise(dirOfCollision);
        perpToCollision = Vec3D_getUnitNormal(dirOfCollision);

        //get component of vel in direction of collision
        double  alpha;
        alpha = Vec3D_getCosAlpha(GO_getVel(moving), dirOfCollision);

        //velocities precollision in cirection of collision
        Vec3D uDOC = Vec3D_scalarMult(dirOfCollision, Vec3D_getMagnitude(GO_getVel(moving)) * alpha);

        alpha = Vec3D_getCosAlpha(GO_getVel(moving), perpToCollision);

        //velocities precollision perp to cirection of collision
        Vec3D uPTC = Vec3D_scalarMult(perpToCollision, Vec3D_getMagnitude(GO_getVel(moving)) * alpha);

        GO_setVel(moving, uDOC);

        //collision in dir of collision
        Phys_conservationMomentumCollision1D(moving, stationary, COR);

        //resolve into global coords again
        GO_setVel(moving, Vec3D_add(GO_getVel(moving), uPTC));
        GO_setAcc(stationary, Vec3D_scalarMult(Vec3D_normalise(GO_getVel(stationary)),CONS_BALL_COURT_DEACC));
    }
}

void Phys_appliedImpulse2D(GameObject* go, Vec3D impulse)
{
    if(GO_getMass(go)==0)GO_setMass(go, 1);
    float mult = 1.0/GO_getMass(go);
    impulse = Vec3D_scalarMult(impulse, mult);
    GO_setVel(go, Vec3D_add(go->vel, impulse));
    GO_setAcc(go, Vec3D_scalarMult(Vec3D_normalise(GO_getVel(go)),CONS_BALL_COURT_DEACC));
}

void Phys_adjustForCollisionWithStatObject(GameObject* go1, GameObject* go2)
{
        Vec3D d = Vec3D_subtract(go1->pos, go2->pos);
        d = Vec3D_normalise(d);
        GO_setPos(go1, Vec3D_add(go2->pos, Vec3D_scalarMult(d, go1->BCirc.r + go2->BCirc.r)));
}

// local static functions
static void makeWalls(Line* T, Line* R, Line* B, Line* L, Rect r)
{
    Vec3D tl, tr, br, bl;

    tl.i = r.x;
    tl.j = r.y;
    tl.k = 0;
    tr.i = r.x + r.w;
    tr.j = r.y;
    tr.k = 0;
    br.i = r.x + r.w;
    br.j = r.y + r.h;
    br.k = 0;
    bl.i = r.x;
    bl.j = r.y + r.h;
    bl.k = 0;

    T->p1 = tl;
    T->p2 = tr;
    R->p1 = tr;
    R->p2 = br;
    B->p1 = br;
    B->p2 = bl;
    L->p1 = bl;
    L->p2 = tl;
}
