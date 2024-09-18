// utils/sequence.h
#pragma once
#include <array>
#include <cstddef>
#include "globals.h"


/* A template class rather than a series of functions. Could be seen as
 * a more advanced version the the Animation utilities, but for pretty
 * much every other type other than actor. Very versatile and powerful,
 * so it should used with caution.*/
template<typename Type, std::size_t Size>
class Sequence {
public:
  Sequence(std::array<Type, Size> order) {
    this->order = order;
    iterator = this->order.begin();

    timestamp = CURRENT_TIME;
  }

  void newSequence(std::array<Type, Size> new_order) {
    order = new_order;
    iterator = order.begin();
    end_of_sequence = false;

    timestamp = CURRENT_TIME;
  }

  void play(const float frame_time, const bool looping) { 
    if (end_of_sequence) {
      return;
    }

    const float time_elapsed = CURRENT_TIME - timestamp;
    if (time_elapsed < frame_time) {
      return;
    }

    end_of_sequence = iterator == order.end();
    if (end_of_sequence == false) {
      iterator++;
      end_of_sequence = iterator == order.end();

      timestamp = CURRENT_TIME;
    }

    if (looping == false && end_of_sequence) {
      iterator--;
      return;
    }
    
    if (end_of_sequence) {
      iterator = order.begin();
      end_of_sequence = false;
    }
  }

  typename std::array<Type, Size>::iterator iterator;
  bool end_of_sequence = false;
private:
  std::array<Type, Size> order;
  float timestamp = 0;
};
