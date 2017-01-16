#include <stdlib.h>
#include "GameState.h"
#include "Constants.h"
#include "Draw.h"

//create destroy functions for gamestate
GameState* GS_initializeGameState()
{
    GameState* gs = malloc(sizeof(GameState));
    gs->ball = NULL;
    gs->players[0] = NULL;
    gs->players[1] = NULL;
    gs->pitch = Rect_create(SIZE_PITCH);
    return gs;
}

void GS_destroyGameState(GameState* gs)
{
    if(gs)
    {
        int i;
        for(i=0;i<2;++i)Player_destroy(gs->players[i]);
        GO_destroyAllGameObjects();
        free(gs);
    }
}

//Game state functions
void GS_loadGameObjects(GameState* gs)
{
    //logical game object, players
    GameObject* player = GO_createGameObject();
    Vec3D playerStartPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150, 0};
    GO_setPos(player, playerStartPosition);
    Circle playerBounds = {0,0,SIZE_PLAYER_H / 2};
    GO_setBCirc(player, playerBounds);
    GO_setMass(player, CONS_MASS_PLAYER);

    Player mccoy = Player_create(player);
    gs->players[0] = mccoy;
    //memcpy(&gs->players[0], mccoy, sizeof(struct _Player));
    //Player_destroy(mccoy);

    GameObject* player_c = GO_createGameObject();
    playerStartPosition.j = 150;
    GO_setPos(player_c, playerStartPosition);
    GO_setBCirc(player_c, playerBounds);
    GO_setMass(player_c, CONS_MASS_PLAYER);

    Player calfnuts = Player_create(player_c);
    gs->players[1] = calfnuts;

    //logical game object, ball
    GameObject* ball = GO_createGameObject();
    Vec3D ballStartPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0};
    GO_setPos(ball, ballStartPosition);
    Circle ballBounds = {0,0,SIZE_BALL_H / 2};
    GO_setBCirc(ball, ballBounds);
    GO_setMass(ball, CONS_MASS_BALL);
    gs->ball = ball;
}

void GS_registerTouchHandlers(GameState* gs)
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

    gs->controllers[0] = controller1;
    gs->controllers[1] = controller2;
    gs->controllers[2] = controller3;

    //register controllers with touch handler
    EH_registerHandler(gs->controllers[0].touchableArea, gs->controllers[0].evHan, true, &gs->controllers[0]);
    EH_registerHandler(gs->controllers[1].touchableArea, gs->controllers[1].evHan, true, &gs->controllers[1]);
    EH_registerHandler(gs->controllers[2].touchableArea, gs->controllers[2].evHan, true, &gs->controllers[2]);

}

