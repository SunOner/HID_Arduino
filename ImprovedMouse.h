#pragma once

#define HID_REPORTID_MOUSE 1

#include <HID.h>

#define MOUSE_LEFT (1 << 0)
#define MOUSE_RIGHT (1 << 1)
#define MOUSE_MIDDLE (1 << 2)
#define MOUSE_PREV (1 << 3)
#define MOUSE_NEXT (1 << 4)
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE | MOUSE_PREV | MOUSE_NEXT)

typedef union ATTRIBUTE_PACKED
{
  struct
  {
    uint8_t buttons;
    int8_t xAxis;
    int8_t yAxis;
    int8_t wheel;
  };
} HID_MouseReport_Data_t;

class Mouse_
{
public:
  Mouse_(void);
  void begin(void);
  void end(void);
  void click(uint8_t b = MOUSE_LEFT);
  void move(signed char x, signed char y, signed char wheel = 0);
  void press(uint8_t b = MOUSE_LEFT);
  void release(uint8_t b = MOUSE_LEFT);
  void releaseAll(void);
  bool isPressed(uint8_t b = MOUSE_LEFT);

  void SendReport(void *data, int length);

protected:
  uint8_t _buttons;
  inline void buttons(uint8_t b);
};
extern Mouse_ Mouse;