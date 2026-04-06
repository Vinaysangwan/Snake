#pragma once
#include "assets.h"

// #############################################################################
//                           Audio
// #############################################################################
bool audio_init(void);
void audio_close(void);

// #############################################################################
//                           Sound Functions
// #############################################################################
typedef struct Sound Sound;

Sound* sound_load(const char* path);
void sound_free(Sound* sound);

void sound_play(Sound* sound);

void sound_set_volume(Sound* sound, float volume);
float sound_get_volume(Sound* sound);

// #############################################################################
//                           music Functions
// #############################################################################
typedef struct Music Music;

Music* music_load(const char* path);
void music_free(Music* music);

void music_play(Music* music);
void music_stop(Music* music);
void music_pause(Music* music);
void music_resume(Music* music);
bool music_is_playing(Music* music);

void music_set_volume(Music* music, float volume);
float music_get_volume(Music* music);


