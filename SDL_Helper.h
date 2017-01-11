#ifndef _SDL_HELPER_H
#define _SDL_HELPER_H

#include <stdbool.h>

//defined functions
#define isMouseEvent(e) ((e).type == SDL_MOUSEBUTTONUP || (e).type == SDL_MOUSEBUTTONDOWN || (e).type == SDL_MOUSEMOTION)
#define isJoystickEvent(e) ((e).type == SDL_JOYAXISMOTION || (e).type == SDL_JOYBUTTONDOWN || (e).type == SDL_JOYBUTTONUP || \
                            (e).type == SDL_JOYDEVICEREMOVED || (e).type == SDL_JOYDEVICEADDED)
#define isMouseBL(e) (((e)->motion.state&SDL_BUTTON_LMASK)==SDL_BUTTON_LMASK)
#define isMouseBR(e) (((e)->motion.state&SDL_BUTTON_RMASK)==SDL_BUTTON_RMASK)

//init SDL for this game
bool SDL_Helper_init();
void SDL_Helper_quit();

#endif // _SDL_HELPER_H
