#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern void mainxx();

int main() {
  printf("hello");

  mainxx();

  return 0;

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
  printf("log: %s.  AF:%X, BC: %X, DE: %X, HL: %X, AF':%X, BC': %X, DE': %X, "
         "HL': %X, ix: %X, iy: %X\r\n",
         name, af, bc, de, hl, af_, bc_, de_, hl_, ix, iy);

  uint8_t *p = (uint8_t *)ix;
  printf("*iX: %x %x\r\n", p[0], p[1]);

  p = (uint8_t *)iy;
  printf("*iy: %x %x\r\n", p[0], p[1]);

  p = (uint8_t *)hl;
  printf("*hl: %x %x\r\n", p[0], p[1]);
}
