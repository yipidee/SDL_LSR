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

#define PhysicalController SDL_Joystick

typedef struct GameState
{
    //Game objects
    Player players[2];
    GameObject* ball;
    Rect pitch;


    //Input elements
    //union controller
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
