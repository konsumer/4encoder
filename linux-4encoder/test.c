#include "linux-4encoder.h"

int main(int argc, char* argv[]) {
  int i2c = open("/dev/i2c-1", O_RDWR);
  if (i2c < 0) {
    return 1;
  }
  if (ioctl(i2c, I2C_SLAVE, ENCODER_ADDR) < 0) {
    return 2;
  }

  printf("Firmware version: %d\nAddress: 0x%.2x\n", linux_4encoder_get_firmware(i2c), linux_4encoder_get_address(i2c));

  for (int i = 0; i < 4; i++) {
    // same as linux_4encoder_set_counter(i2c, i, 0);
    linux_4encoder_counter_reset(i2c, i);
  }

  ColorHSV c = {.h = 0.0, .s = 1.0, .v = 1.0};

  while (true) {
    for (int i = 0; i < 4; i++) {
      if (linux_4encoder_button_down(i2c, i)) {
        linux_4encoder_set_led_int(i2c, i, 0xffffff);
      } else {
        // make a rainbow with HSV
        c.h = (float)i * 0.25;
        linux_4encoder_set_led_hsv(i2c, i, c);
      }
    }
    printf("%d\n", linux_4encoder_get_counter(i2c, 0));
  }

  return 0;
}
