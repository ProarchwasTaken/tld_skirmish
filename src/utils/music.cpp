// utils/music.cpp
#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "data/data_music.h"
#include "utils_music.h"
#include <plog/Log.h>


bool MusicUtils::updateStreamData(const int music_id) {
  if (audio::stream_data == NULL) {
    audio::stream_data = &audio::bgm_metadata[music_id];
    return true;
  }
  else if (audio::stream_data->id != music_id) {
    float time_played = GetMusicTimePlayed(audio::music_stream);
    float song_length = GetMusicTimeLength(audio::music_stream);
    float progress = (time_played / song_length) * 100;

    PLOGD << "Progress on previous song: " << progress;
    audio::stream_data->prev_time = progress;
    audio::stream_data = &audio::bgm_metadata[music_id]; 

    UnloadMusicStream(audio::music_stream);
    return true;
  }
  else {
    return false;
  }
}

void MusicUtils::play(const int music_id) {
  PLOGI << "Attempting to play music associated with ID: " << music_id;
  const int count = audio::bgm_metadata.size(); 
  if (music_id >= count) {
    PLOGE << "Invalid music id!";
    return;
  } 

  updateStreamData(music_id);

  auto meta_data = audio::stream_data;
  PLOGI << "Validating music path: " << meta_data->path;

  if (FileExists(meta_data->path.c_str()) == false) {
    PLOGE << "Invalid path!";
  }
  else {
    audio::music_stream = LoadMusicStream(meta_data->path.c_str());
    audio::music_stream.looping = meta_data->looping;

    SetMusicVolume(audio::music_stream, settings::bgm_volume);
    PlayMusicStream(audio::music_stream);
  } 
}

void MusicUtils::resume(const int music_id) {
  PLOGI << "Attempting to resume music associated with ID: " << music_id;

  const int count = audio::bgm_metadata.size(); 
  if (music_id >= count) {
    PLOGE << "Invalid music id!";
    return;
  } 

  if (updateStreamData(music_id) == false) {
    PLOGW << "Music is already playing!";
    return;
  }

  auto meta_data = audio::stream_data;
  PLOGI << "Validating music path: " << meta_data->path;

  if (FileExists(meta_data->path.c_str())) {
    audio::music_stream = LoadMusicStream(meta_data->path.c_str());
    audio::music_stream.looping = meta_data->looping;

    SeekMusicStream(audio::music_stream, meta_data->prev_time);
    SetMusicVolume(audio::music_stream, settings::bgm_volume);
    PlayMusicStream(audio::music_stream);
  }
}

void MusicUtils::fadeout(const float target_volume, const float seconds) {
  assert(target_volume < 1.0);
  assert(seconds >= 0);  

  audio::bgm_vol_lerp = {true, seconds, target_volume};
}

void MusicUtils::interpolateVolume() {
  VolumeLerpData *lerp_data = &audio::bgm_vol_lerp;
  if (lerp_data->active == false) {
    return;
  }

  float *percentage = &lerp_data->percentage;
  float lerp_time = lerp_data->lerp_time;
  float end_volume = lerp_data->end_volume;
  *percentage -= GetFrameTime() / lerp_time;
  *percentage = Clamp(*percentage, 0.0, 1.0);

  float volume = Lerp(end_volume, 1.0, *percentage);
  SetMusicVolume(audio::music_stream, volume * settings::bgm_volume);

  if (*percentage == 0.0) {
    StopMusicStream(audio::music_stream);
    lerp_data->active = false;
  }
}
