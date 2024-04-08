Install dependencies like this:

```
sudo apt install libi2c-dev puredata-dev
```

```
params: none

inlet 1:
  bang - get values
  rotary X Y - set value of X rotary to Y 
  rgb X R G B - set color of X LED (RGB range is 0-255)
  hsv X H S V - set color of X LED (HSV range is float 0-1)

outlet 1:
  rotary N V
  button N V
  switch V
```

where `N` is the index, and `V` is the int value.


