#include "ram.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void debug() {
  printf("debug:\r\n");
  printf("HIMEM: %p\r\n", HIMEM);
  printf("LOMEM: %p\r\n", LOMEM);
  printf("PAGE: %p\r\n", PAGE);
  printf("FCB_BLOCKS: %p\r\n", FCB_BLOCKS);

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

float convert_to_float(uint8_t *p) {
  float mantissa = 0.0;
  uint8_t exponent = p[4] - 127;

  for (uint8_t i = 0; i < 4; i++) {
    const int byteValue = p[3 - i];
    const int exponent = -(i * 8 + 8);
    const float byteFraction = byteValue * pow(2, exponent);
    mantissa += byteFraction;
  }

  // Add the implicit leading 1
  mantissa += 0.5;

  const float realNumber = mantissa * pow(2, exponent);

  return realNumber;
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

  } else {
    if (d[4] == 0)
      printf(", int: %ld\r\n", *((int32_t *)d));
    else
      printf(", float: %f \r\n", convert_to_float(d));

    for (int i = 0; i < 5; i++) {
      printf("%x ", *d++);
    }
    printf("\r\n");
  }

  if (next)
    log_variable(i, next);
}

void print_number(uint16_t high, uint16_t low, uint8_t exp) {
  if (exp == 0) {
    printf("%ld\r\n", (((int32_t)high << 16) + low));
  } else {
    uint8_t hl_number[5] = {high >> 8, high & 0xFF, low >> 8, low & 0xff, exp};
    printf("%f\r\n", convert_to_float(hl_number));
  }
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

void log_info(const char *name, uint24_t *sp, uint24_t af_, uint24_t bc_,
              uint24_t de_, uint24_t hl_, uint24_t af, uint24_t bc, uint24_t de,
              uint24_t hl, uint24_t ix, uint24_t iy) {
  printf("log: %s.\r\nAF:%X, BC: %X, DE: %X, HL: %X, AF':%X, BC': %X, DE': %X, "
         "HL': %X, ix: %X, iy: %X\r\n",
         name, af, bc, de, hl, af_, bc_, de_, hl_, ix, iy);

  printf("*sp: (%p) ", sp);
  for (int i = 0; i < 7; i++)
    printf("%02X ", sp[i]);

  printf("\r\n");

  uint8_t *p = (uint8_t *)ix;
  printf("*ix: ");
  for (int i = -6; i < 0; i++)
    printf("%02X ", p[i]);

  printf(" | ");

  for (int i = 0; i < 12; i++)
    printf("%02X ", p[i]);

  printf("\r\n");

  p = (uint8_t *)iy;
  printf("*iy: ");
  for (int i = -6; i < 0; i++)
    printf("%02X ", p[i]);

  printf(" | ");

  for (int i = 0; i < 12; i++)
    printf("%02X ", p[i]);

  printf("\r\n");

  p = (uint8_t *)hl;
  printf("*hl: %02x %02x\r\n", p[0], p[1]);

  printf("ACCS: ");
  for (int i = 0; i < 6; i++)
    printf("%02X ", ACCS[i]);
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
