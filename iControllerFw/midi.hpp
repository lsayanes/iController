
#pragma once

#include <stdint.h>

struct Midi
{
  uint32_t  baud      { 31250 };
  uint8_t   channel   { 0 };

  Midi() = default;
 ~Midi() = default;

  Midi(const Midi&) = delete;
  Midi(Midi&&) = delete;
  Midi& operator=(const Midi&) = delete;
  Midi& operator=(Midi&&) = delete;

};