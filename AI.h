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

//prototypes for decision tree building
DecisionTree AI_parseDecisionTree(char* DTfile);
void AI_makeBranchNode(const DecisionTree dt, int id, char* func, int yes, int no);
void AI_makeLeafNode(DecisionTree dt, int id, char* func);

//init AI unit
void AI_init();

//free memory associated with decision tree
void AI_freeDecisionTree(DecisionTree dt);

// function that return AI user input
Input AI_getUserInput(GameState* gs, int id, DecisionTree dt);

/*************************************************************
****************   Branch Node Functions
*************************************************************/
bool returnRandBool(GameState* gs, int i);  //return True or False randomly
bool inOwnHalf(GameState* gs, int i);  //return whether player is in own half
bool hasTouches(GameState* gs, int i);  //return whether player has touches remaining
bool oppHasAllTouches(GameState* gs, int i);  //returns true if opposition has all touches remaining
bool ballInOwnHalf(GameState* gs, int i); //returns true if ball in player's half
bool ballIsStationary(GameState* gs, int i); //returns whether ball stopped
bool canScore(GameState* gs, int i);  //open view of goal
bool touchingBall(GameState* gs, int i);  //touching the ball
bool isPenalty(GameState* gs, int i);  //returns whether game in penalty state
bool concededPenalty(GameState* gs, int i);  //returns whether player conceded penalty

/*************************************************************
****************   Leaf Node Functions
*************************************************************/
Input runToBall(GameState* gs, int i);  //returns Input moving AI in direction of ball
Input stop(GameState* gs, int id);  //returns NULL input (stops player)
Input returnToOwnHalf(GameState* gs, int id);  //returns Input returning player to own half
Input shoot(GameState* gs, int id); //returns input to shoot at goal
Input playIntoCorner(GameState* gs, int id); //returns input to play ball toward bottom right corner

#endif // _AI_H
