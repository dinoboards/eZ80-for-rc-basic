#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ram.h"

void debug() {
  printf("debug:\r\n");
  printf("HIMEM: %p\r\n", HIMEM);
  printf("LOMEM: %p\r\n", LOMEM);
  // printf("FREE: %X\r\n", FREE);
  // printf("FCB: %p\r\n", CPM_SYS_FCB);
  // printf("DISK_BUFFER: %p\r\n", DISK_BUFFER);
  // printf("OPTVAL: %p, %x\r\n", &OPTVAL, OPTVAL);
  // printf("TRPCNT: %p, %x\r\n", &TRPCNT, TRPCNT);
  // printf("RAM_END: %p\r\n", RAM_END);
  // printf("end_of_bss: %p\r\n", end_of_bss);
  // printf("_heap: %p\r\n", _heap);
  // printf("STAVAR: %p, %lX, %lX, %lX\r\n", STAVAR, STAVAR[0], STAVAR[1], STAVAR[2]);
  // printf("FREE_FCB_TABLE: %X\r\n", FREE_FCB_TABLE);

  // printf("TABLE: %p\r\n", TABLE);
  // for (int i = 0; i < 8; i++) {
  //   printf("  TABLE[%d]: %p\r\n", i, TABLE[i]);
  // }
}

void log_info(const char *name,
              uint24_t    af_,
              uint24_t    bc_,
              uint24_t    de_,
              uint24_t    hl_,
              uint24_t    af,
              uint24_t    bc,
              uint24_t    de,
              uint24_t    hl,
              uint24_t    ix,
              uint24_t    iy) {
  printf("log: %s.\r\nAF:%X, BC: %X, DE: %X, HL: %X, AF':%X, BC': %X, DE': %X, "
         "HL': %X, ix: %X, iy: %X\r\n",
         name, af, bc, de, hl, af_, bc_, de_, hl_, ix, iy);

  uint8_t *p = (uint8_t *)ix;
  printf("*iX: ");
  for(int i = 0; i < 8; i++)
    printf("%x ", p[i]);
  printf("\r\n");

  p = (uint8_t *)iy;
  printf("*iy: %x %x\r\n", p[0], p[1]);

  p = (uint8_t *)hl;
  printf("*hl: %x %x\r\n", p[0], p[1]);

  printf("ACCS: ");
  for(int i = 0; i < 8; i++)
    printf("%x ", ACCS[i]);
  printf("\r\n");
}


#define ABORT_X(name)                                                                                                              \
  void abort_##name(uint24_t af, uint24_t bc, uint24_t de, uint24_t hl, uint24_t ix) {                                             \
    printf("Abort " #name ".  AF:%X, BC: %X, DE: %X, HL: %X, ix: %X\r\n", af, bc, de, hl, ix);                                     \
    abort();                                                                                                                       \
  }

ABORT_X(exists)
