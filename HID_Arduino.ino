#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>
#include <usbhub.h>
#include <string.h>

#include "hidcustom.h"

signed char delta[3] = {0, 0, 0};

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb, true);
MouseRptParser Prs;

void HandleButtonChange(uint8_t prevState, uint8_t newState, uint8_t button);

void setup()
{
    Serial.begin(57600);
    Serial.setTimeout(1);
    Usb.Init();
    HidMouse.SetReportParser(0, &Prs);
    Mouse.begin();
}

void loop()
{
    memset(delta, 0, sizeof(delta));

    Usb.Task();

    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');
        ParseSerialCommand(command);
    }

    Mouse.move(delta[0], delta[1], delta[2]);
}

void MouseRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    MYMOUSEINFO *pmi = reinterpret_cast<MYMOUSEINFO*>(buf);

    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_LEFT);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_RIGHT);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_MIDDLE);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_PREV);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_NEXT);

    if (pmi->dX || pmi->dY)
    {
        OnMouseMove(pmi);
    }

    if (pmi->wheel)
    {
        OnWheelMove(pmi);
    }
    
    prevState.mouseInfo.buttons = pmi->buttons;
    prevState.bInfo[0] = buf[0];
}

void MouseRptParser::OnMouseMove(MYMOUSEINFO *mi)
{
    delta[0] = mi->dX;
    delta[1] = mi->dY;
}

void MouseRptParser::OnWheelMove(MYMOUSEINFO *mi)
{
    Mouse.move(0, 0, mi->wheel);
}

void ParseSerialCommand(const String& command)
{
    if (command == "c")
    {
        Mouse.click();
    }
    else if (command == "r")
    {
        Mouse.release();
    }
    else if (command == "p")
    {
        Mouse.press();
    }
    else if (command.startsWith("m"))
    {
        ExecuteMouseMoveCommand(command);
    }
}

void ExecuteMouseMoveCommand(const String& command)
{
    String moveCommand = command;
    moveCommand.replace("m", "");

    int commaIndex = moveCommand.indexOf(',');
    int x = moveCommand.substring(0, commaIndex).toInt();
    int y = moveCommand.substring(commaIndex + 1).toInt();

    Mouse.move(x, y);
}

void HandleButtonChange(uint8_t prevState, uint8_t newState, uint8_t button)
{
    bool prevPressed = CHECK_BIT(prevState, button);
    bool newPressed = CHECK_BIT(newState, button);

    if (prevPressed != newPressed)
    {
        if (newPressed)
        {
            Mouse.press(button);
        } 
        else
        {
            Mouse.release(button);
        }
    }
}
