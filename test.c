#include <stdbool.h>
#include <SDL2/SDL.h>
#include "EventHandler.h"
#include "AnalogueController.h"
#include "Draw.h"
#include "Constants.h"

// prototypes
void releaseResources();

int main(int argc, char* argv[])
{
    /****************************************************************
	*****************************************************************
	**********************                ***************************
	**********************    TEST AREA   ***************************
	**********************                ***************************
	*****************************************************************
	*****************************************************************/

    //first controller
    AnalogueController controller1 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setPosition(&controller1, 51, 51);
    AnalCont_setSize(&controller1, 100);
    AnalCont_setKnobSize(&controller1, 60);

    //second controller
    AnalogueController controller2 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setPosition(&controller2, 151, 51);
    AnalCont_setSize(&controller2, 80);
    AnalCont_setKnobSize(&controller2, 50);

    //register controllers with touch handler
    EH_registerHandler(controller1.touchableArea, controller1.evHan, true, &controller1);
    EH_registerHandler(controller2.touchableArea, controller2.evHan, true, &controller2);

    //create sprites associated with controllers
    Sprite c1Back, c1Knob;
    c1Back = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, 0, 0, 0, NULL);
    c1Knob = Sprite_createSprite(PATH_TO_RED_CONTROLLER, 0, 0, 0, NULL);
    Sprite_setSpriteInWorldDims(c1Back, controller1.base.r*2, controller1.base.r*2);
    Sprite_setSpriteInWorldDims(c1Knob, controller1.knob.r*2, controller1.knob.r*2);
    Sprite_posByCentre(c1Back, true);
    Sprite_posByCentre(c1Knob, true);
    Sprite_setSpriteInWorldPosRef(c1Back, &controller1.base.x, &controller1.base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c1Knob, &controller1.knob.x, &controller1.knob.y, NULL);

    Sprite c2Back, c2Knob;
    c2Back = Sprite_createSprite(PATH_TO_RED_CONTROLLER, 0, 0, 0, NULL);
    c2Knob = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, 0, 0, 0, NULL);
    Sprite_setSpriteInWorldDims(c2Back, controller2.base.r*2, controller2.base.r*2);
    Sprite_setSpriteInWorldDims(c2Knob, controller2.knob.r*2, controller2.knob.r*2);
    Sprite_posByCentre(c2Back, true);
    Sprite_posByCentre(c2Knob, true);
    Sprite_setSpriteInWorldPosRef(c2Back, &controller2.base.x, &controller2.base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c2Knob, &controller2.knob.x, &controller2.knob.y, NULL);


    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )quit = true;
            else
            {
                if(e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION) EH_handleEvent(&e);
            }
        }
        Draw_renderScene();
    }

    releaseResources();
}

void releaseResources()
{
    EH_destroy();
    Draw_quit();
}
