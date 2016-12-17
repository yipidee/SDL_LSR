
#include "Utility/Geometry.h"

#define isInOwnHalf(p) Rect_containsPoint((p)->ownHalf, (p)->go.pos.i, (p)->go.pos.j)

typedef struct _Player* Player;

typedef enum PlayerState
{
    STATIONARY = 0,
    RUNNING = 1,
    KICKING = 2,
}PlayerState;

struct _Player
{
    GameObject go;
    int touches;
    Rect ownHalf;
    bool touchingBall;
    PlayerState state;
};
