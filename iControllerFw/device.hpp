#pragma once

#include "midi.hpp"
#include "switch.hpp"

class Device : public Midi
{
public:

  Device(const uint8_t totalSw):
    totalSwitches { totalSw },
    switches      { new Switch[totalSw] }
  {
  }

 ~Device()
  {
    delete[] switches;
  }

  Device(const Device&) = delete;
  Device(Device&&) = delete;
  Device& operator=(const Device&) = delete;
  Device& operator=(Device&&) = delete;

  virtual void create() = 0;
  virtual void setScene(uint8_t scene) const = 0;

protected:

  uint8_t     totalSwitches;
  Switch*     switches;

  uint8_t currScene { 0 };

};
