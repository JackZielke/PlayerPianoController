#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>

#define NOTE_HEADER    201
#define SUSTAIN_HEADER 202
#define SETTING_HEADER 203
#define RESET_HEADER   204
#define VOLUME_HEADER  205
#define END_HEADER     206

void checkForSerial();
void sendMidiToProMicro(byte note, byte velocity);
void customSerialToProMicro(byte header, byte note, byte velocity);

#endif