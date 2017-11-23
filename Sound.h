#ifndef SOUND_H_
#define SOUND_H_

#include "SDL_Helper.h"

// cast SDL structs to simpler names
typedef Mix_Chunk* SFX;
typedef Mix_Music* BGM;

// initialise the Sound system
#define Sound_init() Mix_OpenSound(44100, MIX_DEFAULT_FORMAT, 2, 2048)
#define Sound_quit() Mix_Quit()

// load resources
#define Sound_loadSFX(pathToRes) Mix_LoadWAV((pathToRes)) 
#define Sound_LoadMusic(pathToRes) Mix_LoadMUS((pathToRes))

// play resource
#define Sound_playSFX(sfx) Mix_PlayChannel(-1, (sfx), 0)
#define Sound_playMusic(music) Mix_PlayMusic((music), -1))
#define Sound_pauseMusic() if(Mix_PlayingMusic()==1)Mix_PauseMusic()
#define Sound_resumeMusic() if(Mix_PausedMusic()==1)Mix_ResumeMusic()
#define Sound_stopMusic() if(Mix_PlayingMusic()==1)Mix_HaltMusic()
//void Sound_rewindMusic();

// free resources
#define Sound_freeSFX(sfx) Mix_FreeChunk((sfx))
#define Sound_freeMusic(music) Mix_FreeMusic((music))

#endif  // SOUND_H_