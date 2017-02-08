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
#include "Constants.h"

Player Player_create(GameObject* go)
{
    Player p = malloc(sizeof(struct _Player));
    p->go = go;
    p->touches = 0;
    p->state = STATIONARY;
    p->touchingBall = false;
    return p;
}

void Player_destroy(Player p)
{
    free(p);
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

GameObject* Player_getGameObject(Player p)
{
    return p->go;
}
