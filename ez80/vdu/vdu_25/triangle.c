#include <v99x8.h>

static int8_t signum(const int a) {
  if (a < 0)
    return -1;
  if (a > 0)
    return 1;
  return 0;
}

static void draw_clipped_line(uint16_t x1, uint16_t y1, uint16_t x2) {
  if (y1 > gsviewport.bottom)
    return;
  if (y1 < gsviewport.top)
    return;

  if (x1 < gsviewport.left) {
    if (x2 < gsviewport.left)
      return;
    x1 = gsviewport.left;
  }
  if (x1 > gsviewport.right) {
    if (x2 > gsviewport.right)
      return;
    x1 = gsviewport.right;
  }

  if (x2 < gsviewport.left)
    x2 = gsviewport.left;
  if (x2 > gsviewport.right)
    x2 = gsviewport.right;

  vdp_draw_line(x1, y1, x2, y1, current_gfg_colour, current_operation_mode);
}

// Triangle fill code derived from:
// https://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

/**
 * This method rasterizes a triangle using only integer variables by using a
 * modified bresenham algorithm.
 * It's important that v2 and v3 lie on the same horizontal line, that is
 * v2.y must be equal to v3.y.
 * @param v1 point_t of triangle
 * @param v2 point_t of triangle, must have same y-coordinate as v3.
 * @param v3 point_t of triangle, must have same y-coordinate as v2.
 */
static void fill_flat_sided_triangle(const point_t *const v1, const point_t *const v2, const point_t *const v3) {

  point_t vTmp1 = (point_t){v1->x, v1->y};
  point_t vTmp2 = (point_t){v1->x, v1->y};

  bool changed1 = false;
  bool changed2 = false;

  uint16_t dx1 = abs(v2->x - v1->x);
  uint16_t dy1 = abs(v2->y - v1->y);

  uint16_t dx2 = abs(v3->x - v1->x);
  uint16_t dy2 = abs(v3->y - v1->y);

  int8_t signx1 = (int)signum(v2->x - v1->x);
  int8_t signx2 = (int)signum(v3->x - v1->x);

  int8_t signy1 = (int)signum(v2->y - v1->y);
  int8_t signy2 = (int)signum(v3->y - v1->y);

  if (dy1 > dx1) {
    uint16_t tmp = dx1;
    dx1          = dy1;
    dy1          = tmp;
    changed1     = true;
  }

  if (dy2 > dx2) {
    uint16_t tmp = dx2;
    dx2          = dy2;
    dy2          = tmp;
    changed2     = true;
  }

  int16_t e1 = 2 * dy1 - dx1;
  int16_t e2 = 2 * dy2 - dx2;

  for (uint16_t i = 0; i <= dx1; i++) {
    draw_clipped_line(vTmp1.x, vTmp1.y, vTmp2.x);

    while (e1 >= 0) {
      if (changed1)
        vTmp1.x += signx1;
      else
        vTmp1.y += signy1;
      e1 = e1 - 2 * dx1;
    }

    if (changed1)
      vTmp1.y += signy1;
    else
      vTmp1.x += signx1;

    e1 = e1 + 2 * dy1;

    while (vTmp2.y != vTmp1.y) {
      while (e2 >= 0) {
        if (changed2)
          vTmp2.x += signx2;
        else
          vTmp2.y += signy2;
        e2 = e2 - 2 * dx2;
      }

      if (changed2)
        vTmp2.y += signy2;
      else
        vTmp2.x += signx2;

      e2 = e2 + 2 * dy2;
    }
  }
}

static void fill_general_triangle(const point_t *const vt1, const point_t *const vt2, const point_t *const vt3) {
  const point_t vTmp =
      (point_t){(int16_t)((int24_t)vt1->x + (((int24_t)vt3->x - (int24_t)vt1->x) * ((int24_t)vt2->y - (int24_t)vt1->y) /
                                             ((int24_t)vt3->y - (int24_t)vt1->y))),
                vt2->y};
  fill_flat_sided_triangle(vt1, vt2, &vTmp);
  fill_flat_sided_triangle(vt3, vt2, &vTmp);
}

static void fill_triangle(const point_t *vt1, const point_t *vt2, const point_t *vt3) {

  const point_t *vTmp;

  if (vt1->y > vt2->y) {
    vTmp = vt1;
    vt1  = vt2;
    vt2  = vTmp;
  }

  if (vt1->y > vt3->y) {
    vTmp = vt1;
    vt1  = vt3;
    vt3  = vTmp;
  }

  if (vt2->y > vt3->y) {
    vTmp = vt2;
    vt2  = vt3;
    vt3  = vTmp;
  }

  if (vt2->y == vt3->y) {
    fill_flat_sided_triangle(vt1, vt2, vt3);
    return;
  }

  if (vt1->y == vt2->y) {
    fill_flat_sided_triangle(vt3, vt1, vt2);
    return;
  }

  fill_general_triangle(vt1, vt2, vt3);
}
