// utils/sound.h
#pragma once
#include <raylib.h>
#include <string>
#include <triblib/data/data_sound.h>


/* Functions that have everything to do with sounds. In a way, they
 * are pretty much wrappers and helpers for the titular Raylib sound
 * functions. Written in a way to make my job much easier.*/
namespace SoundUtils {
  /* For playing a given sound by it's name. Prints an error when the
   * sound metadata associated with the given name is not found.*/
  void play(std::string sound_name);

  /* Basically plays a sound at a set pitch. An overload of the previous
   * function.*/
  void play(std::string sound_name, const float pitch);

  /* For having full control over the sound's volumn, pitch, and pan.*/
  void playPro(std::string sound_name, const float volume, 
               const float pitch, const float pan);

  /* Stops a specific sound by it's name. Gives a warning if the sound 
   * was not playing at the time of this function being called.*/
  void stop(std::string sound_name);

  /* Pauses all sounds that are currently player. The other function could
   * be called to resume them.*/
  void pause();

  /* Resumes all sounds. Due to raylib not having a function that checks
   * if a sound is paused, I had to go with the nuclear option, and have
   * the function 'resume' every sound in the game for now.*/
  void resume();

  /* This is a helper function written for retrieving metadata associated
   * with a specific sound name. Returns a NULL pointer if no metadata
   * associated with the string given is found.*/
  SoundMetaData *getMetaData(const std::string &sound_name);
}

