#include "lcd.h"

void sendSerialToMain(byte header, byte setting, byte value) {
  extern const bool DEBUG_MODE;
  if (DEBUG_MODE) {
    Serial.print("Sending setting: header ");
    Serial.print(header);
    Serial.print(" setting ");
    Serial.print(setting);
    Serial.print(" value ");
    Serial.print(value);
    Serial.println();
  }
  Serial1.write(header);
  Serial1.write(setting);
  Serial1.write(value);
}
