/**************************************************************************
***************************************************************************
***                                                                     ***
*** A module that recieves all touch events from SDL and delegates a    ***
*** handler (function that handles touchs in a UI widget). If the       ***
*** touched area is not 'owned' by any registered handler, the event    ***
*** goes unhandled.                                                     ***
***                                                                     ***
***************************************************************************
**************************************************************************/

#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Utility/Geometry.h"
#include "Utility/List.h"

typedef bool (*EventHandler)(void*, SDL_Event*);

//Register an object as a handler for events
void EH_registerHandler(Rect touchableArea, EventHandler evHan, bool needsNotification, void* selfRef);

//Actual event distribution function
bool EH_handleEvent(SDL_Event* ev);

//create/destroy event handler
void EH_init();
void EH_destroy();

#endif // _EVENT_HANDLER_H
