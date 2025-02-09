#include "ram.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void debug() {
  printf("debug:\r\n");
  printf("HIMEM: %p\r\n", HIMEM);
  printf("LOMEM: %p\r\n", LOMEM);
  printf("PAGE: %p\r\n", PAGE);
  // printf("FCB: %p\r\n", CPM_SYS_FCB);
  // printf("DISK_BUFFER: %p\r\n", DISK_BUFFER);
  // printf("OPTVAL: %p, %x\r\n", &OPTVAL, OPTVAL);
  // printf("TRPCNT: %p, %x\r\n", &TRPCNT, TRPCNT);
  // printf("RAM_END: %p\r\n", RAM_END);
  // printf("end_of_bss: %p\r\n", end_of_bss);
  // printf("_heap: %p\r\n", _heap);
  // printf("STAVAR: %p, %lX, %lX, %lX\r\n", STAVAR, STAVAR[0], STAVAR[1],
  // STAVAR[2]); printf("FREE_FCB_TABLE: %X\r\n", FREE_FCB_TABLE);

  // printf("TABLE: %p\r\n", TABLE);
  // for (int i = 0; i < 8; i++) {
  //   printf("  TABLE[%d]: %p\r\n", i, TABLE[i]);
  // }
}

void log_variable(int i, uint8_t *d) {
  printf("%c: %p ", i + 'A', d);
  uint8_t lb = *d++;
  uint8_t mb = *d++;
  uint8_t ub = *d++;
  uint8_t *next =
      (uint8_t *)(((uint24_t)ub << 16) + ((uint24_t)mb << 8) + (uint24_t)lb);
  printf("Next: %p, '", next);

  bool is_string = false;
  while (*d != 0) {
    is_string = *d == '$';
    printf("%c", *d++);
  }
  printf("'");

  d++;

  if (is_string) {
    uint8_t len = *d++;
    uint8_t max_len = *d++;
    printf(", len: %d, max: %d ", len, max_len);

    char *str = *((char **)d);
    printf("%p: '", str);
    for (int l = 0; l < len; l++)
      printf("%c", str[l]);

    printf("', last_byte @ %p\r\n", d + 2);

  } else
    printf("\r\n");

  if (next)
    log_variable(i, next);
}

void inspect_all() {
  printf("FLAGS: %x, INKEY: %x, EDPTR: %X\r\n", FLAGS, INKEY, EDPTR);

  printf("\r\nPROGAM @ %p:\r\n", PAGE);
  uint8_t *p = PAGE;
  while (1) {
    uint8_t length = p[0];

    if (length == 0)
      break;

    uint8_t ln_lsb = p[1];
    uint8_t ln_msb = p[2];
    uint16_t line_number = ln_msb * 256 + ln_lsb;
    uint8_t token = p[3];

    printf("%p: len: %d num: %d tok: %x\r\n", p, length, line_number, token);
    p += length;
  }
  printf("%p: end\r\n", p);

  printf("\r\nDYNVAR @ %p:\r\n", DYNVAR);
  for (int i = 0; i < 54; i++) {
    uint8_t *d = DYNVAR[i];
    if (d != 0) {
      log_variable(i, d);
    }
  }
  printf("\r\n");

  printf("\r\nFNPTR @ %p\r\n", FNPTR);
  if (FNPTR) {
    for (int i = 0; i < 16; i++)
      printf("%x ", FNPTR[i]);
    printf("\r\n");
  }
}

void log_info(const char *name, uint24_t af_, uint24_t bc_, uint24_t de_,
              uint24_t hl_, uint24_t af, uint24_t bc, uint24_t de, uint24_t hl,
              uint24_t ix, uint24_t iy) {
  printf("log: %s.\r\nAF:%X, BC: %X, DE: %X, HL: %X, AF':%X, BC': %X, DE': %X, "
         "HL': %X, ix: %X, iy: %X\r\n",
         name, af, bc, de, hl, af_, bc_, de_, hl_, ix, iy);

  uint8_t *p = (uint8_t *)ix;
  printf("*ix: ");
  for (int i = -6; i < 20; i++)
    printf("%x ", p[i]);
  printf("\r\n");

  p = (uint8_t *)iy;
  printf("*iy: ");
  for (int i = -6; i < 20; i++)
    printf("%x ", p[i]);
  printf("\r\n");

  p = (uint8_t *)hl;
  printf("*hl: %x %x\r\n", p[0], p[1]);

  printf("ACCS: ");
  for (int i = 0; i < 8; i++)
    printf("%x ", ACCS[i]);
  printf("\r\n");
}

#define ABORT_X(name)                                                          \
  void abort_##name(uint24_t af, uint24_t bc, uint24_t de, uint24_t hl,        \
                    uint24_t ix) {                                             \
    printf("Abort " #name ".  AF:%X, BC: %X, DE: %X, HL: %X, ix: %X\r\n", af,  \
           bc, de, hl, ix);                                                    \
    abort();                                                                   \
  }

ABORT_X(exists)
