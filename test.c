#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#ifdef __ANDROID__
#include <SDL.h>
#include <android/log.h>
#include <SDL_video.h>

#else
#include <SDL2/SDL.h>
#endif
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
//void drawControlsOnScreen(Input input);
void updatePhysics(GameState* gs, Input input1, Input input2);
void checkRules(GameState* gs, bool* resetPositions);
void checkForGoal(GameState* gs);
void resetGamePositions(GameState* gs);
void takePenaltyPositions(GameState* gs);
void normalPlayTick(bool* resetPositions, DecisionTree dt);
void penaltyTick(bool* resetPositions, DecisionTree dt);
void goalScoredTick(bool* resetPositions, DecisionTree dt);
void gameOverTick();
void Draw_calcScreenOffsets(int imgW, int imgH, int borderW, int borderH, int *offsetX, int *offsetY);

//globally available pointer to game state
GameState* gs = NULL;
TextLabel gInfo = NULL;

//Animation data for Ball, (shouldn't really be here)
int BALL_FRAME_INFO[] = {4};
int* ball_frameRate = NULL;

int CALFNUTS_FRAME_INFO[] = {1, 4};
//int* calfnuts_frameRate = NULL;

//background anchor, again horrible architecture
double ZERO_ANCHOR = 0;
int pitch_offset_x, pitch_offset_y;

//transform function relating geometric location between in world and on screen
Vec3D world2screen(Vec3D v)
{
    static const Vec3D CENTRE_POINT_W = {WORLD_WIDTH / 2.0, WORLD_HEIGHT / 2.0, 0};
    const Vec3D CENTRE_POINT_S = {Viewport_getWidth() / 2.0, Viewport_getHeight() / 2.0, 0};

    Vec3D centreToVec = Vec3D_subtract(v, CENTRE_POINT_W);
    v = Vec3D_scalarMult(centreToVec, (float)Viewport_getRatio());
    v = Vec3D_add(v, CENTRE_POINT_S);

    return v;
}

//transform function relating screen location to world location
Vec3D screen2world(Vec3D v)
{
    static const Vec3D CENTRE_POINT_W = {WORLD_WIDTH / 2.0, WORLD_HEIGHT / 2.0, 0};
    const Vec3D CENTRE_POINT_S = {Viewport_getWidth() / 2.0, Viewport_getHeight() / 2.0, 0};

    Vec3D centreToVec = Vec3D_subtract(v, CENTRE_POINT_S);
    v = Vec3D_scalarMult(centreToVec, (float)(1.0/Viewport_getRatio()));
    v = Vec3D_add(v, CENTRE_POINT_W);

    return v;
}

int main(int argc, char* argv[])
{
    Draw_init(&world2screen);
    Draw_calcScreenOffsets(BG_IMG_W, BG_IMG_H, BG_IMG_BORDER_W, BG_IMG_BORDER_H, &pitch_offset_x, &pitch_offset_y);

    gs = GS_initializeGameState();

#ifdef __ANDROID__
    Rect toucharea = {0,0,Viewport_getWidth(), Viewport_getHeight()};
    TS_init(gs, (int)(toucharea.w / 5.0));
    EH_registerHandler(toucharea, &TS_handleEvent, false, NULL);
#endif

    GS_loadGameObjects(gs);
    loadSprites();

    AI_init();
    GS_setTargetBoundaries(gs);

    //Main loop flag
    bool quit = false;

    //Event place holder
    SDL_Event e;

    //AI decision tree to use
    char pathToDT[512];
#ifdef __ANDROID__
    strcpy(pathToDT, SDL_AndroidGetInternalStoragePath());
    strcat(pathToDT, "/");
    strcat(pathToDT, DT_DEFAULT);
#else
    strcpy(pathToDT, DT_DEFAULT);
#endif
    DecisionTree dt = AI_parseDecisionTree(pathToDT);
    DecisionTree celebrationTree = dt;//AI_parseDecisionTree(DT_DEFAULT);

    //labels
    char p1score[20];
    char p2score[20];
    char p1touches[20];
    char p2touches[20];

    TextLabel p1Name = TL_createTextLabel("McDoodle", 0, 0);
    TL_setFontSize(p1Name, (int)(20 * Draw_getScalingFactor()));
    TL_setFont(p1Name, NULL);

    TextLabel p2Name = TL_createTextLabel("Calfnuts", 0, 0);
    TL_setFontSize(p2Name, (int)(20 * Draw_getScalingFactor()));
    TL_setFont(p2Name, NULL);

    TextLabel p1Score = TL_createTextLabel(NULL, 0, 0);
    TL_setFontSize(p1Score, (int)(20 * Draw_getScalingFactor()));
    TL_setFont(p1Score, NULL);

    TextLabel p2Score = TL_createTextLabel(NULL, 0, 0);
    TL_setFontSize(p2Score, (int)(20 * Draw_getScalingFactor()));
    TL_setFont(p2Score, NULL);

    TextLabel p1Touches = TL_createTextLabel(NULL, 0, 0);
    TL_setFontSize(p1Touches, (int)(20 * Draw_getScalingFactor()));
    TL_setFont(p1Touches, NULL);
    
    TextLabel p2Touches = TL_createTextLabel(NULL, 0, 0);
    TL_setFontSize(p2Touches, (int)(20 * Draw_getScalingFactor()));
    TL_setFont(p2Touches, NULL);

    gInfo = TL_createTextLabel(NULL, 0, 0);
    TL_setFontSize(gInfo, (int)(80 * Draw_getScalingFactor()));
    TL_setFont(gInfo, NULL);

    //Set label positions
    int a = TL_getWidth(p1Name);
    int b = TL_getWidth(p2Name);
    int c = (int)fmax(a, b);
    c = Viewport_getWidth() - c - 25;
    TL_setX(p1Name, c);
    TL_setX(p2Name, c);
    TL_setX(p1Score, c);
    TL_setX(p2Score, c);
    TL_setX(p1Touches, c);
    TL_setX(p2Touches, c);

    a = TL_getHeight(p1Name);
    b = TL_getHeight(p2Name);
    c = (int)fmax(a, b);
    TL_setY(p2Name, (int)((10 + pitch_offset_y) * Draw_getScalingFactor()));
    TL_setY(p2Score, TL_getY(p2Name) + TL_getHeight(p2Name));
    TL_setY(p2Touches, TL_getY(p2Score) + TL_getHeight(p2Name));
    TL_setY(p1Name, (int)(Viewport_getHeight() - ((2 * pitch_offset_y) * Draw_getScalingFactor()) - 3 * c));
    TL_setY(p1Score, TL_getY(p1Name) + TL_getHeight(p1Name));
    TL_setY(p1Touches, TL_getY(p1Score) + TL_getHeight(p1Name));

    bool resetPositions = true;
    int t0, t1;

    //to win score
    int winningScore = 5;

    /*************************************************************
    *********              GAME LOOP           *******************
    *************************************************************/
    Draw_updateView();
    Draw_drawSceneBuffer();

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
            }else
            {
                /** NO NEED FOR MOUSE EVENTS TO EXIST **/
                /*if(isMouseEvent(e))
                {
                    EH_handleEvent(&e);
                }*/
                /***************************************/
                #ifndef __ANDROID__
                if(isJoystickEvent(e))
                {
                    PhysCont_handleEvent(&e);
                }
                #else
                if (isTouchEvent(e))
                {
                    Vec3D touch = {e.tfinger.x * Viewport_getWidth(), e.tfinger.y * Viewport_getHeight(), 0};
                    touch = screen2world(touch);
                    e.tfinger.x = (float)touch.i;
                    e.tfinger.y = (float)touch.j;
                    EH_handleEvent(&e);
                }
                #endif
            }
        }

        //select appropriate behaviour based on play state
        switch(gs->currPlayState)
        {
            case NORMAL_PLAY:
                normalPlayTick(&resetPositions, dt);
                //printf("%s\n", "normal");
                break;
            case GOAL_SCORED:
                goalScoredTick(&resetPositions, celebrationTree);
                //printf("%s\n", "goal");
                break;
            case PENALTY:
                penaltyTick(&resetPositions, dt);
                //printf("%s\n", "penalty");
                break;
            case GAME_OVER:
                gameOverTick();
                //printf("%s\n", "gameover");
                break;
        }

        if(Player_touchingBall(gs->players[0])) TS_resetKickSwitch=true;

        //Step 3: draw result
        Draw_updateView();

        snprintf(p1score, 20, "goals %i", gs->players[0]->score);
        snprintf(p2score, 20, "goals %i", gs->players[1]->score);
        t0 = gs->players[0]->touches < 0 ? 0 : gs->players[0]->touches;
        t1 = gs->players[1]->touches < 0 ? 0 : gs->players[1]->touches;
        snprintf(p1touches, 20, "touches %i", t0);
        snprintf(p2touches, 20, "touches %i", t1);
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
        // content of gInfo label is not constant so draw position should be calculated before render
        TL_setX(gInfo, (Viewport_getWidth() - TL_getWidth(gInfo)) / 2);
        TL_setY(gInfo, (Viewport_getHeight() - TL_getHeight(gInfo)) / 2);
        TL_renderTextLabel(gInfo);

        // render content of renderer to screen and reset
        Draw_drawSceneBuffer();

        //first to 5 wins and triggers quit game
        if(gs->players[0]->score >= winningScore || gs->players[1]->score >= winningScore) quit = true;
    }

    AI_freeDecisionTree(dt);
    /*dt = NULL;
    celebrationTree = NULL;*/

    TL_destroyTextLabel(p1Name);
    TL_destroyTextLabel(p2Name);
    TL_destroyTextLabel(p1Score);
    TL_destroyTextLabel(p2Score);
    TL_destroyTextLabel(p1Touches);
    TL_destroyTextLabel(p2Touches);
    TL_destroyTextLabel(gInfo);
    gInfo = NULL;

    releaseResources();

    //Exit from Game (and activity in Android)
    exit(0);
}

void normalPlayTick(bool* resetPositions, DecisionTree dt)
{
    //Step 0: check for reset or change in positions
    if(*resetPositions){
        resetGamePositions(gs);
        *resetPositions = false;
    }

    //Step 1: get input from user (if in playstate requiring input)
    Input input1 = UI_getUserInput();
    Input input2 = AI_getUserInput(gs, 1, dt);
    //if(PhysCont_PhysicalControllerPresent())drawControlsOnScreen(input1);

    //Step 2: Update physics
    //update positions
    updatePhysics(gs, input1, input2);
    checkRules(gs, resetPositions);
    checkForGoal(gs);
}

void checkForGoal(GameState* gs)
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
}

void penaltyTick(bool* resetPositions, DecisionTree dt)
{
    static bool countDownOn = false;
    static int penaltyTickCounter = 0;

    //Step 0: check for reset or change in positions
    if(*resetPositions){
        takePenaltyPositions(gs);
        *resetPositions = false;
        countDownOn = true;
    }

    if(countDownOn) ++penaltyTickCounter;
    if(penaltyTickCounter == 1) TL_setText(gInfo, "Penalty!");
    if(penaltyTickCounter == 40)
    {
        TL_setText(gInfo, NULL);
        countDownOn = false;
        penaltyTickCounter = 0;
    }

    //Step 1: get input from user (if in playstate requiring input)
    if(!countDownOn)
    {
        Input input1 = UI_getUserInput();
        Input input2 = AI_getUserInput(gs, 1, dt);
        //if(PhysCont_PhysicalControllerPresent())drawControlsOnScreen(input1);

        //Step 2: Update physics
        //update positions
        updatePhysics(gs, input1, input2);
    }
    //do penalty related stuff here
    checkRules(gs, resetPositions);
    checkForGoal(gs);
}

void goalScoredTick(bool* resetPositions, DecisionTree dt)
{
    static int goalStateTickCounter = 0;
    //Input input1 = AI_getUserInput(gs, 0, dt);
    //Input input2 = AI_getUserInput(gs, 1, dt);
    //if(PhysCont_PhysicalControllerPresent())hideControls();

    ++goalStateTickCounter;
    if(goalStateTickCounter == 1)TL_setText(gInfo, "GOAL!");
    if(goalStateTickCounter > 150)
    {
        gs->currPlayState = NORMAL_PLAY;
        Player_resetTouches(gs->players[0]);
        Player_resetTouches(gs->players[1]);
        *resetPositions = true;
        goalStateTickCounter = 0;
        TL_setText(gInfo, NULL);
    }
}

void gameOverTick()
{
    ;
}

/*
void hideControls()
{
    ;
}
*/

void setPenConditions(GameState* gs, int id)
{
    int fouled = id == 0 ? 1 : 0;
    Player_setPenaltyFlag(gs->players[id]);
    Player_clearPenaltyFlag(gs->players[fouled]);
    Player_resetTouches(gs->players[0]);
    Player_resetTouches(gs->players[1]);
    gs->currPlayState = PENALTY;    
}

void checkRules(GameState* gs, bool* resetPositions)
{
    //Check 1: too many touches
    if(Player_getTouches(gs->players[0]) < 0)
    {
        setPenConditions(gs, 0);
        *resetPositions = true;
        return;
    }else if(Player_getTouches(gs->players[1]) < 0)
    {
        setPenConditions(gs, 1);
        *resetPositions = true;
        return;
    }

    //Check 2: touch outside own half when opposition has touches
    if(gs->currPlayState!=PENALTY)
    {
        if(!Player_isInOwnHalf(gs->players[0]) && !Player_canLeaveOwnHalf(gs->players[0]) && Player_touchingBall(gs->players[0]))
        {
            setPenConditions(gs, 0);
            *resetPositions = true;
            return;
        }else if(!Player_isInOwnHalf(gs->players[1]) && !Player_canLeaveOwnHalf(gs->players[1]) && Player_touchingBall(gs->players[1]))
        {
            setPenConditions(gs, 1);
            *resetPositions = true;
            return;
        }
    }

    //Check 3: interfere with penalty
    //it's a penalty
    bool cond1 = (gs->currPlayState == PENALTY);
    //the ball has been struck by penalty taker
    bool cond2 = false;
    if((gs->players[0]->concededPenalty && !Player_hasAllTouches(gs->players[1])) ||
        (gs->players[1]->concededPenalty && !Player_hasAllTouches(gs->players[0]))) cond2=true;
    //ball has not yet stopped
    bool cond3 = !Vec3D_isZero(GO_getVel(gs->ball));
    if(cond1 && cond2 && cond3)
    {
        //player who conceded penalty has interfered
        if(Player_concededPenalty(gs->players[0]) && !Player_hasAllTouches(gs->players[0]))
        {
            setPenConditions(gs, 0);
            *resetPositions = true;
            return;
        }
        if(Player_concededPenalty(gs->players[1]) && !Player_hasAllTouches(gs->players[1]))
        {
            setPenConditions(gs, 1);
            *resetPositions = true;
            return;
        }
    }
    if(cond2 && !cond3)
    {
        gs->currPlayState = NORMAL_PLAY;
        Player_clearPenaltyFlag(gs->players[0]);
        Player_clearPenaltyFlag(gs->players[1]);
    }
}

void resetGamePositions(GameState* gs)
{
    Player_setOffsetPos(gs->players[0], Vec3D_makeVector(POS_PLAYER1_START));
    Player_setOffsetPos(gs->players[1], Vec3D_makeVector(POS_PLAYER2_START));
    Player_resetTouches(gs->players[0]);
    Player_resetTouches(gs->players[1]);
    GO_setOffsetPos(gs->ball, Vec3D_makeVector(POS_BALL_START));
    GO_setVel(gs->ball, VECTOR_ZERO);
    GO_setAcc(gs->ball, VECTOR_ZERO);
}

void takePenaltyPositions(GameState* gs)
{
    if(Player_concededPenalty(gs->players[0]))
    {
        Player_setOffsetPos(gs->players[0], Vec3D_makeVector(POS_PENALTY_S_RECEIVER));
        Player_setOffsetPos(gs->players[1], Vec3D_makeVector(POS_PENALTY_S_TAKER));
    }else
    {
        Player_setOffsetPos(gs->players[0], Vec3D_makeVector(POS_PENALTY_N_TAKER));
        Player_setOffsetPos(gs->players[1], Vec3D_makeVector(POS_PENALTY_N_RECEIVER));
    }
    GO_setOffsetPos(gs->ball, Vec3D_makeVector(POS_BALL_START));
    GO_setVel(gs->ball, VECTOR_ZERO);
    GO_setAcc(gs->ball, VECTOR_ZERO);
}

void updatePhysics(GameState* gs, Input input1, Input input2)
{
    //check and rectify for collisions
    // Player1 and ball
    bool lastTickContact = Player_touchingBall(gs->players[0]);
    collisionWithFreeObject(Player_getGameObject(gs->players[0]), gs->ball, input1, &gs->players[0]->touchingBall);
    if(((lastTickContact == false)&&(Player_touchingBall(gs->players[0]) == true)) || 
            ((lastTickContact == true)&&Vec3D_isZero(Player_getVel(gs->players[0]))&&Vec3D_isZero(GO_getVel(gs->ball))))
    {
        Player_decrementTouches(gs->players[0]);
        Player_resetTouches(gs->players[1]);
    }

    // Player2 and ball
    lastTickContact = Player_touchingBall(gs->players[1]);
    collisionWithFreeObject(Player_getGameObject(gs->players[1]), gs->ball, input2, &gs->players[1]->touchingBall);
    if(((lastTickContact == false)&&(Player_touchingBall(gs->players[1]) == true)) || 
            ((lastTickContact == true)&&Vec3D_isZero(Player_getVel(gs->players[1]))&&Vec3D_isZero(GO_getVel(gs->ball))))
    {
        Player_decrementTouches(gs->players[1]);
        Player_resetTouches(gs->players[0]);
    }

    //update ball physics
    GO_setVel(gs->ball, Vec3D_add(GO_getVel(gs->ball), GO_getAcc(gs->ball)));
    GO_zeroReversedDirections(gs->ball);
    GO_move(gs->ball, GO_getVel(gs->ball));

    //Move Player 1
    Player_setVel(gs->players[0], getVelFromInput(input1));
    Player_setIsStationary(gs->players[0], Vec3D_isZero(Player_getVel(gs->players[0])));
    Player_move(gs->players[0]);

    //Move Player 2
    Player_setVel(gs->players[1], getVelFromInput(input2));
    Player_setIsStationary(gs->players[1], Vec3D_isZero(Player_getVel(gs->players[1])));
    Player_move(gs->players[1]);
    
    collisionWithEnergisedObject(Player_getGameObject(gs->players[0]), Player_getGameObject(gs->players[1]));

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

    // determine if in new positions players can legally enter other half
    if(Rect_containsCircle(gs->players[1]->ownHalf, gs->ball->BCirc)&&!Player_hasTouches(gs->players[1]))
        Player_setCanLeaveHalf(gs->players[0], true);
    if(Rect_containsCircle(gs->players[0]->ownHalf, gs->ball->BCirc)&&!Player_hasTouches(gs->players[0]))
        Player_setCanLeaveHalf(gs->players[1], true);

    // determine if in new positions players can no longer legally enter other half
    if(Player_isInOwnHalf(gs->players[0]) && Player_hasTouches(gs->players[1]))
        Player_setCanLeaveHalf(gs->players[0], false);
    if(Player_isInOwnHalf(gs->players[1]) && Player_hasTouches(gs->players[0]))
        Player_setCanLeaveHalf(gs->players[1], false);

    // update player and ball states
    if(Vec3D_isZero(GO_getVel(gs->ball)))
    {
        Sprite_setFrameRate(ball_frameRate, MAX_LONG_VALUE);
    }else if(Vec3D_getMagnitude(GO_getVel(gs->ball))>CONS_MAX_SPEED)
    {
        Sprite_setFrameRate(ball_frameRate, 5);
    }else
    {
        Sprite_setFrameRate(ball_frameRate, 10);
    }

    double alpha = !Vec3D_isZero(GO_getVel(gs->ball)) ? (Vec3D_getAngle(GO_getVel(gs->ball), VECTOR_N)) : 0 ;
    GO_setRPos(gs->ball, alpha);

    Player_updateState(gs->players[1], gs->ball);
}

//TODO move this to Physics
void collisionWithEnergisedObject(GameObject* go1, GameObject* go2)
{
    if(GO_isInContact(go1, go2))
    {
        Vec3D vec = Vec3D_subtract(GO_getPos(go2), GO_getPos(go1));
        double distanceBetweenCentres = Vec3D_getMagnitude(vec);
        double minAllowableDistance = Circle_getR(&go1->BCirc) + Circle_getR(&go2->BCirc);
        float distToMove = (float)(minAllowableDistance - distanceBetweenCentres);

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
    Vec3D impulse;

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

/*
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
*/
void releaseResources()
{
    EH_destroy();
    GS_destroyGameState(gs);
    Draw_quit();
}

void Draw_calcScreenOffsets(int imgW, int imgH, int borderW, int borderH, int *offsetX, int *offsetY)
{
    // Background image for court
    int Sw = Viewport_getWidth();
    int Sh = Viewport_getHeight();

    int Iw = imgW;
    int Ih = imgH;

    int Cw = Iw - 2 * borderW;  // width of real content in bg img
    int Ch = Ih - 2 * borderH;  // height of real content in bg img

    double SA = (double)Sw/(double)Sh;
    double CA = (double)Cw/(double)Ch;
    double SF = CA <= SA ? (double)Sh / (double)Ch : (double)Sw / (double)Cw;

    int Iw_dash = (int)(SF * Iw);
    int Ih_dash = (int)(SF * Ih);

    int offX = (int)((Iw_dash - Sw) / (2 * SF));
    int offY = (int)((Ih_dash - Sh) / (2 * SF));

    *offsetX = offX;
    *offsetY = offY;

    __android_log_print(ANDROID_LOG_VERBOSE, "TT_LSR", "X offset: %d\n", offX);
    __android_log_print(ANDROID_LOG_VERBOSE, "TT_LSR", "Y offset: %d\n", offY);

}

void loadSprites()
{
//    pitch_offset_x = 50 - offw;
//    pitch_offset_y = 100 - offh;
    Sprite bg =  Sprite_createSprite(PATH_TO_COURT, BG_IMG_W - 2 * pitch_offset_x, BG_IMG_H - 2 * pitch_offset_y, 0, NULL);
    Sprite_setSSoffset(bg, pitch_offset_x, pitch_offset_y);
    Sprite_setSpriteInWorldDims(bg, WORLD_WIDTH, WORLD_HEIGHT);
    //Sprite_posByCentre(bg, true);
    Sprite_setSpriteInWorldPosRef(bg, &ZERO_ANCHOR, &ZERO_ANCHOR, NULL);
    Sprite_setIsFullscreen(bg, true);

    //Vec3D allspriteoffset = {pitch_offset_x, pitch_offset_y, 0};

    //sprite rep of players
    Sprite player_s = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(player_s, SIZE_PLAYER_W, SIZE_PLAYER_H);
    Sprite_posByCentre(player_s, true);
    Sprite_setSpriteInWorldPosRef(player_s, &gs->players[0]->go->pos.i, &gs->players[0]->go->pos.j, NULL);
    //Sprite_setOffset(player_s, allspriteoffset);

    Sprite player_cs = Sprite_createSprite(PATH_TO_CALFNUTS, 69, 108, 2, CALFNUTS_FRAME_INFO);
    Sprite_setSpriteInWorldDims(player_cs, SIZE_PLAYER_W, SIZE_PLAYER_H);
    Sprite_posByCentre(player_cs, true);
    Sprite_setSpriteInWorldPosRef(player_cs, &gs->players[1]->go->pos.i, &gs->players[1]->go->pos.j, NULL);
    //Sprite_setOffset(player_cs, allspriteoffset);
    //calfnuts_frameRate = Sprite_getRateSetAddress(player_cs);
    Sprite_setSpriteRotationRef(player_cs, &gs->players[1]->go->rPos);
    Sprite_setSpriteStateRef(player_cs, (int*)&gs->players[1]->state);
    Sprite_setFR(player_cs, 5);

    //sprite rep of ball
    Sprite ball_s = Sprite_createSprite(PATH_TO_BALL, 35, USE_FULL_IMAGE_HEIGHT, 1, BALL_FRAME_INFO);
    Sprite_setSpriteInWorldDims(ball_s, SIZE_BALL_W, SIZE_BALL_H);
    Sprite_posByCentre(ball_s, true);
    Sprite_setSpriteInWorldPosRef(ball_s, &gs->ball->pos.i, &gs->ball->pos.j, NULL);
    //Sprite_setOffset(ball_s, allspriteoffset);
    ball_frameRate = Sprite_getRateSetAddress(ball_s);
    Sprite_setSpriteRotationRef(ball_s, &gs->ball->rPos);
    
    //sprites for posts of goals
    Sprite post1_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post1_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    //Sprite_setOffset(post1_s, allspriteoffset);
    Sprite_posByCentre(post1_s, true);
    Sprite_setSpriteInWorldPosRef(post1_s, &Goal_getLPost(gs->goals[0])->pos.i, &Goal_getLPost(gs->goals[0])->pos.j, NULL);

    Sprite post2_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post2_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    //Sprite_setOffset(post2_s, allspriteoffset);
    Sprite_posByCentre(post2_s, true);
    Sprite_setSpriteInWorldPosRef(post2_s, &Goal_getRPost(gs->goals[0])->pos.i, &Goal_getRPost(gs->goals[0])->pos.j, NULL);

    Sprite post3_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post3_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    //Sprite_setOffset(post3_s, allspriteoffset);
    Sprite_posByCentre(post3_s, true);
    Sprite_setSpriteInWorldPosRef(post3_s, &Goal_getLPost(gs->goals[1])->pos.i, &Goal_getLPost(gs->goals[1])->pos.j, NULL);

    Sprite post4_s = Sprite_createSprite(PATH_TO_POST_ART, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(post4_s, SIZE_POST_DIAMETER, SIZE_POST_DIAMETER);
    //Sprite_setOffset(post4_s, allspriteoffset);
    Sprite_posByCentre(post4_s, true);
    Sprite_setSpriteInWorldPosRef(post4_s, &Goal_getRPost(gs->goals[1])->pos.i, &Goal_getRPost(gs->goals[1])->pos.j, NULL);

/*
#ifndef __ANDROID__
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
    Sprite_setAlpha(c1Back, 126);
    Sprite_setAlpha(c1Knob, 126);

    Sprite c2Back, c2Knob;
    c2Back = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c2Knob = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c2Back, gs->controllers[1].base.r*2, gs->controllers[1].base.r*2);
    Sprite_setSpriteInWorldDims(c2Knob, gs->controllers[1].knob.r*2, gs->controllers[1].knob.r*2);
    Sprite_posByCentre(c2Back, true);
    Sprite_posByCentre(c2Knob, true);
    Sprite_setSpriteInWorldPosRef(c2Back, &gs->controllers[1].base.x, &gs->controllers[1].base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c2Knob, &gs->controllers[1].knob.x, &gs->controllers[1].knob.y, NULL);
    Sprite_setAlpha(c2Back, 126);
    Sprite_setAlpha(c2Knob, 126);

    Sprite c3Back, c3Knob;
    c3Back = Sprite_createSprite(PATH_TO_RED_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    c3Knob = Sprite_createSprite(PATH_TO_ORANGE_CONTROLLER, USE_FULL_IMAGE_WIDTH, USE_FULL_IMAGE_HEIGHT, 0, NULL);
    Sprite_setSpriteInWorldDims(c3Back, gs->controllers[2].base.r*2, gs->controllers[2].base.r*2);
    Sprite_setSpriteInWorldDims(c3Knob, gs->controllers[2].knob.r*2, gs->controllers[2].knob.r*2);
    Sprite_posByCentre(c3Back, true);
    Sprite_posByCentre(c3Knob, true);
    Sprite_setSpriteInWorldPosRef(c3Back, &gs->controllers[2].base.x, &gs->controllers[2].base.y, NULL);
    Sprite_setSpriteInWorldPosRef(c3Knob, &gs->controllers[2].knob.x, &gs->controllers[2].knob.y, NULL);
    Sprite_setAlpha(c3Back, 126);
    Sprite_setAlpha(c3Knob, 126);
#endif
*/
}
