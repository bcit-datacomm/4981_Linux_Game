#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <map>


#define AUDIO_PATH "assets/sounds/"

//music
#define MUS_DARKNUBULA  AUDIO_PATH "Dark Nebula.ogg"
#define MUS_TESTMENU01  AUDIO_PATH "~testsound_menuv01.ogg"

//marine sounds
#define EFX_PDROP01     AUDIO_PATH "Drop 1.ogg"
#define EFX_PDROP02     AUDIO_PATH "Drop 2.ogg"
#define EFX_PPICK01     AUDIO_PATH "Pick Up 1.ogg"
#define EFX_PPICK02     AUDIO_PATH "Pick Up 2.ogg"

#define EFX_PGRUNT01    AUDIO_PATH "~testsound_playergrunt1.ogg"
#define EFX_PDEATH01    AUDIO_PATH "~testsound_death1.ogg"

//weapon sounds
#define EFX_WLPISTOL    AUDIO_PATH "Laser Pistol 1.ogg"
#define EFX_WLRIFLE     AUDIO_PATH "Laser Rifle 1.ogg"

#define EFX_WRELOAD01   AUDIO_PATH "Reload 1.ogg"
#define EFX_WTURRET01   AUDIO_PATH "Turret 1.ogg"

//zombie sounds
#define EFX_ZGROAN01    AUDIO_PATH "Zombie Groan 1.ogg"
#define EFX_ZGRUNT01    AUDIO_PATH "~testsound_zombiegrunt1.ogg"


//maps for storing loaded files.
typedef std::map<std::string, Mix_Music*> musicMap;
typedef std::map<std::string, Mix_Chunk*> chunkMap;


class AudioManager {
public:

    static AudioManager& instance();

    void playMusic(const char * fileName);
    void playEffect(const char * fileName);

private:
    static AudioManager sInstance;

    musicMap  _music;
    chunkMap  _chunks;

    AudioManager();
    ~AudioManager();

    void loadFiles();
    void loadMusic(const char * fileName);
    void loadEffect(const char * fileName);
};

#endif
