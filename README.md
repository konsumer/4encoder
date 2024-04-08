# 4encoder

> [!WARNING]  
> I am still working on this. these devices are quite a bit more complicated to interface in C than [these](https://github.com/konsumer/8encoder/), so it will probly take some time.

Various libs for working with [an i2c 4 rotary-encoder thing](https://www.adafruit.com/product/5752) on a pi.

- [linux-4encoder](linux-4encoder) - C lib. Just uses regular linux stuff.
- [puredata-pi4encoder](puredata-pi4encoder) - Puredata extension that use linux-4encoder
- Simple python example [here](https://github.com/konsumer/4encoder/blob/main/demo.py) that uses blinka/circuitpython

Make sure to add this to your `/boot/firmware/config.txt`:

```
# dtparam=i2c_arm=on,i2c_arm_baudrate=50000
# dtparam=i2c_arm=on,i2c_arm_baudrate=100000
# dtparam=i2c_arm=on,i2c_arm_baudrate=400000
dtparam=i2c_arm=on,i2c_arm_baudrate=1000000
```
