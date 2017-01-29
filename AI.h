#ifndef _AI_H
#define _AI_H

/**
 *
 * The AI core of the two touch game.
 * defines the decision tree
 * Only has one interfaced function which returns controller input for AI character
 *
 */

#include "GameState.h"
#include "UserInput.h"
#include "Constants.h"
#include "Player.h"

//defined functions
#define getVelFromInput(i) Vec3D_scalarMult(UI_getDirVec((i)), CONS_MAX_SPEED)
#define getConFromInput(i) Vec3D_scalarMult(UI_getConVec((i)), CONS_MAX_CONTROL_IMPULSE)
#define getShotFromInput(i) Vec3D_scalarMult(UI_getShotVec((i)), CONS_MAX_APPLIABLE_IMPULSE)

typedef struct decisionTreeNode* DecisionTree;
typedef Input (*LeafNodeFunc)(GameState*, int);
typedef bool (*BranchNodeFunc)(GameState*, int);

struct BFuncEntry
{
    char* name;
    BranchNodeFunc func;
};

struct LFuncEntry
{
    char* name;
    LeafNodeFunc func;
};

extern struct BFuncEntry BFuncList[];
extern struct LFuncEntry LFuncList[];
extern size_t NodeSize;

DecisionTree AI_parseDecisionTree(char* DTfile);
void AI_makeBranchNode(DecisionTree dt, int id, char* func, int yes, int no);
void AI_freeDecisionTree(DecisionTree dt);
void AI_init();
Input AI_getUserInput(GameState* gs, int id, DecisionTree dt);

/*************************************************************
****************   Branch Node Functions
*************************************************************/
//return True or False randomly
bool returnRandBool(GameState* gs, int i);
bool inOwnHalf(GameState* gs, int i);
bool hasTouches(GameState* gs, int i);

/*************************************************************
****************   Branch Node Functions
*************************************************************/
//returns Input moving AI in direction of ball
Input runToBall(GameState* gs, int i);
//returns NULL input (stops player)
Input stop(GameState* gs, int id);

#endif // _AI_H
