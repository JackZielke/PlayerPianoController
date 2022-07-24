#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

#define SUSTAIN_DATA_BYTE 64
#define MIN_NOTE_PITCH    21
//#define MAX_NOTE_PITCH    108 // defined in Brain.ino
#define MIN_NOTE_VELOCITY 4 // MUST BE DYNAMIC (Setting::minNoteVelocity)
#define MAX_NOTE_VELOCITY 127
#define RESET_LENGTH      152
#define RESET_LENGTH2     154
#define SUSTAIN_NOTE      87

void initializeBluetooth();

#endif
