#include <EEPROM.h>
#include "LiquidCrystal_I2C.h"
#include "lcd.h"
#include "input.h"
#include "setting.h"

extern LiquidCrystal_I2C lcd;

const String MENU_NAMES[] = {
  "Handle Notes",
  "Schedule Notes",
  "Min Accepted Vel",
  "Solenoid PWM %",
  "Min Startup Ms",
  "Max Startup Ms",
  "Velocity Var",
  "Min Off Ms",
  "Max Off Ms",
  "Fast Off Ms",
  "Pedal On Ms",
  "Pedal Off Ms",
  "Note Timeout",
  "Sustain Timeout",
  "Auto Reset Mins",
  "Max Left Keys",
  "Max Right Keys",
  "Enable Wifi",
  "Force wifi AP",
  "Enable BLE"
};

int              menuState = MS_WELCOME;
int              currentMenu = -1; //initialize current menu as invalid
const int        NUM_OF_MENUS         = sizeof(MENU_NAMES) / sizeof(MENU_NAMES[0]);
const int        SETTING_MENU_TIMEOUT = 20000;
extern const int SPECIAL_MENU_TIMEOUT = 2000;
unsigned long    exitScreen;

void initializeLCD() {
  uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
  lcd.begin();
  lcd.backlight();
  lcd.createChar(1, note);
}

void printHomeScreen() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Welcome to vik");
  lcd.setCursor(2, 1);
  lcd.print("Player Piano");
  lcd.setCursor(0, 1);
  lcd.write(1);
  lcd.setCursor(15, 1);
  lcd.write(1);
}

void updateDisplay() {
  lcd.clear();
  switch (menuState) {
    case MS_WELCOME:
      printHomeScreen();
      break;

    case MS_SETTINGS:
      lcd.print(MENU_NAMES[currentMenu]);
      lcd.setCursor(0, 1);
      if (menuIsBool(currentMenu)) {
        if (EEPROM.read(currentMenu)) {
          lcd.print("Yes");
        } else {
          lcd.print("No");
        }
      } else {
        lcd.print(EEPROM.read(currentMenu));
      }
      break;

    case MS_VOLUME:
      lcd.setCursor(0, 0);
      lcd.print("Volume");
      lcd.print("          ");
      lcd.setCursor(0, 1);
      lcd.print(lastAnalog);
      lcd.print("%            ");
      break;

    case MS_RESET:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Resetting Keys");
      lcd.setCursor(0, 1);
      lcd.print("...");
      break;
  }
}

void checkSchedule() {
  unsigned long ms = millis();

  if (ms >= lastPressedOverall + SETTING_MENU_TIMEOUT && lastPressedOverall > 0) {
    lastPressedOverall = 0;
    // if another screen isn't scheduled
    if (menuState == MS_SETTINGS) {
      menuState = MS_WELCOME;
      updateDisplay();
    }
  }

  if (ms >= exitScreen && exitScreen > 0) {
    exitScreen = 0;
    // if setting menu is timed out
    if (lastPressedOverall == 0)
      menuState = MS_WELCOME;
    else
      menuState = MS_SETTINGS;
    updateDisplay();
  }
}
