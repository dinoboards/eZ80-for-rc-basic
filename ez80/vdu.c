// #include "vdu.h"
// #include "bbcbasic.h"
#include "vdu.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <v99x8-super.h>

#include <stdio.h>

uint24_t mos_oswrite(uint8_t ch) {
  if (vdu_required_length) {
    data[vdu_index++] = ch;
    if (vdu_index == vdu_required_length) {
      mos_vdu_handler fn = current_fn;
      current_fn = NULL;
      vdu_index = 0;
      vdu_required_length = 0;

      fn(); // may error and not return
    }
    return -1;
  }

  if (ch == 12) { // cls
    if (current_display_mode != 255)
      vdu_cls();

    return 0x0C; // send formfeed to serial
  }

  if (ch == 17) { // vdu_colour
    current_fn = vdu_colour;
    vdu_required_length = 1;
    return -1;
  }

  if (ch == 22) { // MODE
    current_fn = vdu_mode;
    vdu_required_length = 1;
    return -1;
  }

  return ch;
}
