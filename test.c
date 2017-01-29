#include <stdbool.h>
#include <SDL2/SDL.h>
#include "EventHandler.h"
#include "Draw.h"
#include "Constants.h"
#include "GameObject.h"
#include "Physics.h"
#include "Player.h"
#include "GameState.h"
#include "UserInput.h"
#include "AI.h"

// prototypes
void releaseResources();
void loadSprites();
void collisionWithFreeObject(GameObject* go1, GameObject* go2, Input in, bool* contacted);
void collisionWithEnergisedObject(GameObject* go1, GameObject* go2);
void drawControlsOnScreen(Input input);

//globally available pointer to game state
GameState* gs;

int main(int argc, char* argv[])
{
    Draw_init();
    gs = GS_initializeGameState();
    GS_registerTouchHandlers(gs);
    GS_loadGameObjects(gs);
    UI_setOnscreenControlRef(&gs->controllers[0], &gs->controllers[1], &gs->controllers[2]);
    AI_init();
    loadSprites();

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;
    Input input1, input2;
    DecisionTree dt = AI_parseDecisionTree("Assets/decision_trees/_default_.dt");

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

        //Step 1: get input from user
        input1 = UI_getUserInput();
        if(PhysCont_PhysicalControllerPresent())drawControlsOnScreen(input1);
        input2 = AI_getUserInput(gs, 1, dt);

        //Step 2: Update physics
        //update positions
        Player_setVel(gs->players[0], getVelFromInput(input1));
        if(Vec3D_isZero(Player_getVel(gs->players[0])))
        {
            Player_setIsStationary(gs->players[0], true);
        }else
        {
            Player_setIsStationary(gs->players[0], false);
        }
        Player_move(gs->players[0]);
        Player_setVel(gs->players[1], getVelFromInput(input2));
        if(Vec3D_isZero(Player_getVel(gs->players[1])))
        {
            Player_setIsStationary(gs->players[1], true);
        }else
        {
            Player_setIsStationary(gs->players[1], false);
        }
        Player_move(gs->players[1]);

        //update ball physics
        GO_setVel(gs->ball, Vec3D_add(GO_getVel(gs->ball), GO_getAcc(gs->ball)));
        GO_zeroReversedDirections(gs->ball);
        GO_move(gs->ball, GO_getVel(gs->ball));

        //collision detection
        //player/ball pitch boundary
        if(Player_isInContactWithBoundary(gs->players[0], gs->pitch)) Player_adjustForBoundary(gs->players[0], gs->pitch);
        if(Player_isInContactWithBoundary(gs->players[1], gs->pitch)) Player_adjustForBoundary(gs->players[1], gs->pitch);
        if(Phys_inCollisionWithBoundary(gs->ball, gs->pitch)) Phys_boundaryCollision(gs->ball, gs->pitch);

        //check and rectify for collisions
        // Player1 and ball
        static bool p1BallContact = false;
        bool lastTickContact = p1BallContact;
        collisionWithFreeObject(Player_getGameObject(gs->players[0]), gs->ball, input1, &p1BallContact);
        if((lastTickContact == false)&&(p1BallContact == true)) --gs->players[0]->touches;

        // Player2 and ball
        static bool p2BallContact = false;
        lastTickContact = p1BallContact;
        collisionWithFreeObject(Player_getGameObject(gs->players[1]), gs->ball, input2, &p2BallContact);
        if((lastTickContact == false)&&(p2BallContact == true)) --gs->players[1]->touches;

        collisionWithEnergisedObject(Player_getGameObject(gs->players[0]), Player_getGameObject(gs->players[1]));

        //Step 3: draw result
        Draw_renderScene();
    }
    AI_freeDecisionTree(dt);
    releaseResources();

    return 0;
}

void collisionWithEnergisedObject(GameObject* go1, GameObject* go2)
{
    if(GO_isInContact(go1, go2))
    {
        Vec3D vec = Vec3D_subtract(GO_getPos(go2), GO_getPos(go1));
        double distanceBetweenCentres = Vec3D_getMagnitude(vec);
        double minAllowableDistance = Circle_getR(&go1->BCirc) + Circle_getR(&go2->BCirc);
        double distToMove = minAllowableDistance - distanceBetweenCentres;

        if(GO_isStationary(go1) && GO_isStationary(go2))
        {
            distToMove/=2;
            GO_setPos(go2, Vec3D_add(GO_getPos(go2), Vec3D_scalarMult(Vec3D_normalise(vec), distToMove)));
            GO_setPos(go1, Vec3D_add(GO_getPos(go1), Vec3D_negate(Vec3D_scalarMult(Vec3D_normalise(vec), distToMove))));
        }else if(GO_isStationary(go1))
        {
            GO_setPos(go1, Vec3D_add(GO_getPos(go1), Vec3D_negate(Vec3D_scalarMult(Vec3D_normalise(vec), distToMove))));
        }else if(GO_isStationary(go2))
        {
            GO_setPos(go2, Vec3D_add(GO_getPos(go2), Vec3D_scalarMult(Vec3D_normalise(vec), distToMove)));
        }
    }
}

void collisionWithFreeObject(GameObject* go1, GameObject* go2, Input input, bool* contacted)
{
    //static bool contacted = false;
    Vec3D impulse = VECTOR_ZERO;

    //with gs->ball
    if(GO_isInContact(go1, go2))
    {
        if(!*contacted)
        {
            impulse = Vec3D_isZero(UI_getConVec(input)) ? getShotFromInput(input): getConFromInput(input);
            if(Vec3D_isZero(impulse))
            {
                Phys_conservationMomentumCollision2D(go1, go2, CONS_BALL_PLAYER_COR);
            }else
            {
                Phys_appliedImpulse2D(go2, impulse);
            }
        }
        *contacted = true;
    }else
    {
        *contacted = false;
    }
}

void drawControlsOnScreen(Input input)
{
    Vec3D knobPos = {gs->controllers[0].base.x, gs->controllers[0].base.y, 0};

    if(!Vec3D_isZero(UI_getDirVec(input)))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(UI_getDirVec(input), (gs->controllers[0].base.r - gs->controllers[0].knob.r)));
    gs->controllers[0].knob.x = knobPos.i;
    gs->controllers[0].knob.y = knobPos.j;

    if(Vec3D_isZero(UI_getShotVec(input)))
    {
        knobPos.i = gs->controllers[2].base.x;
        knobPos.j = gs->controllers[2].base.y;
        if(!Vec3D_isZero(UI_getConVec(input)))knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(UI_getConVec(input), (gs->controllers[2].base.r - gs->controllers[2].knob.r)));
        gs->controllers[2].knob.x = knobPos.i;
        gs->controllers[2].knob.y = knobPos.j;
        gs->controllers[1].knob.x = gs->controllers[1].base.x;
        gs->controllers[1].knob.y = gs->controllers[1].base.y;
    }else
    {
        knobPos.i = gs->controllers[1].base.x;
        knobPos.j = gs->controllers[1].base.y;
        /*if(!Vec3D_isZero(UI_getShotVec(input)))*/knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(UI_getShotVec(input), (gs->controllers[1].base.r - gs->controllers[1].knob.r)));
        gs->controllers[1].knob.x = knobPos.i;
        gs->controllers[1].knob.y = knobPos.j;
        gs->controllers[2].knob.x = gs->controllers[2].base.x;
        gs->controllers[2].knob.y = gs->controllers[2].base.y;
    }
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
