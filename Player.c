/********************************************************************************************************
*********************************************************************************************************
*********                                   Player Class                                  ***************
*********************************************************************************************************
********************************************************************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include "GameObject.h"
#include "Utility/Geometry.h"
#include "Player.h"

Player Player_create(GameObject* go)
{
    Player p = malloc(sizeof(struct _Player));
    p->go = go;
    p->touches = 0;
    p->state = STATIONARY;
    p->touchingBall = false;
    p->lastScorer = false;
    p->concededPenalty = false;
    p->score = 0;
    return p;
}

void Player_destroy(Player p)
{
    if(p)free(p);
}

void Player_setState(Player p, PlayerState state)
{
    if(p)p->state = state;
}

void Player_setOwnHalf(Player p, Rect r)
{
    p->ownHalf = r;
}

int Player_getTouches(Player p)
{
    return p->touches;
}

bool Player_touchingBall(Player p)
{
    return p->touchingBall;
}

void Player_decrementTouches(Player p)
{
    --p->touches;
}

void Player_resetTouches(Player p)
{
    p->touches = CONS_MAX_TOUCHES;
}

void Player_setLastScorer(Player p, bool b)
{
    p->lastScorer = b;
}

bool Player_isLastScorer(Player p)
{
    return p->lastScorer;
}

void Player_setPenaltyFlag(Player p)
{
    p->concededPenalty = true;
}

void Player_clearPenaltyFlag(Player p)
{
    p->concededPenalty = false;
}

bool Player_concededPenalty(Player p)
{
    return p->concededPenalty;
}

void Player_incrementScore(Player p)
{
    ++p->score;
}

GameObject* Player_getGameObject(Player p)
{
    return p->go;
}

void Player_setCanLeaveHalf(Player p, bool b)
{
    p->canLeaveOwnHalf = b;
}

bool Player_canLeaveOwnHalf(Player p)
{
    return p->canLeaveOwnHalf;
}

void Player_updateState(Player p, GameObject* ball)
{
    double alpha;
    if(Vec3D_isZero(GO_getVel(Player_getGameObject(p))))
    {
        Player_setState(p, STATIONARY);
        alpha = Vec3D_getAngle(Vec3D_subtract(GO_getPos(ball), Player_getPos(p)), VECTOR_N);
    }else
    {
        alpha = Vec3D_getAngle(Player_getVel(p), VECTOR_N);
        Player_setState(p, RUNNING);
    }
    ///////////////////////////////////////////////////
    ///   Don't know why the below line is needed?   //
    ///////////////////////////////////////////////////
    alpha *= -1;
    GO_setRPos(Player_getGameObject(p), alpha);
}