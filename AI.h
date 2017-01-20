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

typedef struct decisionTreeNode* Node;

Node AI_init();
Input AI_getUserInput(GameState* gs, int id, Node start);

#endif // _AI_H
