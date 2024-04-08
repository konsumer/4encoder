#include <stdlib.h>
#include "linux-4encoder.h"
#include "m_pd.h"

static t_class* pi4encoder_class;

typedef struct _pi4encoder {
  t_object x_obj;
  t_outlet* x_out;
  int i2c;
  int tracking_buttons[4];
  int tracking_rotary[4];
  int tracking_switch;
} t_pi4encoder;

static void pi4encoder_outputchanges(t_pi4encoder* x) {
  t_atom msg[3] = {};
  int v = linux_4encoder_switch(x->i2c);
  if (x->tracking_switch != v) {
    SETSYMBOL(&msg[0], gensym("switch"));
    SETFLOAT(&msg[1], v);
    outlet_list(x->x_out, 0, 2, msg);
    x->tracking_switch = v;
  }
  for (int i = 0; i < 4; i++) {
    v = linux_4encoder_get_counter(x->i2c, i);
    if (x->tracking_rotary[i] != v) {
      SETSYMBOL(&msg[0], gensym("rotary"));
      SETFLOAT(&msg[1], i);
      SETFLOAT(&msg[2], v);
      outlet_list(x->x_out, 0, 3, msg);
      x->tracking_rotary[i] = v;
    }

    v = linux_4encoder_button_down(x->i2c, i) ? 1 : 0;
    if (x->tracking_buttons[i] != v) {
      SETSYMBOL(&msg[0], gensym("button"));
      SETFLOAT(&msg[1], i);
      SETFLOAT(&msg[2], v);
      outlet_list(x->x_out, 0, 3, msg);
      x->tracking_buttons[i] = v;
    }
  }
}

static void pi4encoder_handle_rgb(t_pi4encoder* x, t_floatarg i, t_floatarg r, t_floatarg g, t_floatarg b) {
  ColorRGB c = {0};
  c.r = r;
  c.g = g;
  c.b = b;
  linux_4encoder_set_led_rgb(x->i2c, i, c);
}

static void pi4encoder_handle_hsv(t_pi4encoder* x, t_floatarg i, t_floatarg h, t_floatarg s, t_floatarg v) {
  ColorHSV c = {0};
  c.h = h;
  c.s = s;
  c.v = v;
  linux_4encoder_set_led_hsv(x->i2c, i, c);
}

static void pi4encoder_handle_rotary(t_pi4encoder* x, t_floatarg i, t_floatarg value) {
  linux_4encoder_set_counter(x->i2c, i, value);
}

static void pi4encoder_free(t_pi4encoder* x) {
  post("[pi4encoder] terminated");
}

static void* pi4encoder_new() {
  t_pi4encoder* x = (t_pi4encoder*)pd_new(pi4encoder_class);
  x->i2c = open("/dev/i2c-1", O_RDWR);
  if (x->i2c < 0 || ioctl(x->i2c, I2C_SLAVE, ENCODER_ADDR) < 0) {
    post("[pi4encoder] init error.");
    // TODO: how to return error here, so it fails?
    return NULL;
  }
  x->x_out = outlet_new(&x->x_obj, gensym("list"));
  for (int i = 0; i < 4; i++) {
    x->tracking_buttons[i] = 0;
    x->tracking_rotary[i] = 0;
  }
  x->tracking_switch = 0;

  post("[pi4encoder] initialised.");
  return x;
}

void pi4encoder_setup(void) {
  pi4encoder_class = class_new(gensym("pi4encoder"),
                               (t_newmethod)pi4encoder_new,
                               (t_method)pi4encoder_free,
                               sizeof(t_pi4encoder),
                               CLASS_DEFAULT,
                               0);

  class_addmethod(
      pi4encoder_class,
      (t_method)pi4encoder_handle_rgb,
      gensym("rgb"),
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      0);
  class_addmethod(
      pi4encoder_class,
      (t_method)pi4encoder_handle_hsv,
      gensym("hsv"),
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      0);
  class_addmethod(
      pi4encoder_class,
      (t_method)pi4encoder_handle_rotary,
      gensym("rotary"),
      A_DEFFLOAT,
      A_DEFFLOAT,
      0);

  class_addbang(pi4encoder_class, pi4encoder_outputchanges);
}
