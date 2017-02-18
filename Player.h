#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Utility/Geometry.h"
#include "GameObject.h"
#include "Physics.h"

//mappings to gameobject function for convenience
//hard working functions
#define Player_move(p) GO_move((p)->go, GO_getVel((p)->go))
#define Player_isInContact(p1,p2) GO_isInContact((p1)->go, (p2)->go)
#define Player_isInContactWithBoundary(p,b) Phys_inCollisionWithBoundary((p)->go, (b))
#define Player_adjustForBoundary(p,b) Phys_boundaryAdjust((p)->go, (b))
#define Player_isInBounds(p,r) GO_isInBounds((p)->go, (r))

//setters
#define Player_setAcc(p,v) GO_setAcc((p)->go, (v))
#define Player_setVel(p,v) GO_setVel((p)->go, (v))
#define Player_setPos(p,v) GO_setPos((p)->go, (v))
#define Player_setRAcc(p,i) GO_setRAcc((p)->go, (i))
#define Player_setRVel(p,i) GO_setRVel((p)->go, (i))
#define Player_setRPos(p,i) GO_setRPos((p)->go, (i))
#define Player_setIsStationary(p,b) GO_setStationary((p)->go, (b))
#define Player_setBCirc(p,c) GO_setBCirc((p)->go, (c))
#define Player_setMass(p,m) GO_setMass((p)->go, (m))

//getters
#define Player_getAcc(p) GO_getAcc((p)->go)
#define Player_getVel(p) GO_getVel((p)->go)
#define Player_getPos(p) GO_getPos((p)->go)
#define Player_getRAcc(p) GO_getRAcc((p)->go)
#define Player_getRVel(p) GO_getRVel((p)->go)
#define Player_getRPos(p) GO_getRPos((p)->go)
#define Player_getMass(p) GO_getMass((p)->go)

//function defs for readablity of higher level code
#define Player_isInOwnHalf(p) Rect_containsPoint((p)->ownHalf, (p)->go->pos.i, (p)->go->pos.j)
#define Player_hasTouches(p) (Player_getTouches((p)) > 0)

typedef struct _Player* Player;

typedef enum PlayerState
{
    STATIONARY = 0,
    RUNNING = 1,
    KICKING = 2,
}PlayerState;

struct _Player
{
    GameObject* go;
    int touches;
    int score;
    Rect ownHalf;
    bool touchingBall, lastScorer, concededPenalty, canLeaveOwnHalf;
    PlayerState state;
};

Player Player_create(GameObject* go);
void Player_destroy(Player p);
void Player_setOwnHalf(Player p, Rect r);
int Player_getTouches(Player p);
void Player_decrementTouches(Player p);
void Player_resetTouches(Player p);
void Player_incrementScore(Player p);
bool Player_touchingBall(Player p);
GameObject* Player_getGameObject(Player p);
void Player_setLastScorer(Player p, bool b);
bool Player_isLastScorer(Player p);
void Player_setPenaltyFlag(Player p);
void Player_clearPenaltyFlag(Player p);
bool Player_concededPenalty(Player p);
void Player_setCanLeaveHalf(Player p, bool b);
bool Player_canLeaveOwnHalf(Player p);

#endif // _PLAYER_H_
