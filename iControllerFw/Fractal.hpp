#pragma once

#include "device.hpp"

class Fractal : public Device
{
public:
  Fractal(uint8_t totalSwitches) : Device(totalSwitches) { }
 ~Fractal() = default;

  Fractal(const Fractal&) = delete;
  Fractal(Fractal&&) = delete;
  Fractal& operator=(const Fractal&) = delete;
  Fractal& operator=(Fractal&&) = delete;

  inline void setScene(uint8_t scene) const override
  {
    uint8_t msg[3] =
    {
      0xB0 | channel,   //CC + channel
      34,     //scene select
      scene   //0-7
    };

    Serial.write(msg, sizeof(msg));
  }

  inline void setSwitch(uint8_t index, int32_t pinNum, char id, char id_ex)
  {
    if(index < totalSwitches)
    {
      switches[index].pin    = pinNum;
      switches[index].tag[0] = id;
      switches[index].tag[1] = id_ex;

      pinMode(pinNum, INPUT);
    }
  }

  void create() override
  {
    setSwitch(0, 2, ' ', '1');
  }

  inline void sceneDemo()
  {
    for(uint8_t i = 0; i < totalSwitches; i++)
    {
      if(HIGH == digitalRead(switches[i].pin))
      {
          setScene(currScene++);

          while(HIGH == digitalRead(switches[i].pin))
            ;

          return;
      }
    }
  }
};
