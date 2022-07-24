#include <AppleMIDI.h>
#include "WiFi.h"
#include "wifihandle.h"
#include "main.h"
#include "sustain.h"
#include "settings.h"
#include "note.h"

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, MIDI, "VikPlayerPiano", DEFAULT_CONTROL_PORT);

void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
  extern bool acceptMidi;
  if(acceptMidi && note <= MAX_NOTE_PITCH && note >= MIN_NOTE_PITCH && velocity > Setting::minNoteVelocity) {
    notes[note - MIN_NOTE_PITCH].prepareToSchedule(velocity);
  }
  if(DEBUG_MODE) Serial.printf("[WIFI] Received note on : channel %d, note %d, velocity %d\n", channel, note, velocity);
}

void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
  extern bool acceptMidi;
  if(acceptMidi && note <= MAX_NOTE_PITCH && note >= MIN_NOTE_PITCH) {
    notes[note - MIN_NOTE_PITCH].prepareToSchedule(0);
  }
  if(DEBUG_MODE) Serial.printf("[WIFI] Received note off : channel %d, note %d, velocity %d\n", channel, note, velocity);
}

void onControlChange(uint8_t channel, uint8_t controller, uint8_t value) {
  extern bool acceptMidi;

  if (controller == 120 || controller == 123) { // reset/note-off signal
    resetAll();
  }

  if (acceptMidi && controller == SUSTAIN_DATA_BYTE) {
    sustain.prepareToSchedule(value);
  }

  if(DEBUG_MODE) Serial.printf("[WIFI] Received control change : channel %d, controller %d, value %d\n", channel, controller, value);
}

void onSystemReset() {
  if(DEBUG_MODE) Serial.printf("[WIFI] Received system reset\n");
  resetAll();
}

void loopWifi() {
  MIDI.read();
}

void WiFiEventDebug(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("[WIFI] Connected to AP");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("[WIFI] Disconnected from AP");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("[WIFI] Client connected");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("[WIFI] Client disconnected");
      break;
  }
}

void WiFiReset() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void initializeWifi() {
  uint8_t retry;

  WiFiReset();

  WiFi.setHostname("VikPlayerPiano");
  if (Setting::wifiAp == 0) {
    WiFi.begin(ssid, password);
  }

  if(DEBUG_MODE) {
    Serial.println("[WIFI] Started WIFI");
    WiFi.onEvent(WiFiEventDebug);
  }

  // try to connect to defined wifi, if failure, fallback to create a SoftAP
  if (Setting::wifiAp == 0) {
    retry = 10;
    while (WiFi.status() != WL_CONNECTED && retry > 0) {
      delay(500);
      retry--;
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    if(DEBUG_MODE) Serial.println("[WIFI] Failed to connect to SSID, fallback to create a SoftAP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("VikPlayerPiano", "pianorules");
  } else {
    if(DEBUG_MODE) Serial.println("[WIFI] Connected to defined SSID in DHCP mode");
  }

  MIDI.begin();
  MIDI.setHandleNoteOn(onNoteOn);
  MIDI.setHandleNoteOff(onNoteOff);
  MIDI.setHandleControlChange(onControlChange);
  MIDI.setHandleSystemReset(onSystemReset);

  if(DEBUG_MODE) Serial.println("[WIFI] Waiting for connections...");
  //BLEMidiServer.enableDebugging(); // moar debug
}
