
#pragma once

#include <stdint.h>

struct Switch
{
  int32_t pin;
  char    tag[3];

  Switch() : pin{ 0 }
  {
    tag[0] = 0;
    tag[1] = 0;
    tag[2] = 0;

  }

  Switch(int32_t pinnum, char id, char id_ex) : pin{pinnum}
  {
    tag[0] = id;
    tag[1] = id_ex;
    tag[2] = 0;

  }
};