#include "pch.h"
#include "audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// #############################################################################
//                           Statics
// #############################################################################
static ma_engine audioEngine;
static bool audioEngineReady = false;

// #############################################################################
//                           Audio Functions
// #############################################################################
bool audio_init(void)
{
  if (audioEngineReady)
  {
    return true;
  }

  ma_result result = ma_engine_init(NULL, &audioEngine);
  if(result != MA_SUCCESS)
  {
    fprintf(stderr, "Failed to Create Audio Engine: %d\n", result);
    return false;
  }

  audioEngineReady = true;
  return true;
}

void audio_close(void)
{
  if(!audioEngineReady)
    return;

  ma_engine_uninit(&audioEngine);
  audioEngineReady = false;
}

// #############################################################################
//                           Sound Functions
// #############################################################################
#define SOUND_POOL_SIZE 8

struct Sound
{
  char path[512];
  float volume;
  ma_sound pool[SOUND_POOL_SIZE];
  int poolInit[SOUND_POOL_SIZE];
};

Sound* sound_load(const char* path)
{
  if (!audioEngineReady) 
  {
    fprintf(stderr, "Engine not initialised. Call nx_audio_init() first.\n");
    return NULL;
  }

  Sound* s = (Sound*)calloc(1, sizeof(Sound));
  if (!s) return NULL;

  #ifdef _WIN32
      strncpy_s(s->path, sizeof(s->path), path, sizeof(s->path) - 1);
  #else
      strncpy(s->path, path, sizeof(s->path) - 1);
      s->path[sizeof(s->path) - 1] = '\0';
  #endif

  s->volume = 1.0f;

  /* Eagerly init slot 0 so we catch bad paths early */
  ma_result result = ma_sound_init_from_file(
    &audioEngine, path,
    MA_SOUND_FLAG_DECODE,   /* decode to PCM up front for low-latency */
    NULL, NULL,
    &s->pool[0]
  );

  if (result != MA_SUCCESS) 
  {
    fprintf(stderr, "Failed to load sound '%s': %d\n", path, result);
    free(s);
    return NULL;
  }

  ma_sound_set_volume(&s->pool[0], s->volume);
  s->poolInit[0] = 1;
  return s;
}

void sound_free(Sound* sound)
{
  if (!sound) return;

  for (int i = 0; i < SOUND_POOL_SIZE; i++) 
  {
    if (sound->poolInit[i]) 
    {
      ma_sound_uninit(&sound->pool[i]);
    }
  }

  free(sound);
}

void sound_play(Sound* sound)
{
  if (!sound || !audioEngineReady) return;

  /* Find a free slot: prefer one that has finished playing */
  int slot = -1;
  for (int i = 0; i < SOUND_POOL_SIZE; i++) 
  {
    if (!sound->poolInit[i]) 
    {
      slot = i;
      break;
    }
    if (!ma_sound_is_playing(&sound->pool[i])) 
    {
      slot = i;
      break;
    }
  }

  if (slot == -1) 
  {
    /* All slots busy - steal slot 0 (oldest) */
    slot = 0;
    ma_sound_uninit(&sound->pool[0]);
    sound->poolInit[0] = 0;
  }

  /* (Re)initialise slot if needed */
  if (!sound->poolInit[slot]) 
  {
    ma_result result = ma_sound_init_from_file(
      &audioEngine, sound->path,
      MA_SOUND_FLAG_DECODE,
      NULL, NULL,
      &sound->pool[slot]
    );
    if (result != MA_SUCCESS) 
    {
      fprintf(stderr, "Pool reinit failed for '%s': %d\n", sound->path, result);
      return;
    }
    sound->poolInit[slot] = 1;
  }

  /* Rewind so replaying a finished slot works */
  ma_sound_seek_to_pcm_frame(&sound->pool[slot], 0);
  ma_sound_set_volume(&sound->pool[slot], sound->volume);
  ma_sound_set_looping(&sound->pool[slot], MA_FALSE);
  ma_sound_start(&sound->pool[slot]);
}

void sound_set_volume(Sound* sound, float volume)
{
  if (!sound) return;

  if (volume < 0.0f) volume = 0.0f;
  if (volume > 1.0f) volume = 1.0f;

  sound->volume = volume;

  /* Apply to all live slots */
  for (int i = 0; i < SOUND_POOL_SIZE; i++) 
  {
    if (sound->poolInit[i]) 
    {
      ma_sound_set_volume(&sound->pool[i], volume);
    }
  }
}

float sound_get_volume(Sound* sound)
{
  return sound ? sound->volume : 0.0f;
}

// #############################################################################
//                           Music Functions
// #############################################################################
struct Music
{
  ma_sound sound;
  bool initialised;
  float volume;
};

Music* music_load(const char* path)
{
  if (!audioEngineReady) 
  {
    fprintf(stderr, "Engine not initialised. Call nx_audio_init() first.\n");
    return NULL;
  }
 
  Music* m = (Music*)calloc(1, sizeof(Music));
  if (!m) return NULL;

  /* Stream flag: don't decode all at once, suitable for large music files */
  ma_result result = ma_sound_init_from_file(
    &audioEngine, path,
    MA_SOUND_FLAG_STREAM,
    NULL, NULL,
    &m->sound
  );

  if (result != MA_SUCCESS) 
  {
    fprintf(stderr, "Failed to load music '%s': %d\n", path, result);
    free(m);
    return NULL;
  }

  m->volume      = 1.0f;
  m->initialised = true;

  ma_sound_set_looping(&m->sound, MA_TRUE);
  ma_sound_set_volume(&m->sound, m->volume);

  return m;
}

void music_free(Music* music)
{
  if (!music) return;

  if (music->initialised) 
  {
    ma_sound_stop(&music->sound);
    ma_sound_uninit(&music->sound);
  }

  free(music);
};

void music_play(Music* music)
{
  if (!music || !music->initialised) return;

  ma_sound_seek_to_pcm_frame(&music->sound, 0);
  ma_sound_start(&music->sound);
}

void music_stop(Music* music)
{
  if (!music || !music->initialised) return;

  ma_sound_stop(&music->sound);
  ma_sound_seek_to_pcm_frame(&music->sound, 0);
}

void music_pause(Music* music)
{
  if (!music || !music->initialised) return;

  ma_sound_stop(&music->sound);
}

void music_resume(Music* music)
{
  if (!music || !music->initialised) return;

  ma_sound_start(&music->sound);
}

bool music_is_playing(Music* music)
{
  if (!music || !music->initialised) return false;

  return (bool)ma_sound_is_playing(&music->sound);
}

void music_set_volume(Music* music, float volume)
{
  if (!music || !music->initialised) return;

  if (volume < 0.0f) volume = 0.0f;
  if (volume > 1.0f) volume = 1.0f;

  music->volume = volume;
  ma_sound_set_volume(&music->sound, volume);
}

float music_get_volume(Music* music)
{
  return music ? music->volume : 0.0f;
}

