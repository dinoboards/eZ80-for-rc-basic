// #include "vdu.h"
// #include "bbcbasic.h"
#include "vdu.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <v99x8-super.h>

#include <stdio.h>

// Y co-ord to store font data
// need enough room to allow 2 pages for all modes
#define FONT_Y_OFFSET (current_display_mode >= 16 ? 256 : (576 * 2))

static void graphic_print_char(uint8_t ch) {

  // calculate real physical location to begin printing;

  if ((ch < ' ')) { // TODO: WE NEED TO RENDER OUT BITMAPS FOR ALL CHARS AFTER ' '
    // printf("TODO: process char 0x'%X'\n", ch);
    return;
  }

  const point_t gpos = (point_t){(int16_t)current_tpos.x * 8, (int16_t)current_tpos.y * 8};

  // calculate x and y of 'ch'
  const uint16_t from_x = (ch % 32) * 8;
  const uint16_t from_y = FONT_Y_OFFSET + (ch / 32) * 8;

  prepare_font_pattern(ch, from_x, from_y);

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(from_x, from_y, gpos.x, gpos.y, 8, 8, 0, 0);

  current_tpos.x++;
  if (current_tpos.x > tviewport.right) {
    vdu_cr();
    vdu_lf();
  }
}

uint24_t mos_oswrite(uint8_t ch) {
  if (vdu_required_length) {
    data[vdu_index++] = ch;
    if (vdu_index == vdu_required_length) {
      mos_vdu_handler fn  = current_fn;
      current_fn          = NULL;
      vdu_index           = 0;
      vdu_required_length = 0;

      fn(); // may error and not return
    }
    return -1;
  }

  if (ch == 8) {
    vdu_bs();
    return ch;
  }

  if (ch == '\n') {
    vdu_lf();
    return ch;
  }

  if (ch == 12) { // cls
    if (current_display_mode != 255)
      vdu_cls();

    return 0x0C; // send formfeed to serial
  }

  if (ch == '\r') {
    vdu_cr();
    return ch;
  }

  if (ch == 16) { // clg
    vdu_clg();
    return -1;
  }

  if (ch == 17) { // vdu_colour
    current_fn          = vdu_colour;
    vdu_required_length = 1;
    return -1;
  }

  if (ch == 18) { // gcol mode, colour
    current_fn          = vdu_gcol;
    vdu_required_length = 2;
    return -1;
  }

  if (ch == 19) { // colour
    current_fn          = vdu_colour_define;
    vdu_required_length = 5;
    return -1;
  }

  if (ch == 22) { // MODE
    current_fn          = vdu_mode;
    vdu_required_length = 1;
    return -1;
  }

  if (ch == 23) { // multi purpose
    current_fn          = vdu_multi_purpose;
    vdu_required_length = 9;
    return -1;
  }

  if (ch == 24) { // set g viewport
    current_fn          = vdu_set_gviewport;
    vdu_required_length = 8;
    return -1;
  }

  if (ch == 25) { // plot
    current_fn          = vdu_plot;
    vdu_required_length = 5;
    return -1;
  }

  if (ch == 28) { // set text viewport
    current_fn          = vdu_set_tviewport;
    vdu_required_length = 4;
    return -1;
  }

  if (ch == 29) { // set origin
    current_fn          = vdu_set_origin;
    vdu_required_length = 4;
    return -1;
  }

  if (ch == 31) {
    current_fn          = vdu_tab;
    vdu_required_length = 2;
    return -1;
  }

  if (current_display_mode == 255)
    return ch;

  graphic_print_char(ch);
  // print to graphic screen at current text post

  // for the time, lets dual output to serial and graphic
  if (ch <= 127)
    return ch;

  return -1;
}
