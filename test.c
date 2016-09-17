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

    AnalogueController controller = AnalCont_create();
    AnalCont_setPosition(&controller, 51, 51);
    AnalCont_setSize(&controller, 100);
    AnalCont_setKnobSize(&controller, 60);

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
                if(Rect_containsPoint(AnalCont_getTouchableArea(&controller), e.button.x, e.button.y))
                {
                    AnalCont_handleEvent(&controller, &e);
                }else
                {
                    SDL_Event ev;
                    ev.type = SDL_MOUSEBUTTONUP;
                    ev.button.x = 0;
                    ev.button.y = 0;
                    AnalCont_handleEvent(&controller, &ev);
                }
            }
            //Vec3D_print(AnalCont_getCurrentInput(&controller));
        }

        Draw_controller(&controller);
        Draw_renderScene();
    }

    Draw_quit();
}
