#ifndef _TOUCHSCREENCONTROL_H_
#define _TOUCHSCREENCONTROL_H_

/***************************************************************************
****************************************************************************
********           Touchscreen Interface
********           =====================
********
********  Onscreen analogues controllers take up real estate, but fingers
********  badly obstruct the view. This Interface gives user analogue
********  controller like functionality, but takes up no space, and fingers
********  can be kept out of the action.
********
********  Author: A. Connolly
********  Date: 2017/4/14
***************************************************************************/

#ifdef __ANDROID__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <stdbool.h>
#include "Utility/Geometry.h"
#include "GameState.h"

extern bool TS_resetKickSwitch;

// init touchscreen control module
void TS_init(GameState* gs, int maxMag);

// get vectors representing current inputs
Vec3D TS_getDirInput();
Vec3D TS_getKickInput();
bool TS_notInit(Vec3D v);
bool TS_noTarget(Vec3D v);

// event handling
bool TS_handleEvent(void* tc, SDL_Event* e);

#endif