#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//all constants used in game defined here
//therefore not adjustable without recompile, needs to be changed. Read from config file?

//////////////////////////////////////////////////////////////////////////////////////////
/////////   Defines for physical dimensions                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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
/////////   Defines for physics constants                                  //////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CONS_BALL_WALL_COR 100
#define CONS_BALL_COURT_COR 100
#define CONS_BALL_COURT_DEACC -100
#define CONS_BALL_AIR_DEACC -100
#define CONS_PLAYER_WALL_COR 100
#define CONS_PLAYER_COURT_COR 100
#define CONS_PLAYER_COURT_DEACC -100
#define CONS_ACC_GRAVITY -100
#define CONS_PLAYER_HEIGHT 100
#define CONS_JUMP_HEIGHT 100

#endif // CONSTANTS_H_
