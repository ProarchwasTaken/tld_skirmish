// utils_music.h
#pragma once


/* Functions that have everything to do with music like SoundUtils.
 * Also there to make my job much easier.*/
namespace MusicUtils {
  bool updateStreamData(const int music_id);

  void play(const int music_id);
  void resume(const int music_id);

  void fadeout(const float target_volume, const float seconds);
  void interpolateVolume();
}
