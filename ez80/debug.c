#include "ram.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void debug() {
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

uint8_t log_str_z(uint8_t **str_ref) {
  uint8_t *str = *str_ref;

  int i = 50;
  while (*str) {
    if (i-- > 0)
      putchar(*str++);
    else
      str++;
  }

  if (i < 0)
    printf("...");

  *str_ref = str + 1;
  return str[-1];
}

void log_fn_proc(const char *prefix, uint8_t *proc) {
  if (!proc)
    return;

  printf("%06X: ", (uint24_t)proc);
  uint24_t next_proc = *((uint24_t *)proc);

  proc += 3;
  printf("%s", prefix);
  log_str_z(&proc);

  uint24_t proc_ptr = *((uint24_t *)proc);
  printf(" @ %06X %02X %02X LB: %06X\r\n", proc_ptr, proc[3], proc[4],
         (uint24_t)proc + 4);

  log_fn_proc(prefix, (uint8_t *)next_proc);
}

void log_proc(uint8_t *proc) { log_fn_proc("PROC", proc); }

void log_fn(uint8_t *fn) { log_fn_proc("FN", fn); }

void log_variable(uint8_t i, uint8_t *d) {
  if (!d)
    return;

  printf("%06X: ", (uint24_t)d);
  uint24_t next_var = *((uint24_t *)d);
  d += 3;

  putchar(i);
  uint8_t last_char = log_str_z(&d);

  if (last_char == '$') {
    uint8_t len = *d++;
    uint8_t max_len = *d++;
    char *str = *((char **)d);
    d += 3;
    printf(" len: %d max: %d @ %06X LB: %06X ", len, max_len, (uint24_t)str,
           (uint24_t)(d - 1));

    if (str) {
      printf("\r\n%06X: ", (uint24_t)str);
      for (int l = 0; l < len; l++)
        putchar(str[l]);

      printf("  LB: %06X\r\n", (uint24_t)(str + len - 1));
    }

  } else {
    if (d[4] == 0)
      printf(" i: %ld LB: %06X\r\n", *((int32_t *)d), (uint24_t)(d + 4));
    else
      printf(" f: %f LB: %06X\r\n", convert_to_float(d), (uint24_t)(d + 4));
  }

  log_variable(i, (uint8_t *)next_var);
}

#if 0
void print_number(uint16_t high, uint16_t low, uint8_t exp) {
  if (exp == 0) {
    printf("%ld\r\n", (((int32_t)high << 16) + low));
  } else {
    uint8_t hl_number[5] = {high >> 8, high & 0xFF, low >> 8, low & 0xff, exp};
    printf("%f\r\n", convert_to_float(hl_number));
  }
}
#endif

uint8_t valid_var_char(uint8_t c) {
  if (c < 26)
    return c + 'A';

  return c + 69; // a-z
}

void inspect_all() {
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

    printf("%p: %d -- len: %d tok: %x\r\n", p, line_number, length, token);
    p += length;
  }
  printf("%p: end\r\n", p);

  printf("\r\nDynamic Variables:\r\n");
  for (uint8_t i = 0; i < 54; i++) {
    uint8_t *d = DYNVAR[i];
    log_variable(valid_var_char(i), d);
  }

  printf("\r\n");
  log_fn(FNPTR);

  printf("\r\n");
  log_proc(PROPTR);

  printf("\r\nDATPTR @ %p\r\n", DATPTR);
  if (DATPTR) {
    for (int i = 0; i < 16; i++)
      printf("%x ", DATPTR[i]);
    printf("\r\n");
  }

  printf("\r\nFREE: %06X\r\n", FREE);
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
  printf("*hl: %02X %02X %02X %02X %02X %02X\r\n", p[0], p[1], p[2], p[3], p[4],
         p[5]);

  p = (uint8_t *)de;
  printf("*de: %02X %02X %02X %02X %02X %02X\r\n", p[0], p[1], p[2], p[3], p[4],
         p[5]);

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
