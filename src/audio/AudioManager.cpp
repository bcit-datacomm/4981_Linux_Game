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
* Designer:
*
* Author: DericM
*
* Notes:
* This class loads the audio files and plays them.
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>

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
*   Programmer: DericM, Alex Zielinski
*
*   Modified: Alex Zielinski ~ April 4
*               Added few more audio files to load
*
*   Description:
*       Loads all audio asset files
*/
void AudioManager::loadFiles(){
#ifndef SERVER
    ////MUSIC
    loadMusic(MUS_DARKNUBULA);
    loadMusic(MUS_TESTMENU01);
    loadMusic(MUS_MENUBKG_1);
    loadMusic(MUS_MENUBKG_2);


    ////CHUNKS
    //marine
    loadEffect(EFX_PDROP01);
    loadEffect(EFX_PDROP02);
    loadEffect(EFX_PPICK01);
    loadEffect(EFX_PPICK02);

    loadEffect(EFX_PGRUNT01);
    loadEffect(EFX_PDEATH01);

    //weapon
    loadEffect(EFX_WLPISTOL);
    loadEffect(EFX_WLRIFLE);

    loadEffect(EFX_WRELOAD01);
    loadEffect(EFX_WTURRET01);

    //zombie
    loadEffect(EFX_ZGROAN01);
    loadEffect(EFX_ZGRUNT01);

    //baracade
    loadEffect(EFX_BINSTALL);
#endif
}



/**
*   Function: loadMusic(const char *fileName)
*               const char *fileName: file path of audio file to load
*   Date:
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
