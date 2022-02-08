#include <BLEMidi.h>
#include "bluetooth.h"
#include "main.h"
#include "sustain.h"
#include "settings.h"
#include "note.h"

void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  extern bool acceptMidi;
  // extern uint8_t Setting::minNoteVelocity;
  if(acceptMidi && note <= MAX_NOTE_PITCH && note >= MIN_NOTE_PITCH && velocity > Setting::minNoteVelocity) {
    notes[note - MIN_NOTE_PITCH].prepareToSchedule(velocity);
  }
  if(DEBUG_MODE) Serial.printf("[BLE] Received note on : channel %d, note %d, velocity %d (timestamp %dms)\n", channel, note, velocity, timestamp);
}

void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  extern bool acceptMidi;
  if(acceptMidi && note <= MAX_NOTE_PITCH && note >= MIN_NOTE_PITCH) {
    notes[note - MIN_NOTE_PITCH].prepareToSchedule(0);
  }
  if(DEBUG_MODE) Serial.printf("[BLE] Received note off : channel %d, note %d, velocity %d (timestamp %dms)\n", channel, note, velocity, timestamp);
}

void onControlChange(uint8_t channel, uint8_t controller, uint8_t value, uint16_t timestamp) {
  extern bool acceptMidi;

  if (controller == 123 || controller == 120) { // reset/note-off signal
    resetAll();
  }

  if (acceptMidi && controller == SUSTAIN_DATA_BYTE) {
    sustain.prepareToSchedule(value);
  }

  if(DEBUG_MODE) Serial.printf("[BLE] Received control change : channel %d, controller %d, value %d (timestamp %dms)\n", channel, controller, value, timestamp);
}

void initializeBluetooth() {
  BLEMidiServer.begin("VikPlayerPiano");
  if(DEBUG_MODE) BLEMidiServer.setOnConnectCallback([](){ Serial.println("[BLE] Connected"); });
  if(DEBUG_MODE) BLEMidiServer.setOnDisconnectCallback([](){ Serial.println("[BLE] Disconnected"); });
  BLEMidiServer.setNoteOnCallback(onNoteOn);
  BLEMidiServer.setNoteOffCallback(onNoteOff);
  BLEMidiServer.setControlChangeCallback(onControlChange);

  if(DEBUG_MODE) Serial.println("[BLE] Waiting for connections...");
  //BLEMidiServer.enableDebugging(); // moar debug
}
