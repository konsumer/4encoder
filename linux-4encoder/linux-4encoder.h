#pragma once

#include <i2c/smbus.h>
#include <linux/i2c-dev.h>

#include "../shared.h"

#define ENCODER_ADDR 0x49
#define INCREMENT_REG 0x20
#define BUTTON_REG 0x50
#define SWITCH_REG 0x60
#define RGB_LED_REG 0x70
#define RESET_COUNTER_REG 0x40
#define FIRMWARE_VERSION_REG 0xFE
#define I2C_ADDRESS_REG 0xFF

// TODO: none of this is correct but default address

// Get counter-value (-2147483648 - 2147483647) will be reset after counter-reset
int32_t linux_4encoder_get_counter(int i2c, uint8_t index);

// Set counter-value (-2147483648 - 2147483647) will be reset after counter-reset
void linux_4encoder_set_counter(int i2c, uint8_t index, uint32_t value);

// Get current rotary increment (-2147483648 - 2147483647) will be reset after get
int32_t linux_4encoder_get_increment(int i2c, uint8_t index);

// Reset the counter on a rotary
void linux_4encoder_counter_reset(int i2c, uint8_t index);

// Is the rotary-button pressed?
bool linux_4encoder_button_down(int i2c, uint8_t index);

// Get current switch value (0/1)
uint8_t linux_4encoder_switch(int i2c);

// Get the current RGB value of an LED as an int (0x000000 - 0xffffff)
uint32_t linux_4encoder_get_led_int(int8_t i2c, uint8_t index);

// Set the current RGB value of an LED using an int (0x000000 - 0xffffff)
void linux_4encoder_set_led_int(int8_t i2c, uint8_t index, uint32_t color);

// Get the firmware version
uint8_t linux_4encoder_get_firmware(int8_t i2c);

// Get the i2c address (1-127: you already know this because that is how you talk to it)
uint8_t linux_4encoder_get_address(int8_t i2c);

// Set the i2c address (1-127) AFter, it will have a new address (so you will need to re-init i2c)
void linux_4encoder_set_address(int8_t i2c, uint8_t address);

// Get the current RGB value of an LED as a struct
ColorRGB linux_4encoder_get_led_rgb(int8_t i2c, uint8_t index);

// Set the current RGB value of an LED using a struct
void linux_4encoder_set_led_rgb(int8_t i2c, uint8_t index, ColorRGB value);

// Set the current HSV value of an LED using a struct
void linux_4encoder_set_led_hsv(int8_t i2c, uint8_t index, ColorHSV value);

// define this, if you just want to use this as a header
#ifndef NO_LINUX_8ENCODER_IMPLEMENTATION

// Get counter-value (-2147483648 - 2147483647) will be reset after counter-reset
int32_t linux_4encoder_get_counter(int i2c, uint8_t index) {
  int32_t out = 0;
  i2c_get_register_val(i2c, index * 4, &out, 4);
  return out;
}

// Set counter-value (-2147483648 - 2147483647) will be reset after counter-reset
void linux_4encoder_set_counter(int i2c, uint8_t index, uint32_t value) {
  i2c_set_register_val(i2c, index * 4, &value, 4);
}

// Get current rotary increment (-2147483648 - 2147483647) will be reset after get
int32_t linux_4encoder_get_increment(int i2c, uint8_t index) {
  int32_t out = 0;
  i2c_get_register_val(i2c, INCREMENT_REG + (index * 4), &out, 4);
  return out;
}

// Reset the counter on a rotary
void linux_4encoder_counter_reset(int i2c, uint8_t index) {
  int v = 1;
  i2c_set_register_val(i2c, RESET_COUNTER_REG + index, &v, 1);
}

// Is the rotary-button pressed?
bool linux_4encoder_button_down(int i2c, uint8_t index) {
  uint8_t out = 0;
  i2c_get_register_val(i2c, BUTTON_REG + index, &out, 1);
  return out == 0;  // inverted
}

// Get current switch value (0/1)
uint8_t linux_4encoder_switch(int i2c) {
  uint8_t out = 0;
  i2c_get_register_val(i2c, SWITCH_REG, &out, 1);
  return out;
}

// Get the current RGB value of an LED as an int (0x000000 - 0xffffff)
uint32_t linux_4encoder_get_led_int(int8_t i2c, uint8_t index) {
  int out = 0;
  i2c_get_register_val(i2c, RGB_LED_REG + (index * 3), &out, 3);
  return out;
}

// Set the current RGB value of an LED using an int (0x000000 - 0xffffff)
void linux_4encoder_set_led_int(int8_t i2c, uint8_t index, uint32_t color) {
  i2c_set_register_val(i2c, RGB_LED_REG + (index * 3), &color, 3);
}

// Get the firmware version
uint8_t linux_4encoder_get_firmware(int8_t i2c) {
  uint8_t out = 0;
  i2c_get_register_val(i2c, FIRMWARE_VERSION_REG, &out, 1);
  return out;
}

// Get the i2c address (1-127: you already know this because that is how you talk to it)
uint8_t linux_4encoder_get_address(int8_t i2c) {
  uint8_t out = 0;
  i2c_get_register_val(i2c, I2C_ADDRESS_REG, &out, 1);
  return out;
}

// Set the i2c address (1-127) AFter, it will have a new address (so you will need to re-init i2c)
void linux_4encoder_set_address(int8_t i2c, uint8_t address) {
  i2c_set_register_val(i2c, I2C_ADDRESS_REG, &address, 3);
}

// Get the current RGB value of an LED as a struct
ColorRGB linux_4encoder_get_led_rgb(int8_t i2c, uint8_t index) {
  ColorRGB out = {0};
  i2c_get_register_val(i2c, RGB_LED_REG + (index * 3), &out, 3);
  return out;
}

// Set the current RGB value of an LED using a struct
void linux_4encoder_set_led_rgb(int8_t i2c, uint8_t index, ColorRGB value) {
  i2c_set_register_val(i2c, RGB_LED_REG + (index * 3), &value, 3);
}

// Set the current HSV value of an LED using a struct
void linux_4encoder_set_led_hsv(int8_t i2c, uint8_t index, ColorHSV value) {
  linux_4encoder_set_led_int(i2c, index, hsv_to_rgb_int(value.h, value.s, value.v));
}

#endif  // NO_LINUX_8ENCODER_IMPLEMENTATION
