#include <hidboot.h>
#include "ImprovedMouse.h"

#define CHECK_BIT(var, pos) ((var)&pos)

struct MYMOUSEINFO
{
  uint8_t buttons;
  uint16_t dX;
  uint16_t dY;
  int8_t wheel;
};

class MouseRptParser : public MouseReportParser
{
  union
  {
    MYMOUSEINFO mouseInfo;
    uint16_t bInfo[sizeof(MYMOUSEINFO)];
  } prevState;

protected:
  void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);

  void OnMouseMove(MYMOUSEINFO *mi);
  void OnWheelMove(MYMOUSEINFO *mi);
};