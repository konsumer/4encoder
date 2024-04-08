/*

Shared header for utils that are used by all C implementations, for various driver-libs.

*/

#pragma once

// TODO: do I really need all these?
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// convert 4 uint8_t to uint32_t
uint32_t uint8s_to_32(uint8_t v[4]) {
  return v[0] | (v[1] << 8) | (v[2] << 16) | (v[3] << 24);
}

// HSV is 0-1 floats, returns single uint32
uint32_t hsv_to_rgb_int(float h, float s, float v) {
  uint8_t hsv[3] = {(uint8_t)(h * 255.0), (uint8_t)(s * 255.0), (uint8_t)(v * 255.0)};

  uint8_t rgb[4] = {0};
  uint8_t region, remainder, p, q, t;

  if (hsv[1] == 0) {
    rgb[0] = hsv[2];
    rgb[1] = hsv[2];
    rgb[2] = hsv[2];
    return uint8s_to_32(rgb);
  }

  region = hsv[0] / 43;
  remainder = (hsv[0] - (region * 43)) * 6;

  p = (hsv[2] * (255 - hsv[1])) >> 8;
  q = (hsv[2] * (255 - ((hsv[1] * remainder) >> 8))) >> 8;
  t = (hsv[2] * (255 - ((hsv[1] * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
    case 0:
      rgb[0] = hsv[2];
      rgb[1] = t;
      rgb[2] = p;
      break;
    case 1:
      rgb[0] = q;
      rgb[1] = hsv[2];
      rgb[2] = p;
      break;
    case 2:
      rgb[0] = p;
      rgb[1] = hsv[2];
      rgb[2] = t;
      break;
    case 3:
      rgb[0] = p;
      rgb[1] = q;
      rgb[2] = hsv[2];
      break;
    case 4:
      rgb[0] = t;
      rgb[1] = p;
      rgb[2] = hsv[2];
      break;
    default:
      rgb[0] = hsv[2];
      rgb[1] = p;
      rgb[2] = q;
      break;
  }

  return uint8s_to_32(rgb);
}

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} ColorRGB;

typedef struct {
  float h;
  float s;
  float v;
} ColorHSV;

// Get a value, by register
bool i2c_get_register_val(int i2c, uint8_t reg, void* out_pntr, uint8_t len) {
  if (write(i2c, &reg, 1) != 1) {
    return false;
  }
  // 1us pause helps with missing messages
  usleep(1);
  if (read(i2c, out_pntr, len) != len) {
    return false;
  }
  // 10us pause helps with missing messages
  usleep(10);
  return true;
}

// Set a value, by register (max length 32 bytes)
bool i2c_set_register_val(int i2c, uint8_t reg, void* in_ptr, uint8_t len) {
  uint8_t msg[33] = {0};
  msg[0] = reg;
  memcpy(msg + 1, in_ptr, len);

  if (write(i2c, msg, len + 1) != (len + 1)) {
    return false;
  }

  // 100us pause helps with missing messages
  usleep(100);
  return true;
}
