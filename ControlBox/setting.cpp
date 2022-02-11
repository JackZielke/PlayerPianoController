#include <EEPROM.h>
#include "setting.h"
#include "lcd.h"
#include "serial.h"
#include "input.h"

bool menuIsBool(int menu) {
  if (menu == S_SCHEDULE_NOTES ||
      menu == S_HANDLE_NOTES ||
      menu == S_ENABLE_WIFI ||
      menu == S_WIFI_AP ||
      menu == S_ENABLE_BLE) {
    return true;
  }
  return false;
}

void changeSetting(int changeBy) {
  if (menuIsBool(currentMenu))
    EEPROM.write(currentMenu, !(static_cast<bool>(EEPROM.read(currentMenu))));
  else
    EEPROM.write(currentMenu, EEPROM.read(currentMenu) + changeBy);
}
