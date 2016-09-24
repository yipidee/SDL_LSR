#include <stdbool.h>
#include <SDL2/SDL.h>
#include "AnalogueController.h"
#include "Draw.h"

int main(int argc, char* argv[])
{
    /****************************************************************
	*****************************************************************
	**********************                ***************************
	**********************    TEST AREA   ***************************
	**********************                ***************************
	*****************************************************************
	*****************************************************************/

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //first controller
    AnalogueController controller1 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setPosition(&controller1, 51, 51);
    AnalCont_setSize(&controller1, 100);
    AnalCont_setKnobSize(&controller1, 60);
    char* p1 = "/home/adrian/dev/SDL/TT_LSR/Assets/red.png";
    char* p2 = "/home/adrian/dev/SDL/TT_LSR/Assets/orange.png";
    AnalCont_setPathToBImg(&controller1, p1);
    AnalCont_setPathToKImg(&controller1, p2);

    //second controller
    AnalogueController controller2 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setPosition(&controller2, 151, 51);
    AnalCont_setSize(&controller2, 100);
    AnalCont_setKnobSize(&controller2, 60);
    p1 = "/home/adrian/dev/SDL/TT_LSR/Assets/orange.png";
    p2 = "/home/adrian/dev/SDL/TT_LSR/Assets/red.png";
    AnalCont_setPathToBImg(&controller2, p1);
    AnalCont_setPathToKImg(&controller2, p2);

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else
            {
                if(Rect_containsPoint(AnalCont_getTouchableArea(&controller1), e.button.x, e.button.y))
                {
                    AnalCont_handleEvent(&controller1, &e);
                }else
                {
                    SDL_Event ev;
                    ev.type = SDL_MOUSEBUTTONUP;
                    ev.button.x = 0;
                    ev.button.y = 0;
                    AnalCont_handleEvent(&controller1, &ev);
                }
            }
            //Vec3D_print(AnalCont_getCurrentInput(&controller));
        }

        Draw_controller(&controller1);
        Draw_controller(&controller2);
        Draw_renderScene();
    }

    Draw_freeImage(controller1.baseImg);
    Draw_freeImage(controller1.knobImg);
    Draw_quit();
}
