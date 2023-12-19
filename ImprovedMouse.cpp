#include "ImprovedMouse.h"

static const uint8_t _hidMultiReportDescriptorMouse[] PROGMEM = {

    0x05, 0x01,              
    0x09, 0x02,              
    0xa1, 0x01,              
    0x85, HID_REPORTID_MOUSE,


    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x08,
    0x15, 0x00,
    0x25, 0x01,
    0x95, 0x08,
    0x75, 0x01,
    0x81, 0x02,


    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x09, 0x38,
    0x15, 0x81,
    0x25, 0x7f,
    0x75, 0x08,
    0x95, 0x03,
    0x81, 0x06,


    0xc0 
};

Mouse_::Mouse_(void)
{
  static HIDSubDescriptor node(_hidMultiReportDescriptorMouse, sizeof(_hidMultiReportDescriptorMouse));
  HID().AppendDescriptor(&node);
}

void Mouse_::begin(void)
{
  end();
}

void Mouse_::end(void)
{
  _buttons = 0;
  move(0, 0, 0);
}

void Mouse_::click(uint8_t b)
{
  _buttons = b;
  move(0, 0, 0);
  _buttons = 0;
  move(0, 0, 0);
}

void Mouse_::move(signed char x, signed char y, signed char wheel)
{
  HID_MouseReport_Data_t report;
  report.buttons = _buttons;
  report.xAxis = x;
  report.yAxis = y;
  report.wheel = wheel;
  SendReport(&report, sizeof(report));
}

void Mouse_::buttons(uint8_t b)
{
  if (b != _buttons)
  {
    _buttons = b;
    move(0, 0, 0);
  }
}

void Mouse_::press(uint8_t b)
{
  buttons(_buttons | b);
}

void Mouse_::release(uint8_t b)
{
  buttons(_buttons & ~b);
}

void Mouse_::releaseAll(void)
{
  _buttons = 0;
  move(0, 0, 0);
}

bool Mouse_::isPressed(uint8_t b)
{
  if ((b & _buttons) > 0)
    return true;
  return false;
}

void Mouse_::SendReport(void *data, int length)
{
  HID().SendReport(HID_REPORTID_MOUSE, data, length);
}

Mouse_ Mouse;
