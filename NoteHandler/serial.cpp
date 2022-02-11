#include "serial.h"
#include "shiftRegister.h"

extern const bool DEBUG_MODE;
extern const uint8_t SUSTAIN_HEADER = 202;

void checkForSerial()
{
  const byte NOTE_HEADER = 201;
  const byte SETTING_HEADER = 203;
  while(Serial1.available() > 2) {
    int header = Serial1.read();

    if (header == NOTE_HEADER) {
      int note     = Serial1.read();
      int velocity = Serial1.read();
      activateNote(note, velocity);
      if (DEBUG_MODE) {
        Serial.print("note header: ");
        Serial.print(header);
        Serial.print(" note: ");
        Serial.print(note);
        Serial.print(" vel: ");
        Serial.println(velocity);
      }

    } else if (header == SETTING_HEADER) {
      uint8_t value1 = Serial1.read();
      uint8_t value2 = Serial1.read();
      if (DEBUG_MODE) {
        Serial.print("setting header: ");
        Serial.print(header);
        Serial.print(" val1: ");
        Serial.print(value1);
        Serial.print(" val2: ");
        Serial.println(value2);
      }
      //Pro Micro knows that only one setting is being sent
      pwmPercent = value2;
    }
  }
}

void sendSerialToMain(byte header, byte setting, byte value)
{
  Serial1.print(header);
  Serial1.print(setting);
  Serial1.print(value);
}

void sendSerialToUSB(String* message, int lengthOfMessage)
{
  for(int index = 0; index < lengthOfMessage; index++)
    Serial.println(message[index]);
}
