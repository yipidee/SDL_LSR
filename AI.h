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
DecisionTree AI_loadDecisionTree(char* dtFile);
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
bool returnRand1in3(GameState* gs, int i);  //return True or False randomly
bool returnRand1in5(GameState* gs, int i);  //return True or False randomly
bool returnRand1in10(GameState* gs, int i);  //return True or False randomly
bool returnRand1in20(GameState* gs, int i);  //return True or False randomly
bool returnRand1in50(GameState* gs, int i);  //return True or False randomly
bool returnRand1in100(GameState* gs, int i);  //return True or False randomly
bool inOwnHalf(GameState* gs, int i);  //return whether player is in own half
bool hasTouches(GameState* gs, int i);  //return whether player has touches remaining
bool oppHasAllTouches(GameState* gs, int i);  //returns true if opposition has all touches remaining
bool oppHasTouches(GameState* gs, int i);  // returns true  if opposition has any touches
bool ballInOwnHalf(GameState* gs, int i); //returns true if ball in player's half
bool ballIsStationary(GameState* gs, int i); //returns whether ball stopped
bool canScore(GameState* gs, int i);  //open view of goal
bool touchingBall(GameState* gs, int i);  //touching the ball
bool isPenalty(GameState* gs, int i);  //returns whether game in penalty state
bool concededPenalty(GameState* gs, int i);  //returns whether player conceded penalty
bool inDefensivePosition(GameState* gs, int i); //returns whether player in blocking position
bool scoredLast(GameState* gs, int i); //returns whether player scored last

/*************************************************************
****************   Leaf Node Functions
*************************************************************/
Input runToBall(GameState* gs, int i);  //returns Input moving AI in direction of ball
Input stop(GameState* gs, int id);  //returns NULL input (stops player)
Input returnToOwnHalf(GameState* gs, int id);  //returns Input returning player to own half
Input shoot(GameState* gs, int id); //returns input to shoot at goal
Input playIntoLCorner(GameState* gs, int id); //returns input to play ball toward bottom right corner
Input playIntoRCorner(GameState* gs, int id); //returns input to play ball toward bottom right corner
Input returnToOwnHalf(GameState* gs, int id); //player runs back to own half
Input takeDefensivePosition(GameState* gs, int id); //player runs back to own half
Input runToOpposition(GameState* gs, int id); //sprint toward opposition player (taunting)
Input walkToOwnGoal(GameState* gs, int id); //walk forlornly to own goal

#endif // _AI_H
