#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
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
            {"hasTouches", &hasTouches},
            {"touchingBall", &touchingBall},
            {"canScore", &canScore},
            {"END", NULL}
            };

struct LFuncEntry LFuncList[] = {
            {"runToBall", &runToBall},
            {"stop", &stop},
            {"returnToOwnHalf", &returnToOwnHalf},
            {"shoot", &shoot},
            {"END", NULL}
            };

size_t NodeSize;

void _makeBranchNode(Node n, BranchNodeFunc func, Node yes, Node no);
void _makeLeafNode(Node n, LeafNodeFunc func);
BranchNodeFunc lookupBFunc(char* funcName);
LeafNodeFunc lookupLFunc(char* funcName);

void AI_init()
{
    NodeSize = sizeof(struct decisionTreeNode);
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
    //dummy function returning null input for the sake of testing

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

//returns true if ball in player's half
bool ballInOwnHalf(GameState* gs, int i)
{
    return (Rect_containsPoint(gs->players[i]->ownHalf, gs->ball->pos.i, gs->ball->pos.j));
}

//returns true if player has site of goal
bool canScore(GameState* gs, int i)
{

    int targetGoal = i == 0 ? 1 : 0;
    Vec3D target = GO_getPos(Goal_getLPost(gs->goals[targetGoal]));
    Line l = {GO_getPos(gs->players[i]),target};
    Circle oppBCirc = Player_getGameObject(gs->players[targetGoal])->BCirc;
    if(Circle_inCollisionWithLine(oppBCirc, l))
    {
        // no view of left post
        l.p2 = GO_getPos(Goal_getRPost(gs->goals[targetGoal]));
        if(Circle_inCollisionWithLine(oppBCirc, l))
        {
            // no view of either left or right post
            return false;
        }else
        {
            // view of right post
            Vec3D p = GO_getPos(Player_getGameObject(gs->players[targetGoal]));
            Vec3D pClosestL = Line_getClosestPointFromPointOnLine(l, p);
            Vec3D edge = Vec3D_add(p, Vec3D_scalarMult(Vec3D_normalise(Vec3D_subtract(pClosestL, p)), SIZE_PLAYER_W / 2));
            Vec3D farEdge;
            farEdge.k = 0;
            farEdge.j = edge.j;
            farEdge.i = 1 / Line_getGradient(l) * (farEdge.j - l.p1.j) + l.p1.i;
            if(Vec3D_getMagnitude(Vec3D_subtract(farEdge, edge)) >= SIZE_BALL_W) return true;
        }
    }else
    {
        // view of left post
        Line farPostSite = l;
        farPostSite.p2 = GO_getPos(Goal_getRPost(gs->goals[targetGoal]));
        if(Circle_inCollisionWithLine(oppBCirc, farPostSite))
        {
            Vec3D p = GO_getPos(Player_getGameObject(gs->players[targetGoal]));
            Vec3D pClosestL = Line_getClosestPointFromPointOnLine(farPostSite, p);
            Vec3D edge = Vec3D_add(p, Vec3D_scalarMult(Vec3D_normalise(Vec3D_subtract(pClosestL, p)), SIZE_PLAYER_W / 2));
            Vec3D farEdge;
            farEdge.k = 0;
            farEdge.j = edge.j;
            farEdge.i = 1 / Line_getGradient(l) * (farEdge.j - l.p1.j) + l.p1.i;
            if(Vec3D_getMagnitude(Vec3D_subtract(farEdge, edge)) >= SIZE_BALL_W) return true;
        }else return true;
    }
    return false;
}

bool touchingBall(GameState* gs, int i)
{
    GameObject tempGO = *Player_getGameObject(gs->players[i]);
    GO_setPos(&tempGO, Vec3D_add(GO_getPos(&tempGO), GO_getVel(&tempGO)));
    return GO_isInContact(&tempGO, gs->ball);
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
    i.shot = VECTOR_S;
    return i;
}

/***************************************************************
***********   functions for building decision trees
***************************************************************/

void AI_makeBranchNode(DecisionTree dt, int id, char* func, int yes, int no)
{
    if(!func || !dt) {printf("Null pointer for func name or tree when making Decision tree");exit(-1);}
    Node n = dt + id*NodeSize;
    BranchNodeFunc f = lookupBFunc(func);
    Node yesn = dt + yes*NodeSize;
    Node non = dt + no*NodeSize;
    _makeBranchNode(n, f, yesn, non);
}
void _makeBranchNode(Node n, BranchNodeFunc func, Node yes, Node no)
{
    n->type = BranchNode;
    n->node.b.func = func;
    n->node.b.yes = yes;
    n->node.b.no = no;
}

void AI_makeLeafNode(DecisionTree dt, int id, char* func)
{
    if(!func || !dt) {printf("Null pointer for func name or tree when making Decision tree");exit(-1);}
    Node n = dt + id*NodeSize;
    LeafNodeFunc f = lookupLFunc(func);
    _makeLeafNode(n, f);
}

void _makeLeafNode(Node n, LeafNodeFunc func)
{
    n->type = LeafNode;
    n->node.l.func = func;
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
