#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "AI.h"

typedef struct decisionTreeNode* Node;

typedef enum NodeType
{
    BranchNode,
    LeafNode
}NodeType;

typedef struct bNode
{
    BranchNodeFunc func;
    Node yes, no;
}bNode;

typedef struct lNode
{
    LeafNodeFunc func;
}lNode;

union Nodes
{
    bNode b;
    lNode l;
};

struct decisionTreeNode
{
    int id;
    NodeType type;
    union Nodes node;
};

struct BFuncEntry BFuncList[] = {
            {"randomTrueFalse", &returnRandBool},
            {"inOwnHalf", &inOwnHalf},
            {"ballInOwnHalf", &ballInOwnHalf},
            {"ballIsStationary", &ballIsStationary},
            {"hasTouches", &hasTouches},
            {"oppHasAllTouches", &oppHasAllTouches},
            {"oppHasTouches", &oppHasTouches},
            {"touchingBall", &touchingBall},
            {"canScore", &canScore},
            {"isPenalty", &isPenalty},
            {"concededPenalty", &concededPenalty},
            {"END", NULL}
            };

struct LFuncEntry LFuncList[] = {
            {"runToBall", &runToBall},
            {"stop", &stop},
            {"returnToOwnHalf", &returnToOwnHalf},
            {"shoot", &shoot},
            {"playIntoCorner", &playIntoCorner},
            {"returnToOwnHalf", &returnToOwnHalf},
            {"END", NULL}
            };

size_t NodeSize;
const Vec3D OUTSIDE_PLAYABLE_RANGE = {-1,-1,-1};
Vec3D gShotTarget;  // storage location of scorable target for "canScore" function

BranchNodeFunc lookupBFunc(char* funcName);
LeafNodeFunc lookupLFunc(char* funcName);

void AI_init()
{
    NodeSize = sizeof(struct decisionTreeNode);
    gShotTarget = OUTSIDE_PLAYABLE_RANGE;
    srand((int)time(NULL));
}

void AI_freeDecisionTree(DecisionTree dt)
{
    free(dt);
    dt = NULL;
}

// function that traverses decision tree and return AI input
Input AI_getUserInput(GameState* gs, int id, Node start)
{
    if(start == NULL) return INPUT_NULL;
    if(start->type == BranchNode)
    {

        Node next = start->node.b.func(gs, id) ? start->node.b.yes : start->node.b.no;
        if(next == NULL) return INPUT_NULL;
        return (AI_getUserInput(gs, id, next));
    }else
    {
        return start->node.l.func(gs, id);
    }
}

/*************************************************************
****************   Branch Node Functions
*************************************************************/
//return True or False randomly
bool returnRandBool(GameState* gs, int i)
{
    double r = rand();
    r /= RAND_MAX;
    bool res = (r > 0.5);
    return res;
}

//returns true if player in own half of court
bool inOwnHalf(GameState* gs, int i)
{
    return Player_isInOwnHalf(gs->players[i]);
}

//returns true if player has more than 0 touches remaining
bool hasTouches(GameState* gs, int i)
{
    return (Player_getTouches(gs->players[i]) > 0);
}

bool oppHasAllTouches(GameState* gs, int i)
{
    int id = i == 0 ? 1 : 0;
    return (gs->players[id]->touches == CONS_MAX_TOUCHES);
}

bool oppHasTouches(GameState* gs, int i)
{
    int id = i == 0 ? 1 : 0;
    return (gs->players[id]->touches > 0);
}

//returns true if ball in player's half
bool ballInOwnHalf(GameState* gs, int i)
{
    return (Rect_containsPoint(gs->players[i]->ownHalf, gs->ball->pos.i, gs->ball->pos.j));
}

bool ballIsStationary(GameState* gs, int i)
{
    return Vec3D_isZero(GO_getVel(gs->ball));
}


//returns true if player has site of goal
bool canScore(GameState* gs, int i)
{
    int targetGoal = i == 0 ? 1 : 0;
    Vec3D target = GO_getPos(Goal_getLPost(gs->goals[targetGoal]));
    //line of site between ball and target
    Line l = {GO_getPos(gs->ball),target};
    //bounding circle of oppositions player
    Circle oppBCirc = Player_getGameObject(gs->players[targetGoal])->BCirc;
    //check if line of site passes through bounding circle
    if(Circle_inCollisionWithLine(oppBCirc, l, gs->ball->BCirc.r * 2))
    {
        // no view of left post
        //change line of site to right post
        l.p2 = GO_getPos(Goal_getRPost(gs->goals[targetGoal]));
        if(Circle_inCollisionWithLine(oppBCirc, l, gs->ball->BCirc.r * 2))
        {
            // no view of either left or right post
            // goal is covered, can't score
            return false;
        }else
        {
            // view of right post
            Vec3D p = Player_getPos(gs->players[targetGoal]);
            Vec3D pClosestL = Line_getClosestPointFromPointOnLine(l, p);
            Vec3D edge = Vec3D_add(p, Vec3D_scalarMult(Vec3D_normalise(Vec3D_subtract(pClosestL, p)), oppBCirc.r));
            Vec3D farEdge;
            farEdge.k = 0;
            farEdge.j = edge.j;
            farEdge.i = 1 / Line_getGradient(l) * (farEdge.j - l.p1.j) + l.p1.i;
            if(Vec3D_getMagnitude(Vec3D_subtract(farEdge, edge)) >= (gs->ball->BCirc.r * 2)) return true;
        }
    }else
    {
        // view of left post
        Line farPostSite = l;
        farPostSite.p2 = GO_getPos(Goal_getRPost(gs->goals[targetGoal]));
        if(Circle_inCollisionWithLine(oppBCirc, farPostSite, gs->ball->BCirc.r * 2))
        {
            Vec3D p = Player_getPos(gs->players[targetGoal]);
            Vec3D pClosestL = Line_getClosestPointFromPointOnLine(farPostSite, p);
            Vec3D edge = Vec3D_add(p, Vec3D_scalarMult(Vec3D_normalise(Vec3D_subtract(pClosestL, p)), SIZE_PLAYER_W / 2));
            Vec3D farEdge;
            farEdge.k = 0;
            farEdge.j = edge.j;
            farEdge.i = 1 / Line_getGradient(l) * (farEdge.j - l.p1.j) + l.p1.i;
            if(Vec3D_getMagnitude(Vec3D_subtract(farEdge, edge)) >= (gs->ball->BCirc.r * 2)) return true;
        }else
        {
            gShotTarget = Vec3D_scalarMult(Vec3D_add(GO_getPos(Goal_getLPost(gs->goals[targetGoal])),
                                                        GO_getPos(Goal_getRPost(gs->goals[targetGoal]))
                                                    ),0.5);
            return true;
        }
    }
    gShotTarget = OUTSIDE_PLAYABLE_RANGE;
    return false;
}

bool touchingBall(GameState* gs, int i)
{
    GameObject tempGO = *Player_getGameObject(gs->players[i]);
    GO_setPos(&tempGO, Vec3D_add(GO_getPos(&tempGO), GO_getVel(&tempGO)));
    return GO_isInContact(&tempGO, gs->ball);
}

bool isPenalty(GameState* gs, int i)
{
    return (gs->currPlayState == PENALTY);
}

bool concededPenalty(GameState* gs, int i)
{
    return Player_concededPenalty(gs->players[i]);
}

/*************************************************************
****************   Leaf Node Functions
*************************************************************/
//returns Input moving AI in direction of ball
Input runToBall(GameState* gs, int i)
{
    Input in = INPUT_NULL;
    Vec3D toBall = Vec3D_subtract(GO_getPos(gs->ball), Player_getPos(gs->players[1]));
    int mag = Vec3D_getMagnitude(toBall);
    toBall = Vec3D_normalise(toBall);
    if(mag<CONS_MAX_SPEED)
    {
        toBall = Vec3D_scalarMult(toBall, mag / CONS_MAX_SPEED);
    }
    in.direction = toBall;
    return in;
}

//returns NULL input (stops player)
Input stop(GameState* gs, int id)
{
    return INPUT_NULL;
}

//returns Input returning player to own half
Input returnToOwnHalf(GameState* gs, int id)
{
    Input i = INPUT_NULL;
    i.direction = id == 0 ? VECTOR_S : VECTOR_N;
    return i;
}

Input shoot(GameState* gs, int id)
{
    Input i = INPUT_NULL;
    Vec3D target = Vec3D_equal(gShotTarget, OUTSIDE_PLAYABLE_RANGE) ? GO_getPos(Goal_getLPost(gs->goals[0])) : gShotTarget;
    Vec3D dir = Vec3D_subtract(target, GO_getPos(gs->ball));
    i.shot = Vec3D_normalise(dir);
    return i;
}

Input playIntoCorner(GameState* gs, int id)
{
    Input i = INPUT_NULL;
    Vec3D dir = Vec3D_subtract(Vec3D_makeVector(WORLD_WIDTH, WORLD_HEIGHT, 0), GO_getPos(gs->ball));
    i.control = Vec3D_normalise(dir);
    return i;
}

/***************************************************************
***********   functions for building decision trees
***************************************************************/

void AI_makeBranchNode(DecisionTree dt, int id, char* func, int yes, int no)
{
    if(!func || !dt) {printf("Null pointer for func name or tree when making Decision tree");exit(-1);}
    dt[id].id = id;
    dt[id].type = BranchNode;
    dt[id].node.b.func = lookupBFunc(func);
    dt[id].node.b.yes = &dt[yes];
    dt[id].node.b.no = &dt[no];
}

void AI_makeLeafNode(DecisionTree dt, int id, char* func)
{
    if(!func || !dt) {printf("Null pointer for func name or tree when making Decision tree");exit(-1);}
    dt[id].id = id;
    dt[id].type = LeafNode;
    dt[id].node.l.func = lookupLFunc(func);
}

BranchNodeFunc lookupBFunc(char* funcName)
{
    if(funcName!=NULL)
    {
        int i = 0;
        struct BFuncEntry b = BFuncList[i];
        while(b.func)
        {
            if(!strcmp(funcName, b.name)) return b.func;
            ++i;
            b = BFuncList[i];
        }
    }
    return NULL;
}

LeafNodeFunc lookupLFunc(char* funcName)
{
    if(funcName!=NULL)
    {
        int i = 0;
        struct LFuncEntry l = LFuncList[i];
        while(l.func)
        {
            if(!strcmp(funcName, l.name)) return l.func;
            ++i;
            l = LFuncList[i];
        }
    }
    return NULL;
}
