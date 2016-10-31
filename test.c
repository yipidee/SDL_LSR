#include <stdbool.h>
#include <SDL2/SDL.h>
#include "EventHandler.h"
#include "AnalogueController.h"
#include "Draw.h"
#include "Constants.h"
#include "GameObject.h"

// prototypes
void releaseResources();

int main(int argc, char* argv[])
{
    //first controller
    AnalogueController controller1 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setSize(&controller1, 100);
    AnalCont_setKnobSize(&controller1, 60);
    AnalCont_setPosition(&controller1, controller1.base.r+10, SCREEN_HEIGHT-controller1.base.r-10);

    //second controller
    AnalogueController controller2 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setSize(&controller2, 80);
    AnalCont_setKnobSize(&controller2, 50);
    AnalCont_setPosition(&controller2, SCREEN_WIDTH-controller2.base.r-10, SCREEN_HEIGHT-controller2.base.r-30);

    //thirdd controller
    AnalogueController controller3 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setSize(&controller3, 80);
    AnalCont_setKnobSize(&controller3, 50);
    AnalCont_setPosition(&controller3, SCREEN_WIDTH-2*controller2.base.r-controller3.base.r-10, SCREEN_HEIGHT-controller3.base.r-10);

    //register controllers with touch handler
    EH_registerHandler(controller1.touchableArea, controller1.evHan, true, &controller1);
    EH_registerHandler(controller2.touchableArea, controller2.evHan, true, &controller2);
    EH_registerHandler(controller3.touchableArea, controller3.evHan, true, &controller3);

    //logical game object, player
    GameObject player = GO_createGameObject();
    Vec3D playerStartPosition = {250,250,0};
    GO_setPos(&player, playerStartPosition);
    Circle playerBounds = {0,0,50};
    GO_setBCirc(&player, playerBounds);

    //logical game object, ball
    GameObject ball = GO_createGameObject();
    Vec3D ballStartPosition = {250,500,0};
    GO_setPos(&ball, ballStartPosition);
    Circle ballBounds = {0,0,25};
    GO_setBCirc(&ball, ballBounds);

    //sprite rep of player
    Sprite player_s = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(player_s, 100, 100);
    Sprite_posByCentre(player_s, true);
    Sprite_setSpriteInWorldPosRef(player_s, &player.pos.i, &player.pos.j, NULL);

    //sprite rep of ball
    Sprite ball_s = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(ball_s, 50, 50);
    Sprite_posByCentre(ball_s, true);
    Sprite_setSpriteInWorldPosRef(ball_s, &ball.pos.i, &ball.pos.j, NULL);

    //create sprites associated with controllers
    Sprite c1Back, c1Knob;
    c1Back = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c1Knob = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c1Back, controller1.base.r*2, controller1.base.r*2);
    Sprite_setSpriteInWorldDims(c1Knob, controller1.knob.r*2, controller1.knob.r*2);
    Sprite_posByCentre(c1Back, true);
    Sprite_posByCentre(c1Knob, true);
    Sprite_setSpriteInWorldPosRef(c1Back, &controller1.base.x, &controller1.base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c1Knob, &controller1.knob.x, &controller1.knob.y, NULL);

    Sprite c2Back, c2Knob;
    c2Back = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c2Knob = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c2Back, controller2.base.r*2, controller2.base.r*2);
    Sprite_setSpriteInWorldDims(c2Knob, controller2.knob.r*2, controller2.knob.r*2);
    Sprite_posByCentre(c2Back, true);
    Sprite_posByCentre(c2Knob, true);
    Sprite_setSpriteInWorldPosRef(c2Back, &controller2.base.x, &controller2.base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c2Knob, &controller2.knob.x, &controller2.knob.y, NULL);

    Sprite c3Back, c3Knob;
    c3Back = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c3Knob = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c3Back, controller3.base.r*2, controller3.base.r*2);
    Sprite_setSpriteInWorldDims(c3Knob, controller3.knob.r*2, controller3.knob.r*2);
    Sprite_posByCentre(c3Back, true);
    Sprite_posByCentre(c3Knob, true);
    Sprite_setSpriteInWorldPosRef(c3Back, &controller3.base.x, &controller3.base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c3Knob, &controller3.knob.x, &controller3.knob.y, NULL);


    //Main loop flag
    bool quit = false;

    int MaxVx = 10;

    //Event handler
    SDL_Event e;
    Vec3D input;
    Vec3D delta;

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
        //get input
        input = AnalCont_getCurrentInput(&controller1);
        delta.i = input.i * MaxVx /100;
        delta.j = input.j * MaxVx /100;

        //update physics
        //update position data
        GO_move(&player, delta);

        //collision detection
        //screen boundaries
        //TODO fix this to move bounding circle too
        if(player.pos.i>=SCREEN_WIDTH-50)player.pos.i=SCREEN_WIDTH-50;
        if(player.pos.j>=SCREEN_HEIGHT-50)player.pos.j=SCREEN_HEIGHT-50;
        if(player.pos.i<=50)player.pos.i=50;
        if(player.pos.j<=50)player.pos.j=50;

        //with ball
        if(GO_isInContact(player, ball))
        {
            printf("contact\n");
        }

        //draw result
        Draw_renderScene();
    }
    releaseResources();
}

void releaseResources()
{
    EH_destroy();
    Draw_quit();
}
