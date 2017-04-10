/*------------------------------------------------------------------------------
* Source: AudioManage.cpp
*
* Functions:
*    AudioManager& AudioManager::instance()
*    AudioManager::AudioManager()
*    void AudioManager::playMusic(const char *fileName)
*    void AudioManager::playEffect(const char *fileName)
*    void AudioManager::loadFiles()
*    void AudioManager::loadMusic(const char *fileName)
*    void AudioManager::loadEffect(const char *fileName)
*
* Date:
*
* Revisions:
* Edited By : Yiaoping Shu- Style guide
* Edited By : Alex Zielinski - Fixed seg fault caused by desctructor
*                            - Added comments where appropriate
*                            -
*
* Designer(s): DericM, Alex Zielinski
*
* Author(s): DericM, Alex Zielinski
*
* Notes:
* This class loads the audio files and plays them.
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <random>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "AudioManager.h"
#include "../log/log.h"


AudioManager AudioManager::sInstance;

AudioManager& AudioManager::instance() {
    return sInstance;
}

/**
*   Function: ~AudioManager()
*
*   Date:
*
*   Programmer: DericM
*
*   Description:
*       Constructor of AudioManager
*/
AudioManager::AudioManager(){
#ifndef SERVER
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        logv("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    loadFiles();
#endif
}


/**
*   Function: ~AudioManager()
*
*   Date:
*
*   Programmer: DericM, Alex Zielinski
*
*   Modified: Alex Zielinski ~ March 30
*               Fix seg fault
*
*   Description:
*       Desctructor of AudioManager. Frees memory and quits SLD mixer
*/
AudioManager::~AudioManager(){
#ifndef SERVER
    //clear all loaded files
    for(auto const& music : mus) {
        Mix_FreeMusic(music.second);
    }
    for(auto const& chunk : chun) {
        Mix_FreeChunk(chunk.second);
    }

    // close audio and quit SDL audio mixer
    Mix_CloseAudio();
    Mix_Quit();
#endif
}


/**
*   Function: playMusic(const char *fileName)
*               const char *fileName: file path of audio file to play
*   Date:
*
*   Returns: void
*
*   Programmer: DericM
*
*   Description:
*       Plays a single background music file from the _music map.
*/
void AudioManager::playMusic(const char *fileName){
#ifndef SERVER
    Mix_Music *music = mus[fileName];

    logv("%s\n", fileName);

    if(Mix_PlayMusic(music, -1) == -1) {
        logv("Mix_PlayMusic: %s\n", Mix_GetError());
    }
#endif
}


/**
*   Function: playEffect(const char *fileName)
*               const char *fileName: file path of audio file to play
*   Date:
*
*   Returns: void
*
*   Programmer: DericM
*
*   Description:
*       Plays a single sound effect from the _chunks map.
*/
void AudioManager::playEffect(const char *fileName){
#ifndef SERVER
    Mix_Chunk *chunk = chun[fileName];

    logv("%s\n", fileName);

    if (Mix_PlayChannel(-1, chunk, 0) == -1 ){
        logv("Mix_PlayChannel: %s\n", Mix_GetError());
    }
#endif
}


/**
*   Function:   loadFiles()
*
*   Date:
*
*   Returns: void
*
*   Programmer: DericM, Alex Zielinski
*
*   Modified: Alex Zielinski ~ April 4
*               Added few more audio files to load
*             Alex Zielinski ~ April 6
*               Added new audio files
*
*   Description:
*       Loads all audio asset files
*/
void AudioManager::loadFiles(){
#ifndef SERVER
    /* MUSIC */
    loadMusic(MUS_MENUBKG01);
    loadMusic(MUS_MENUBKG02);
    loadMusic(MUS_GAMEBKG);
    loadMusic(MUS_DARKNUBULA);

    /* CHUNKS */
    // menu interactions
    loadEffect(MENU_CLICK01);
    loadEffect(MENU_CLICK02);

    // marine
    loadEffect(EFX_PDROP01);
    loadEffect(EFX_PDROP02);
    loadEffect(EFX_PPICK01);
    loadEffect(EFX_PPICK02);
    loadEffect(EFX_MEDKIT);
    loadEffect(EFX_PURCHASE);

    // weapon
    loadEffect(EFX_WLPISTOL);
    loadEffect(EFX_WLRIFLE);
    loadEffect(EFX_WSHOTGUN);
    loadEffect(EFX_WTURRET);
    loadEffect(EFX_WDEAGLE);
    loadEffect(EFX_WREPEATER);
    loadEffect(EFX_WRAIL01);
    loadEffect(EFX_WRAIL02);
    loadEffect(EFX_WTYPE75K);

    // weapon reload
    loadEffect(EFX_WRELOAD01);
    loadEffect(EFX_WRELOAD02);
    loadEffect(EFX_WRELOAD03);
	loadEffect(EFX_WRELOAD04);
	loadEffect(EFX_WRELOAD05);
	loadEffect(EFX_WRELOAD06);
	loadEffect(EFX_WRELOAD07);
	loadEffect(EFX_WRELOAD08);


    // baracade
    loadEffect(EFX_BINSTALL);

    // zombie
    loadEffect(EFX_ZATTACK01);
    loadEffect(EFX_ZATTACK02);
    loadEffect(EFX_ZGROAN01);
#endif
}


/**
*   Function: loadMusic(const char *fileName)
*               const char *fileName: file path of audio file to load
*   Date:
*
*   Returns: void
*
*   Programmer: DericM
*
*   Description:
*       Loads a single background music file into the _music map.
*/
void AudioManager::loadMusic(const char *fileName){
#ifndef SERVER
    Mix_Music *music = nullptr;
    music = Mix_LoadMUS(fileName);
    if (music == nullptr) {
        logv("Failed to load music: %s\n SDL_mixer Error: %s\n",
            fileName, Mix_GetError());
    }

    mus[fileName] = music;
#endif
}


/**
*   Function: loadEffect(const char *fileName)
*               const char *fileName: file path of audio file to load
*   Date:
*
*   Returns: void
*
*   Programmer: DericM
*
*   Description:
*       Loads a single sound effect file into the _chunks map.
*/
void AudioManager::loadEffect(const char *fileName){
#ifndef SERVER
    Mix_Chunk *sound = nullptr;
    sound = Mix_LoadWAV(fileName);
    if (sound == nullptr) {
        logv("Failed to load sound: %s\n SDL_mixer Error: %s\n",
            fileName, Mix_GetError());
    }

    chun[fileName] = sound;
#endif
}


/**
*   Function: fadeMusicOut(int ms)
*               int ms: amount of milliseconds to fade out music
*
*   Returns: void
*
*   Date: April 5, 2017
*
*   Programmer: Alex Zielinski
*
*   Description:
*       Wrapper fucntion to fade music out. The amount of time is takes to fade
*       the music out is specified by the paramater int ms (in milliseconds)
*/
void AudioManager::fadeMusicOut(int ms){
#ifndef SERVER
    Mix_FadeOutMusic(ms);
#endif
}


/**
*   Function: playMenuMusic(const char *fileName1, const char *fileName2)
*               const char *fileName1: first menu music audio file
*               const char *fileName2: second menu music audio file
*
*   Returns: void
*
*   Date: April 5, 2017
*
*   Programmer: Alex Zielinski
*
*   Description:
*       Randomly selects one of the 2 menu background music audio files
*       to play when the menu loads
*/
void AudioManager::playMenuMusic(const char *fileName1, const char *fileName2){
#ifndef SERVER
    srand (time(nullptr)); // random seed generator
    int random = rand() % 2; // randomly select 1 or 2

    // check what number was selected
    switch(random)
    {
        case 0:
            playMusic(fileName1);
            break;
        case 1:
            playMusic(fileName2);
            break;
    }
#endif
}
