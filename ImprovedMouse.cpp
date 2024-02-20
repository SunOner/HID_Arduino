#include "ImprovedMouse.h"

static const uint8_t _hidMultiReportDescriptorMouse[] PROGMEM = {

    0x05, 0x01,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,              // USAGE (Mouse)
    0xa1, 0x01,              // COLLECTION (Application)
    0x85, HID_REPORTID_MOUSE, // REPORT_ID (Report ID)


    0x05, 0x09,              // USAGE_PAGE (Button)
    0x19, 0x01,              // USAGE_MINIMUM (Button 1)
    0x29, 0x08,              // USAGE_MAXIMUM (Button 8)
    0x15, 0x00,              // LOGICAL_MINIMUM (0)
    0x25, 0x01,              // LOGICAL_MAXIMUM (1)
    0x95, 0x08,              // REPORT_COUNT (8)
    0x75, 0x01,              // REPORT_SIZE (1)
    0x81, 0x02,              // INPUT (Data,Var,Abs)


    0x05, 0x01,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x30,              // USAGE (X)
    0x09, 0x31,              // USAGE (Y)
    0x09, 0x38,              // USAGE (Wheel)
    0x15, 0x81,              // LOGICAL_MINIMUM (-127)
    0x25, 0x7f,              // LOGICAL_MAXIMUM (127)
    0x75, 0x08,              // REPORT_SIZE (8)
    0x95, 0x03,              // REPORT_COUNT (3)
    0x81, 0x06,              // INPUT (Data,Var,Rel)


    0xc0                     // END_COLLECTION
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
