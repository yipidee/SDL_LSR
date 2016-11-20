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
#define SIZE_PLAYER_H 70
#define SIZE_PLAYER_W 70
#define SIZE_BALL_H 40
#define SIZE_BALL_W 40

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for paths to art resources                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define PATH_TO_ORANGE_CONTROLLER "/home/adrian/dev/SDL/TT_LSR/Assets/orange.png"
#define PATH_TO_RED_CONTROLLER "/home/adrian/dev/SDL/TT_LSR/Assets/red.png"

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
/////////        Defines for physics constants                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CONS_MAX_APPLIED_IMPULSE 7
#define CONS_MAX_SPEED 10
#define CONS_BALL_WALL_COR 0.8
#define CONS_BALL_COURT_COR 0.5
#define CONS_BALL_PLAYER_COR 0.9
#define CONS_BALL_COURT_DEACC -0.3
#define CONS_BALL_AIR_DEACC -100
#define CONS_PLAYER_WALL_COR 100
#define CONS_PLAYER_PLAYER_COR 0.3
#define CONS_PLAYER_COURT_COR 100
#define CONS_PLAYER_COURT_DEACC -100
#define CONS_ACC_GRAVITY -100
#define CONS_PLAYER_HEIGHT 100
#define CONS_JUMP_HEIGHT 100
#define CONS_MASS_PLAYER 30

#define PI 3.14159265358979323846

#endif // CONSTANTS_H_
