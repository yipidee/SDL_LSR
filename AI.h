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

typedef struct decisionTreeNode* DecisionTree;

DecisionTree AI_init();
Input AI_getUserInput(GameState* gs, int id, DecisionTree dt);

#endif // _AI_H
