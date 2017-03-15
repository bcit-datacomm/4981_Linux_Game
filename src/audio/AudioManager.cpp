/** AudioManager.cpp
*   This class loads the audio files and plays them.
    Author: DericM
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "AudioManager.h"


AudioManager AudioManager::sInstance;

AudioManager& AudioManager::instance() {
    return sInstance;
}

AudioManager::AudioManager(){
 
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

    loadFiles();
}


AudioManager::~AudioManager(){
    //clear all loaded files
    for(auto const& music : _music) {
        Mix_FreeMusic(music.second);
    }
    for(auto const& chunk : _chunks) {
        Mix_FreeChunk(chunk.second);
    }

    Mix_CloseAudio();
}



//This plays a single background music file from the _music map.
void AudioManager::playMusic(const char * fileName){
        
    Mix_Music * music = _music[fileName];

    printf("%s\n", fileName);

    if(Mix_PlayMusic(music, -1) == -1) {
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
    }
}



//This plays a single sound effect from the _chunks map.
void AudioManager::playEffect(const char * fileName){

    Mix_Chunk * chunk = _chunks[fileName];
    
    printf("%s\n", fileName);

    if (Mix_PlayChannel(-1, chunk, 0) == -1 ){
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
    }
}




void AudioManager::loadFiles(){
    
    ////MUSIC
    loadMusic(MUS_DARKNUBULA);
    loadMusic(MUS_TESTMENU01);


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

}




//This loads a single background music file into the _music map.
void AudioManager::loadMusic(const char * fileName){

    Mix_Music * music = NULL;
    music = Mix_LoadMUS(fileName);
    if (music == NULL ) {
        printf( "Failed to load music: %s\n SDL_mixer Error: %s\n", 
            fileName, Mix_GetError() );
    }

    _music[fileName] = music;
}


//This loads a single sound effect file into the _chunks map.
void AudioManager::loadEffect(const char * fileName){

    Mix_Chunk * sound = NULL;
    sound = Mix_LoadWAV(fileName);
    if (sound == NULL ) {
        printf( "Failed to load sound: %s\n SDL_mixer Error: %s\n", 
            fileName, Mix_GetError() );
    }

    _chunks[fileName] = sound;
}
