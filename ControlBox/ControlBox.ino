#include <EEPROM.h>
#include <Wire.h>
#include <MIDIUSB.h>
#include "LiquidCrystal_I2C.h"
#include "input.h"
#include "lcd.h"
#include "setting.h"
#include "midi.h"
#include "serial.h"

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

extern const bool DEBUG_MODE = false;

void setup() {
  if (DEBUG_MODE) Serial.begin(38400);
  Serial1.begin(38400);

  initializeInputs();
  initializeLCD();
  printHomeScreen();
}

void loop() {
  checkForInput();
  checkSchedule();
  checkForMidiUSB();
}
