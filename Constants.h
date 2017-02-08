#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//all constants used in game defined here
//therefore not adjustable without recompile, needs to be changed. Read from config file?

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for physical dimensions                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640
#define SIZE_CONTROLLER_1 100
#define SIZE_CONTROLLER_2 80
#define SIZE_CONTROLLER_3 80
#define SIZE_CONTROLLER_KNOB1 60
#define SIZE_CONTROLLER_KNOB2 50
#define SIZE_CONTROLLER_KNOB3 50
#define SIZE_PLAYER_H 50
#define SIZE_PLAYER_W 50
#define SIZE_BALL_H 28
#define SIZE_BALL_W 28
#define SIZE_PITCH 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
#define SIZE_BEHIND_GOAL_AREA 60
#define SIZE_POST_DIAMETER 8
#define SIZE_GOAL_WIDTH 100
#define POS_S_GOAL_R (SCREEN_WIDTH-SIZE_GOAL_WIDTH)/2, SCREEN_HEIGHT-SIZE_BEHIND_GOAL_AREA, 0
#define POS_S_GOAL_L (SCREEN_WIDTH+SIZE_GOAL_WIDTH)/2, (SCREEN_HEIGHT-SIZE_BEHIND_GOAL_AREA), 0
#define POS_N_GOAL_L (SCREEN_WIDTH-SIZE_GOAL_WIDTH)/2, SIZE_BEHIND_GOAL_AREA, 0
#define POS_N_GOAL_R (SCREEN_WIDTH+SIZE_GOAL_WIDTH)/2, SIZE_BEHIND_GOAL_AREA, 0

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for paths to art resources                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define PATH_TO_ORANGE_CONTROLLER "/home/adrian/dev/SDL/TT_LSR/Assets/orange.png"
#define PATH_TO_RED_CONTROLLER "/home/adrian/dev/SDL/TT_LSR/Assets/red.png"
#define PATH_TO_BLUE_CONTROLLER "/home/adrian/dev/SDL/TT_LSR/Assets/blue.png"
#define PATH_TO_POST_ART "/home/adrian/dev/SDL/TT_LSR/Assets/blue.png"

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for colours                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define COLOUR_WHITE 0x00, 0x00, 0x00, 0xFF
#define COLOUR_BLACK 0xFF, 0xFF, 0xFF, 0xFF
#define COLOUR_RED 0xFF, 0x00, 0x00, 0xFF
#define COLOUR_GREEN 0x00, 0xFF, 0x00, 0xFF
#define COLOUR_BLUE 0x00, 0x00, 0xFF, 0xFF
#define COLOUR_YELLOW 0xFF, 0xFF, 0x00, 0xFF

//////////////////////////////////////////////////////////////////////////////////////////
/////////        Defines for controller constants                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CONS_CONTROLLER_DEADZONE 1000

//////////////////////////////////////////////////////////////////////////////////////////
/////////        Defines for physics constants                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CONS_MAX_APPLIABLE_IMPULSE 65
#define CONS_MAX_CONTROL_IMPULSE 35
#define CONS_MAX_SPEED 7
#define CONS_BALL_WALL_COR 0.65
#define CONS_BALL_PLAYER_COR 0.1
#define CONS_BALL_COURT_DEACC -0.3
//#define CONS_ACC_GRAVITY -100
//#define CONS_PLAYER_HEIGHT 100
//#define CONS_JUMP_HEIGHT 100
#define CONS_MASS_PLAYER 12
#define CONS_MASS_BALL 3
#define CONS_MAX_TOUCHES 2

//////////////////////////////////////////////////////////////////////////////////////////
/////////        Defines for Player/AI                                //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define PLAYER_HALF_TOP 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2
#define PLAYER_HALF_BOTTOM 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT
#define DT_DEFAULT "Assets/decision_trees/_default_.dt"

#define PI 3.14159265358979323846

#endif // CONSTANTS_H_
