#include "ram.h"
// #include "vdu.h"
#include <cpm.h>
#include <stdbool.h>
#include <stdio.h>

static const uint8_t test_values[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                                        0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB,
                                        0xCC, 0xDD, 0xEE, 0xFF};

static bool test_at(volatile uint8_t *ptr) {
  const uint8_t *values = test_values;
  for (int i = 0; i < 16; i++)
    *ptr++ = *values++;

  ptr--;
  values--;

  for (int i = 0; i < 16; i++)
    if (*ptr-- != *values--)
      return false;

  return true;
}


/* seach from heap up - and discover how much ram is installed above heap */
/* return the address of the byte just above available ram*/
/* assumes a 64k paging */
static uint8_t* find_mem_top(uint8_t* heap) {
  uint8_t* current = (uint8_t *)(((uint24_t)heap + 65535) & ~65535);;

  while (test_at(current))
    current += 0x10000; // Increment by 64K blocks

  return current;
}

void osinit() {
  // init_font_patterns();

  LOMEM = (uint8_t *)(((uint24_t)_heap + 255) & ~255);
  printf("LOMEM: %p\r\n", LOMEM);

  HIMEM = find_mem_top(LOMEM);

  printf("HIMEM: %p\r\n", HIMEM);
  printf("%d bytes available\r\n", HIMEM - LOMEM);
}
