/*------------------------------------------------------------------------------
* Header: AudioManage.h
*
* Functions:
*           static AudioManager& instance();
*           void playMusic(const char *fileName);
*           void playEffect(const char *fileName);
*           void fadeMusicOut(int ms);
*           void playMenuMusic(const char *fileName1, const char *fileName2);
*           void loadFiles();
*           void loadMusic(const char *fileName);
*           void loadEffect(const char *fileName);
*
* Date:
*
* Revisions:
* Edited By :
*   Yiaoping Shu
*   Alex Zielinski: added a few more sounds (April 4, 2017)
*   Alex Zielinski: added a music fade out wrapper function (April 5, 2017)
*   Alex Zielinski: added new audio effects (April 6, 2017)
*
* Designer(s): DericM, Alex Zielinski
*
* Author(s): DericM, Alex Zielinski
*
* Notes:
*
------------------------------------------------------------------------------*/

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

// audio fade out in ms
#define MUSICFADE 1000

#define AUDIO_PATH "assets/sounds/"

// music
#define MUS_MENUBKG01   AUDIO_PATH "music_lunar_moon.ogg"
#define MUS_MENUBKG02   AUDIO_PATH "music_ember_fall.ogg"
#define MUS_GAMEBKG     AUDIO_PATH "music_fog_of_war.ogg"
#define MUS_DARKNUBULA  AUDIO_PATH "music_dark_nebula.ogg"

// menu interactions
#define MENU_CLICK01    AUDIO_PATH "main_menu_button_click_effect.ogg"
#define MENU_CLICK02    AUDIO_PATH "game_menu_HUD_click_effect.ogg"

// marine interaction sounds
#define EFX_PDROP01     AUDIO_PATH "drop_effect_1.ogg"
#define EFX_PDROP02     AUDIO_PATH "drop_effect_2.ogg"
#define EFX_PPICK01     AUDIO_PATH "pick_up_effect_1.ogg"
#define EFX_PPICK02     AUDIO_PATH "pick_up_effect_2.ogg"
#define EFX_MEDKIT      AUDIO_PATH "medkit_effect"

// weapon sounds
#define EFX_WLPISTOL    AUDIO_PATH "weapon_pistol_fire_effect.ogg"
#define EFX_WLRIFLE     AUDIO_PATH "weapon_rifle_fire_effect.ogg"
#define EFX_WSHOTGUN    AUDIO_PATH "weapon_rifle_fire_effect.ogg"
#define EFX_WTURRET01   AUDIO_PATH "weapon_turret_fire_effect.ogg"

// weapon reload sounds
#define EFX_WRELOAD01   AUDIO_PATH "weapon_reload_effect.ogg"
#define EFX_WRELOAD02   AUDIO_PATH "weapon_shotgun_reload_effect.ogg"

// baracade sounds
#define EFX_BINSTALL    AUDIO_PATH "baracade_effect.ogg"

// zombie sounds
#define EFX_ZATTACK01   AUDIO_PATH "zombie_hit_1.ogg"
#define EFX_ZATTACK02   AUDIO_PATH "zombie_hit_2.ogg"
#define EFX_ZGROAN01    AUDIO_PATH "zombie_groan_effect_1"


//maps for storing loaded files.
typedef std::map<std::string, Mix_Music*> musicMap;
typedef std::map<std::string, Mix_Chunk*> chunkMap;


class AudioManager {
public:

    static AudioManager& instance();

    void playMusic(const char *fileName);
    void playEffect(const char *fileName);
    void fadeMusicOut(int ms);
    void playMenuMusic(const char *fileName1, const char *fileName2);

private:
    static AudioManager sInstance;

    musicMap  mus;
    chunkMap  chun;

    AudioManager();
    ~AudioManager();

    void loadFiles();
    void loadMusic(const char *fileName);
    void loadEffect(const char *fileName);
};

#endif
