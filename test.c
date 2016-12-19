#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "EventHandler.h"
#include "AnalogueController.h"
#include "Draw.h"
#include "Constants.h"
#include "GameObject.h"
#include "Physics.h"
#include "PhysicalController.h"
#include "SDL_Helper.h"
#include "Player.h"

// prototypes
void releaseResources();

int main(int argc, char* argv[])
{
    //first controller
    AnalogueController controller1 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setSize(&controller1, SIZE_CONTROLLER_1);
    AnalCont_setKnobSize(&controller1, SIZE_CONTROLLER_KNOB1);
    AnalCont_setPosition(&controller1, controller1.base.r+10, SCREEN_HEIGHT-controller1.base.r-10);

    //second controller
    AnalogueController controller2 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setSize(&controller2, SIZE_CONTROLLER_2);
    AnalCont_setKnobSize(&controller2, SIZE_CONTROLLER_KNOB2);
    AnalCont_setPosition(&controller2, SCREEN_WIDTH-controller2.base.r-10, SCREEN_HEIGHT-controller2.base.r-30);

    //third controller
    AnalogueController controller3 = AnalCont_create(ANALOGUE_MODE);
    AnalCont_setSize(&controller3, SIZE_CONTROLLER_3);
    AnalCont_setKnobSize(&controller3, SIZE_CONTROLLER_KNOB3);
    AnalCont_setPosition(&controller3, SCREEN_WIDTH-2*controller2.base.r-controller3.base.r-10, SCREEN_HEIGHT-controller3.base.r-10);

    //register controllers with touch handler
    EH_registerHandler(controller1.touchableArea, controller1.evHan, true, &controller1);
    EH_registerHandler(controller2.touchableArea, controller2.evHan, true, &controller2);
    EH_registerHandler(controller3.touchableArea, controller3.evHan, true, &controller3);

    //logical game object, player
    GameObject player = GO_createGameObject();
    Vec3D playerStartPosition = {250,250,0};
    GO_setPos(&player, playerStartPosition);
    Circle playerBounds = {0,0,SIZE_PLAYER_H / 2};
    GO_setBCirc(&player, playerBounds);
    GO_setMass(&player, CONS_MASS_PLAYER);

    Player mccoy = Player_create(player);

    //logical game object, ball
    GameObject ball = GO_createGameObject();
    Vec3D ballStartPosition = {250,500,0};
    GO_setPos(&ball, ballStartPosition);
    Circle ballBounds = {0,0,SIZE_BALL_H / 2};
    GO_setBCirc(&ball, ballBounds);
    GO_setMass(&ball, CONS_MASS_BALL);

    //sprite rep of player
    Sprite player_s = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(player_s, SIZE_PLAYER_W, SIZE_PLAYER_H);
    Sprite_posByCentre(player_s, true);
    Sprite_setSpriteInWorldPosRef(player_s, &mccoy->go.pos.i, &mccoy->go.pos.j, NULL);

    //sprite rep of ball
    Sprite ball_s = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(ball_s, SIZE_BALL_W, SIZE_BALL_H);
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

    //Pitch boundary
    const Rect PitchBoundary = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;
    Vec3D input = VECTOR_ZERO;
    Vec3D delta = VECTOR_ZERO;

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
        //input = AnalCont_getCurrentInput(&controller1);
        input = PhysCont_getLeftStickInput();
        delta = Vec3D_scalarMult(input, CONS_MAX_SPEED);

        Vec3D knobPos = {controller1.base.x, controller1.base.y, 0};

        if(!Vec3D_equal(input, VECTOR_ZERO))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(PhysCont_getLeftStickInput(), (controller1.base.r - controller1.knob.r)));
        controller1.knob.x = knobPos.i;
        controller1.knob.y = knobPos.j;

        if(!PhysCont_getShotMask())
        {
            input = PhysCont_getRightStickInput();

            knobPos.i = controller3.base.x;
            knobPos.j = controller3.base.y;
            if(!Vec3D_equal(input, VECTOR_ZERO))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(PhysCont_getRightStickInput(), (controller3.base.r - controller3.knob.r)));
            controller3.knob.x = knobPos.i;
            controller3.knob.y = knobPos.j;
            controller2.knob.x = controller2.base.x;
            controller2.knob.y = controller2.base.y;
        }else
        {
            input = PhysCont_getRightStickInput();

            knobPos.i = controller2.base.x;
            knobPos.j = controller2.base.y;
            if(!Vec3D_equal(input, VECTOR_ZERO))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(PhysCont_getRightStickInput(), (controller2.base.r - controller2.knob.r)));
            controller2.knob.x = knobPos.i;
            controller2.knob.y = knobPos.j;
            controller3.knob.x = controller3.base.x;
            controller3.knob.y = controller3.base.y;
        }
        Player_setVel(mccoy, delta);
        Player_setIsStationary(mccoy, Vec3D_equal(Player_getVel(mccoy), VECTOR_ZERO));

        delta = Vec3D_add(ball.vel, ball.acc);

        if(((ball.vel.i >= 0) && (delta.i <= 0)) || ((ball.vel.i <= 0) && (delta.i >= 0))) {delta.i = 0;ball.acc.i=0;}
        if(((ball.vel.j >= 0) && (delta.j <= 0)) || ((ball.vel.j <= 0) && (delta.j >= 0))) {delta.j = 0;ball.acc.j=0;}

        GO_setVel(&ball, delta);

        Player_move(mccoy, Player_getVel(mccoy));
        GO_move(&ball, GO_getVel(&ball));

        //collision detection
        //player pitch boundary
        if(Player_isInContactWithBoundary(mccoy, PitchBoundary)) Player_adjustForBoundary(mccoy, PitchBoundary);
        //ball collides with walls
        if(Phys_inCollisionWithBoundary(&ball, PitchBoundary)) Phys_boundaryCollision(&ball, PitchBoundary);

        //with ball
        if(GO_isInContact(mccoy->go, ball))
        {
            if(!contacted)
            {
                input = PhysCont_getRightStickInput();
                impulse = PhysCont_getShotMask() ? Vec3D_scalarMult(input, CONS_MAX_APPLIABLE_IMPULSE): Vec3D_scalarMult(input, CONS_MAX_CONTROL_IMPULSE);
                if(Vec3D_equal(impulse, VECTOR_ZERO))
                {
                    Phys_conservationMomentumCollision2D(&mccoy->go, &ball, CONS_BALL_PLAYER_COR);
                }else
                {
                    Phys_appliedImpulse2D(&ball, impulse);
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
}

void releaseResources()
{
    EH_destroy();
    Draw_quit();
}
