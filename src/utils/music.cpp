// utils/music.cpp
#include <raylib.h>
#include "globals.h"
#include "utils_music.h"
#include <plog/Log.h>


void MusicUtils::play(const int music_id) {
  PLOGI << "Attempting to play music associated with ID: " << music_id;

  const int count = audio::bgm_metadata.size(); 
  if (music_id >= count) {
    PLOGE << "Invalid music id!";
    return;
  } 

  if (audio::stream_data == NULL) {
    audio::stream_data = &audio::bgm_metadata[music_id];
  }
  else if (audio::stream_data->id != music_id) {
    float time_played = GetMusicTimePlayed(audio::music_stream);
    audio::stream_data->prev_time = time_played;
    UnloadMusicStream(audio::music_stream);

    audio::stream_data = &audio::bgm_metadata[music_id]; 
  }

  auto meta_data = audio::stream_data;
  PLOGI << "Validating music path: " << meta_data->path;

  if (DirectoryExists(meta_data->path.c_str()) == false) {
    PLOGE << "Invalid path!";
  }
  else {
    audio::music_stream = LoadMusicStream(meta_data->path.c_str());
    audio::music_stream.looping = meta_data->looping;

    PlayMusicStream(audio::music_stream);
  } 
}
