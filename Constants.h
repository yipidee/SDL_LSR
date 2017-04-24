#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//all constants used in game defined here
//therefore not adjustable without recompile, needs to be changed. Read from config file?

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for physical dimensions                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////

// The below dimensions are in world
#define WORLD_WIDTH 427
#define WORLD_HEIGHT 640
/*
#define SIZE_CONTROLLER_1 90
#define SIZE_CONTROLLER_2 70
#define SIZE_CONTROLLER_3 70
#define SIZE_CONTROLLER_KNOB1 55
#define SIZE_CONTROLLER_KNOB2 35
#define SIZE_CONTROLLER_KNOB3 35
*/
#define SIZE_PLAYER_H 40
#define SIZE_PLAYER_W 40
#define SIZE_BALL_H 20
#define SIZE_BALL_W 20
#define SIZE_PITCH 0, 0, WORLD_WIDTH, WORLD_HEIGHT
#define SIZE_BEHIND_GOAL_AREA 50
#define SIZE_POST_DIAMETER 8
#define SIZE_GOAL_WIDTH 75

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for positions of objects                                    //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define POS_S_GOAL_R (WORLD_WIDTH-SIZE_GOAL_WIDTH)/2, WORLD_HEIGHT-SIZE_BEHIND_GOAL_AREA, 0
#define POS_S_GOAL_L (WORLD_WIDTH+SIZE_GOAL_WIDTH)/2, (WORLD_HEIGHT-SIZE_BEHIND_GOAL_AREA), 0
#define POS_N_GOAL_L (WORLD_WIDTH-SIZE_GOAL_WIDTH)/2, SIZE_BEHIND_GOAL_AREA, 0
#define POS_N_GOAL_R (WORLD_WIDTH+SIZE_GOAL_WIDTH)/2, SIZE_BEHIND_GOAL_AREA, 0
#define POS_PLAYER1_START WORLD_WIDTH / 2, WORLD_HEIGHT - 150, 0
#define POS_PLAYER2_START WORLD_WIDTH / 2, 150, 0
#define POS_BALL_START WORLD_WIDTH / 2, WORLD_HEIGHT / 2, 0
#define POS_PENALTY_N_TAKER POS_PLAYER1_START
#define POS_PENALTY_N_RECEIVER 50, 50, 0
#define POS_PENALTY_S_TAKER POS_PLAYER2_START
#define POS_PENALTY_S_RECEIVER 50, WORLD_HEIGHT - 50, 0

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for paths to art resources                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define PATH_TO_ORANGE_CONTROLLER "assets/imgs/orange.png"
#define PATH_TO_RED_CONTROLLER "assets/imgs/red.png"
#define PATH_TO_BLUE_CONTROLLER "assets/imgs/blue.png"
#define PATH_TO_BALL "assets/imgs/ball.png"
#define PATH_TO_COURT "assets/imgs/court.png"
#define PATH_TO_CALFNUTS "assets/imgs/calfnuts_sprite.png"
#define PATH_TO_POST_ART PATH_TO_RED_CONTROLLER

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for colours                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define COLOUR_WHITE 0x00, 0x00, 0x00, 0xFF
#define COLOUR_BLACK 0xFF, 0xFF, 0xFF, 0xFF
#define COLOUR_RED 0xFF, 0x00, 0x00, 0xFF
#define COLOUR_GREEN 0x00, 0xFF, 0x00, 0xFF
#define COLOUR_BLUE 0x00, 0x00, 0xFF, 0xFF
#define COLOUR_YELLOW 0xFF, 0xFF, 0x00, 0xFF

#define RGB_BLACK {0, 0, 0}
#define RGB_YELLOW {255, 255, 0}

//////////////////////////////////////////////////////////////////////////////////////////
/////////                  Defines for colours                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define TEXT_LABEL_MAX_LENGTH 128
#define DEFAULT_TEXT_SIZE 12
#define DEFAULT_FONT "assets/Fonts/Yantiq.ttf"
//////////////////////////////////////////////////////////////////////////////////////////
/////////        Defines for controller constants                               //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CONS_CONTROLLER_DEADZONE 1000

//////////////////////////////////////////////////////////////////////////////////////////
/////////        Defines for physics constants                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CONS_MAX_APPLIABLE_IMPULSE 55
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
#define PLAYER_HALF_TOP 0, 0, WORLD_WIDTH, WORLD_HEIGHT / 2
#define PLAYER_HALF_BOTTOM 0, WORLD_HEIGHT / 2, WORLD_WIDTH, WORLD_HEIGHT
#define DT_DEFAULT "assets/decision_trees/_default_.dt"

#define PI 3.14159265358979323846
#define MAX_LONG_VALUE 65535

#endif // CONSTANTS_H_
