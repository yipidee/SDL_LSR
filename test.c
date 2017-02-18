#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "EventHandler.h"
#include "Draw.h"
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
void updatePhysics(GameState* gs, Input input1, Input input2);
void checkRules(GameState* gs);
void resetGamePositions(GameState* gs);
void takePenaltyPositions(GameState* gs);
void hideControls();

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

    //each player's input
    Input input1, input2;

    //AI decision tree to use
    DecisionTree dt = AI_parseDecisionTree(DT_DEFAULT);
    DecisionTree celebrationTree = dt;//AI_parseDecisionTree(DT_DEFAULT);

    //labels
    char p1score[20];
    char p2score[20];
    char p1touches[20];
    char p2touches[20];

    TextLabel p1Name = TL_createTextLabel("McDoodle", 0, 0);
    TL_setFont(p1Name, NULL);
    TL_setFontSize(p1Name, 20);

    TextLabel p2Name = TL_createTextLabel("Calfnuts", 0, 0);
    TL_setFont(p2Name, NULL);
    TL_setFontSize(p2Name, 20);

    TextLabel p1Score = TL_createTextLabel(NULL, 0, 0);
    TL_setFont(p1Score, NULL);
    TL_setFontSize(p1Score, 20);

    TextLabel p2Score = TL_createTextLabel(NULL, 0, 0);
    TL_setFont(p2Score, NULL);
    TL_setFontSize(p2Score, 20);

    TextLabel p1Touches = TL_createTextLabel(NULL, 0, 0);
    TL_setFont(p1Touches, NULL);
    TL_setFontSize(p1Touches, 20);
    
    TextLabel p2Touches = TL_createTextLabel(NULL, 0, 0);
    TL_setFont(p2Touches, NULL);
    TL_setFontSize(p2Touches, 20);

    //Set label positions
    int a = TL_getWidth(p1Name);
    int b = TL_getWidth(p2Name);
    int c = fmax(a, b);
    c = SCREEN_WIDTH - c - 15;
    TL_setX(p1Name, c);
    TL_setX(p2Name, c);
    TL_setX(p1Score, c);
    TL_setX(p2Score, c);
    TL_setX(p1Touches, c);
    TL_setX(p2Touches, c);

    a = TL_getHeight(p1Name);
    b = TL_getHeight(p2Name);
    c = fmax(a, b);
    TL_setY(p2Name, 5);
    TL_setY(p2Score, TL_getY(p2Name) + TL_getHeight(p2Name));
    TL_setY(p2Touches, TL_getY(p2Score) + TL_getHeight(p2Name));
    TL_setY(p1Name, SCREEN_HEIGHT - 105 - 3 * c);
    TL_setY(p1Score, TL_getY(p1Name) + TL_getHeight(p1Name));
    TL_setY(p1Touches, TL_getY(p1Score) + TL_getHeight(p1Name));
 
    bool resetPositions = true;

    /*************************************************************
    *********              GAME LOOP           *******************
    *************************************************************/

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

        //for info only, prints out current state of play for tick
        switch(gs->currPlayState)
        {
            case NORMAL_PLAY:
                printf("normal play\n");
                break;
            case GOAL_SCORED:
                printf("goal\n");
                break;
            case PENALTY:
                printf("penalty\n");
                break;
            case GAME_OVER:
                printf("game over...\n");
                break;
        }

        //Step 0: check for reset or change in positions
        if(resetPositions){
            resetGamePositions(gs);
            resetPositions = false;
        }
        if(gs->currPlayState==PENALTY)takePenaltyPositions(gs);

        //Step 1: get input from user (if in playstate requiring input)
        if(gs->currPlayState==NORMAL_PLAY || gs->currPlayState==PENALTY)
        {
            input1 = UI_getUserInput();
            input2 = AI_getUserInput(gs, 1, dt);
            if(PhysCont_PhysicalControllerPresent())drawControlsOnScreen(input1);
        }else if(gs->currPlayState == GOAL_SCORED)
        {
            static int goalStateTickCounter = 0;
            input1 = AI_getUserInput(gs, 0, celebrationTree);
            input2 = AI_getUserInput(gs, 1, celebrationTree);
            if(PhysCont_PhysicalControllerPresent())hideControls();

            ++goalStateTickCounter;
            if(goalStateTickCounter > 200)
            {
                gs->currPlayState = NORMAL_PLAY;
                resetPositions = true;
                goalStateTickCounter = 0;
            }
        }

        //Step 2: Update physics
        //update positions
        updatePhysics(gs, input1, input2);
        if(gs->currPlayState == NORMAL_PLAY)checkRules(gs);

        if(gs->currPlayState != PENALTY)
        {
            if(Goal_scored(gs->goals[0], gs->ball))
            {
                Player_incrementScore(gs->players[1]);
                Player_setLastScorer(gs->players[1], true);
                Player_setLastScorer(gs->players[0], false);
                gs->currPlayState = GOAL_SCORED;
            }
            else if(Goal_scored(gs->goals[1], gs->ball))
            {
                Player_incrementScore(gs->players[0]);
                Player_setLastScorer(gs->players[0], true);
                Player_setLastScorer(gs->players[1], false);
                gs->currPlayState = GOAL_SCORED;
            }
        }else
        {
            //do penalty related stuff here

        }

        //Step 3: draw result
        Draw_renderScene();
        snprintf(p1score, 20, "goals %i", gs->players[0]->score);
        snprintf(p2score, 20, "goals %i", gs->players[1]->score);
        snprintf(p1touches, 20, "touches %i", gs->players[0]->touches);
        snprintf(p2touches, 20, "touches %i", gs->players[1]->touches);
        TL_setText(p1Score, p1score);
        TL_setText(p2Score, p2score);
        TL_setText(p1Touches, p1touches);
        TL_setText(p2Touches, p2touches);
        TL_renderTextLabel(p1Name);
        TL_renderTextLabel(p2Name);
        TL_renderTextLabel(p1Score);
        TL_renderTextLabel(p2Score);
        TL_renderTextLabel(p1Touches);
        TL_renderTextLabel(p2Touches);
    }
    AI_freeDecisionTree(dt);
    dt = NULL;
    TL_destroyTextLabel(p1Name);
    TL_destroyTextLabel(p2Name);
    TL_destroyTextLabel(p1Score);
    TL_destroyTextLabel(p2Score);
    TL_destroyTextLabel(p1Touches);
    TL_destroyTextLabel(p2Touches);
    p1Name = NULL;
    p2Name = NULL;
    p1Score = NULL;
    p2Score = NULL;
    p1Touches = NULL;
    p2Touches = NULL;
    releaseResources();

    return 0;
}

void hideControls()
{
    ;
}

void checkRules(GameState* gs)
{
    //Check 1: too many touches
    if(Player_getTouches(gs->players[0]) < 0)
    {
        Player_setPenaltyFlag(gs->players[0]);
        gs->currPlayState = PENALTY;
        return;
    }else if(Player_getTouches(gs->players[1]) < 0)
    {
        Player_setPenaltyFlag(gs->players[1]);
        gs->currPlayState = PENALTY;
        return;
    }

    //Check 2: outside own half when opposition has touches
    if(Rect_containsCircle(gs->players[1]->ownHalf, gs->ball->BCirc)&&!Player_hasTouches(gs->players[1]))
        Player_setCanLeaveHalf(gs->players[0], true);
    if(Rect_containsCircle(gs->players[0]->ownHalf, gs->ball->BCirc)&&!Player_hasTouches(gs->players[0]))
        Player_setCanLeaveHalf(gs->players[1], true);

    if(!Player_isInOwnHalf(gs->players[0]) && !Player_canLeaveOwnHalf(gs->players[0]))
    {
        Player_setPenaltyFlag(gs->players[0]);
        gs->currPlayState = PENALTY;
        return;
    }else if(!Player_isInOwnHalf(gs->players[1]) && !Player_canLeaveOwnHalf(gs->players[1]))
    {
        Player_setPenaltyFlag(gs->players[1]);
        gs->currPlayState = PENALTY;
        return;
    }

    if(Player_isInOwnHalf(gs->players[0]) && Player_hasTouches(gs->players[1]))
        Player_setCanLeaveHalf(gs->players[0], false);
    if(Player_isInOwnHalf(gs->players[1]) && Player_hasTouches(gs->players[0]))
        Player_setCanLeaveHalf(gs->players[1], false);
}

void resetGamePositions(GameState* gs)
{
    Player_setPos(gs->players[0], Vec3D_makeVector(POS_PLAYER1_START));
    Player_setPos(gs->players[1], Vec3D_makeVector(POS_PLAYER2_START));
    GO_setPos(gs->ball, Vec3D_makeVector(POS_BALL_START));
}

void takePenaltyPositions(GameState* gs)
{
    if(Player_concededPenalty(gs->players[0]))
    {
        Player_setPos(gs->players[0], Vec3D_makeVector(POS_PENALTY_S_RECEIVER));
        Player_setPos(gs->players[1], Vec3D_makeVector(POS_PENALTY_S_TAKER));
        GO_setPos(gs->ball, Vec3D_makeVector(POS_BALL_START));
    }else
    {
        Player_setPos(gs->players[0], Vec3D_makeVector(POS_PENALTY_N_TAKER));
        Player_setPos(gs->players[1], Vec3D_makeVector(POS_PENALTY_N_RECEIVER));
        GO_setPos(gs->ball, Vec3D_makeVector(POS_BALL_START));
    }
    Player_clearPenaltyFlag(gs->players[0]);
    Player_clearPenaltyFlag(gs->players[1]);
}

void updatePhysics(GameState* gs, Input input1, Input input2)
{
    //Move Player 1
    Player_setVel(gs->players[0], getVelFromInput(input1));
    if(Vec3D_isZero(Player_getVel(gs->players[0])))
    {
        Player_setIsStationary(gs->players[0], true);
    }else
    {
        Player_setIsStationary(gs->players[0], false);
    }
    Player_move(gs->players[0]);

    //Move Player 2
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

    //collisions between ball and goals posts
    if(GO_isInContact(Goal_getLPost(gs->goals[0]), gs->ball)){Phys_conservationMomentumCollision2D(Goal_getLPost(gs->goals[0]), gs->ball, CONS_BALL_WALL_COR);}
    else if(GO_isInContact(Goal_getRPost(gs->goals[0]), gs->ball)){Phys_conservationMomentumCollision2D(Goal_getRPost(gs->goals[0]), gs->ball, CONS_BALL_WALL_COR);}
    else if(GO_isInContact(Goal_getLPost(gs->goals[1]), gs->ball)){Phys_conservationMomentumCollision2D(Goal_getLPost(gs->goals[1]), gs->ball, CONS_BALL_WALL_COR);}
    else if(GO_isInContact(Goal_getRPost(gs->goals[1]), gs->ball)){Phys_conservationMomentumCollision2D(Goal_getRPost(gs->goals[1]), gs->ball, CONS_BALL_WALL_COR);}

    //collisions between player 1 and goal posts
    if(GO_isInContact(Player_getGameObject(gs->players[0]), Goal_getLPost(gs->goals[0])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[0]), Goal_getLPost(gs->goals[0]));
    } else if(GO_isInContact(Player_getGameObject(gs->players[0]), Goal_getRPost(gs->goals[0])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[0]), Goal_getRPost(gs->goals[0]));
    } else if(GO_isInContact(Player_getGameObject(gs->players[0]), Goal_getLPost(gs->goals[1])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[0]), Goal_getLPost(gs->goals[1]));
    } else if(GO_isInContact(Player_getGameObject(gs->players[0]), Goal_getRPost(gs->goals[1])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[0]), Goal_getRPost(gs->goals[1]));
    }

    //collisions between player 2 and goal posts
    if(GO_isInContact(Player_getGameObject(gs->players[1]), Goal_getLPost(gs->goals[1])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[1]), Goal_getLPost(gs->goals[1]));
    } else if(GO_isInContact(Player_getGameObject(gs->players[1]), Goal_getRPost(gs->goals[1])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[1]), Goal_getRPost(gs->goals[1]));
    } else if(GO_isInContact(Player_getGameObject(gs->players[1]), Goal_getLPost(gs->goals[0])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[1]), Goal_getLPost(gs->goals[0]));
    } else if(GO_isInContact(Player_getGameObject(gs->players[1]), Goal_getRPost(gs->goals[0])))
    {
        Phys_adjustForCollisionWithStatObject(Player_getGameObject(gs->players[1]), Goal_getRPost(gs->goals[0]));
    }

    //check and rectify for collisions
    // Player1 and ball
    bool lastTickContact = Player_touchingBall(gs->players[0]);
    collisionWithFreeObject(Player_getGameObject(gs->players[0]), gs->ball, input1, &gs->players[0]->touchingBall);
    if(((lastTickContact == false)&&(Player_touchingBall(gs->players[0]) == true)) || 
            ((lastTickContact == true)&&Vec3D_isZero(Player_getVel(gs->players[0]))&&
            Circle_containsPoint(Player_getGameObject(gs->players[0])->BCirc, GO_getPos(gs->ball).i,GO_getPos(gs->ball).j)))
    {
        Player_decrementTouches(gs->players[0]);
        Player_resetTouches(gs->players[1]);
    }

    // Player2 and ball
    lastTickContact = Player_touchingBall(gs->players[1]);
    collisionWithFreeObject(Player_getGameObject(gs->players[1]), gs->ball, input1, &gs->players[1]->touchingBall);
    if(((lastTickContact == false)&&(Player_touchingBall(gs->players[1]) == true)) || 
            ((lastTickContact == true)&&Vec3D_isZero(Player_getVel(gs->players[1]))&&
            Circle_containsPoint(Player_getGameObject(gs->players[1])->BCirc, GO_getPos(gs->ball).i,GO_getPos(gs->ball).j)))
    {
        Player_decrementTouches(gs->players[1]);
        Player_resetTouches(gs->players[0]);
    }

    collisionWithEnergisedObject(Player_getGameObject(gs->players[0]), Player_getGameObject(gs->players[1]));
}

//TODO move this to Physics
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
        knobPos = Vec3D_add(knobPos, Vec3D_scalarMult(UI_getShotVec(input), (gs->controllers[1].base.r - gs->controllers[1].knob.r)));
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

    //sprites for posts of goals
    Sprite post1_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post1_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    Sprite_posByCentre(post1_s, true);
    Sprite_setSpriteInWorldPosRef(post1_s, &Goal_getLPost(gs->goals[0])->pos.i, &Goal_getLPost(gs->goals[0])->pos.j, NULL);

    Sprite post2_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post2_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    Sprite_posByCentre(post2_s, true);
    Sprite_setSpriteInWorldPosRef(post2_s, &Goal_getRPost(gs->goals[0])->pos.i, &Goal_getRPost(gs->goals[0])->pos.j, NULL);

    Sprite post3_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post3_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    Sprite_posByCentre(post3_s, true);
    Sprite_setSpriteInWorldPosRef(post3_s, &Goal_getLPost(gs->goals[1])->pos.i, &Goal_getLPost(gs->goals[1])->pos.j, NULL);

    Sprite post4_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post4_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    Sprite_posByCentre(post4_s, true);
    Sprite_setSpriteInWorldPosRef(post4_s, &Goal_getRPost(gs->goals[1])->pos.i, &Goal_getRPost(gs->goals[1])->pos.j, NULL);

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
