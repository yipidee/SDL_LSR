#include <stdbool.h>
#include <SDL2/SDL.h>
//#include <math.h>
#include "EventHandler.h"
//#include "AnalogueController.h"
#include "Draw.h"
#include "Constants.h"
#include "GameObject.h"
#include "Physics.h"
//#include "PhysicalController.h"
//#include "SDL_Helper.h"
#include "Player.h"
#include "GameState.h"
#include "UserInput.h"

// prototypes
void releaseResources();
void loadSprites();

//globally available pointer to game state
GameState* gs;

int main(int argc, char* argv[])
{
    Draw_init();
    gs = GS_initializeGameState();
    GS_registerTouchHandlers(gs);
    GS_loadGameObjects(gs);
    UI_setOnscreenControlRef(&gs->controllers[0], &gs->controllers[1], &gs->controllers[2]);
    loadSprites();

    //Pitch boundary TODO: Move to gamestate initialisation
    gs->pitch = Rect_create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;
    Input input;
    //Vec3D delta = VECTOR_ZERO;

    Vec3D impulse = VECTOR_ZERO;

    bool contacted = false;

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
                if(isMouseEvent(e))
                {
                    EH_handleEvent(&e);
                }else if(isJoystickEvent(e))
                {
                    PhysCont_handleEvent(&e);
                }
            }
        }

        //get input
        input = UI_getUserInput();
        //delta = Vec3D_scalarMult(UI_getDirVec(input), CONS_MAX_SPEED);

        Vec3D knobPos = {gs->controllers[0].base.x, gs->controllers[0].base.y, 0};

        if(!Vec3D_equal(UI_getDirVec(input), VECTOR_ZERO))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(UI_getDirVec(input), (gs->controllers[0].base.r - gs->controllers[0].knob.r)));
        gs->controllers[0].knob.x = knobPos.i;
        gs->controllers[0].knob.y = knobPos.j;

        if(!Vec3D_equal(UI_getConVec(input),VECTOR_ZERO))
        {
            knobPos.i = gs->controllers[2].base.x;
            knobPos.j = gs->controllers[2].base.y;
            if(!Vec3D_equal(UI_getConVec(input), VECTOR_ZERO))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(UI_getConVec(input), (gs->controllers[2].base.r - gs->controllers[2].knob.r)));
            gs->controllers[2].knob.x = knobPos.i;
            gs->controllers[2].knob.y = knobPos.j;
            gs->controllers[1].knob.x = gs->controllers[1].base.x;
            gs->controllers[1].knob.y = gs->controllers[1].base.y;
        }else
        {
            knobPos.i = gs->controllers[1].base.x;
            knobPos.j = gs->controllers[1].base.y;
            if(!Vec3D_equal(UI_getShotVec(input), VECTOR_ZERO))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(UI_getShotVec(input), (gs->controllers[1].base.r - gs->controllers[1].knob.r)));
            gs->controllers[1].knob.x = knobPos.i;
            gs->controllers[1].knob.y = knobPos.j;
            gs->controllers[2].knob.x = gs->controllers[2].base.x;
            gs->controllers[2].knob.y = gs->controllers[2].base.y;
        }
        Player_setVel(gs->players[0], Vec3D_scalarMult(UI_getDirVec(input), CONS_MAX_SPEED));
        
        //move below functionality to physics
        GO_zeroReversedDirections(gs->ball);
        //delta = Vec3D_add(GO_getVel(gs->ball), GO_getAcc(gs->ball));
        GO_setVel(gs->ball, Vec3D_add(GO_getVel(gs->ball), GO_getAcc(gs->ball)));

        Player_move(gs->players[0]);
        GO_move(gs->ball, GO_getVel(gs->ball));

        //collision detection
        //player pitch boundary
        if(Player_isInContactWithBoundary(gs->players[0], gs->pitch)) Player_adjustForBoundary(gs->players[0], gs->pitch);
        //gs->ball collides with walls
        if(Phys_inCollisionWithBoundary(gs->ball, gs->pitch)) Phys_boundaryCollision(gs->ball, gs->pitch);

        //with gs->ball
        if(GO_isInContact(Player_getGameObject(gs->players[0]), gs->ball))
        {
            if(!contacted)
            {
                impulse = Vec3D_equal(UI_getConVec(input), VECTOR_ZERO) ? Vec3D_scalarMult(UI_getShotVec(input), CONS_MAX_APPLIABLE_IMPULSE): Vec3D_scalarMult(UI_getConVec(input), CONS_MAX_CONTROL_IMPULSE);
                if(Vec3D_equal(impulse, VECTOR_ZERO))
                {
                    Phys_conservationMomentumCollision2D(gs->players[0]->go, gs->ball, CONS_BALL_PLAYER_COR);
                }else
                {
                    Phys_appliedImpulse2D(gs->ball, impulse);
                }
            }
            contacted = true;
        }else
        {
            contacted = false;
        }

        //draw result
        Draw_renderScene();
    }
    releaseResources();

    return 0;
}

void releaseResources()
{
    EH_destroy();
    GS_destroyGameState(gs);
    Draw_quit();
}

void loadSprites()
{
    //sprite rep of players
    Sprite player_s = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(player_s, SIZE_PLAYER_W, SIZE_PLAYER_H);
    Sprite_posByCentre(player_s, true);
    Sprite_setSpriteInWorldPosRef(player_s, &gs->players[0]->go->pos.i, &gs->players[0]->go->pos.j, NULL);

    Sprite player_cs = Sprite_createSprite(PATH_TO_BLUE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(player_cs, SIZE_PLAYER_W, SIZE_PLAYER_H);
    Sprite_posByCentre(player_cs, true);
    Sprite_setSpriteInWorldPosRef(player_cs, &gs->players[1]->go->pos.i, &gs->players[1]->go->pos.j, NULL);

    //sprite rep of ball
    Sprite ball_s = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(ball_s, SIZE_BALL_W, SIZE_BALL_H);
    Sprite_posByCentre(ball_s, true);
    Sprite_setSpriteInWorldPosRef(ball_s, &gs->ball->pos.i, &gs->ball->pos.j, NULL);


    //create sprites associated with controllers
    Sprite c1Back, c1Knob;
    c1Back = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c1Knob = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c1Back, gs->controllers[0].base.r*2, gs->controllers[0].base.r*2);
    Sprite_setSpriteInWorldDims(c1Knob, gs->controllers[0].knob.r*2, gs->controllers[0].knob.r*2);
    Sprite_posByCentre(c1Back, true);
    Sprite_posByCentre(c1Knob, true);
    Sprite_setSpriteInWorldPosRef(c1Back, &gs->controllers[0].base.x, &gs->controllers[0].base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c1Knob, &gs->controllers[0].knob.x, &gs->controllers[0].knob.y, NULL);

    Sprite c2Back, c2Knob;
    c2Back = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c2Knob = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c2Back, gs->controllers[1].base.r*2, gs->controllers[1].base.r*2);
    Sprite_setSpriteInWorldDims(c2Knob, gs->controllers[1].knob.r*2, gs->controllers[1].knob.r*2);
    Sprite_posByCentre(c2Back, true);
    Sprite_posByCentre(c2Knob, true);
    Sprite_setSpriteInWorldPosRef(c2Back, &gs->controllers[1].base.x, &gs->controllers[1].base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c2Knob, &gs->controllers[1].knob.x, &gs->controllers[1].knob.y, NULL);

    Sprite c3Back, c3Knob;
    c3Back = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c3Knob = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c3Back, gs->controllers[2].base.r*2, gs->controllers[2].base.r*2);
    Sprite_setSpriteInWorldDims(c3Knob, gs->controllers[2].knob.r*2, gs->controllers[2].knob.r*2);
    Sprite_posByCentre(c3Back, true);
    Sprite_posByCentre(c3Knob, true);
    Sprite_setSpriteInWorldPosRef(c3Back, &gs->controllers[2].base.x, &gs->controllers[2].base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c3Knob, &gs->controllers[2].knob.x, &gs->controllers[2].knob.y, NULL);
}

