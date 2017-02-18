#ifndef _GAMESTATE_H
#define _GAMESTATE_H

/***********************************************************************
************************************************************************
*****  Collection of structures to make handling game elements more  ***
*****  simple                                                        ***
************************************************************************
***********************************************************************/

#include <SDL2/SDL.h>
#include "GameObject.h"
#include "Player.h"
#include "Utility/Geometry.h"
#include "PhysicalController.h"
#include "AnalogueController.h"
#include "EventHandler.h"
#include "Goal.h"

#define PhysicalController SDL_Joystick

//states of play
typedef enum PlayState
{
    NORMAL_PLAY,        //Normal game play, starts from reset positions
    GOAL_SCORED,        //Input ignored, short period of celebration
    PENALTY,            //Rule infraction resulting in penalty kick
    GAME_OVER           //Game over
}PlayState;

typedef struct GameState
{
    //current play state
    PlayState currPlayState;

    //Game objects
    Player players[2];
    GameObject* ball;
    Rect pitch;
    Goal goals[2];

    //Input elements
    AnalogueController controllers[3]; //TODO: collect this into single structure with "getInput" function

}GameState;

//create destroy functions for gamestate
GameState* GS_initializeGameState();
void GS_destroyGameState(GameState* gs);

//Game state functions
void GS_loadGameObjects(GameState* gs);
void GS_registerTouchHandlers(GameState* gs);

//Gamestate physics update
void GS_updatePhysics(GameState* gs);

#endif // _GAMESTATE_H
