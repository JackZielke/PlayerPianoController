#include <dummy.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "sustain.h"
#include "settings.h"
#include "serial.h"
#include "note.h"
#include "bluetooth.h"
#include "wifihandle.h"
#include "main.h"
#pragma GCC diagnostic pop

const bool DEBUG_MODE = false;

#define LED_BUILTIN 2

void resetAll() {
	// turn midi off during reset to prevent errors
	acceptMidi = false;

	for (int noteIndex = 0; noteIndex < 87; noteIndex++) {
		notes[noteIndex].resetSchedule();
	}
	Note::resetInstances();
	sustain.resetSchedule();
	msReset();
	acceptMidi = true;
}

void flashLED() {
	digitalWrite(LED_BUILTIN, HIGH);
	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
	delay(100);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
	delay(100);
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(38400);

	// wait for settings from pro micro
	// todo: move settings to flash/eeprom
	while (millis() < 10000) {
		checkForSerial();
	}

	initializeBluetooth();
  initializeWifi();
	// We use note88 (last note on the piano) for sustain pedal output
}

void loop() {
	checkForSerial();
	loopWifi();

	for (int noteIndex = 0; noteIndex < 87; noteIndex++) {
		notes[noteIndex].checkSchedule();
		notes[noteIndex].checkForErrors();
	}

	sustain.checkSchedule();
	sustain.checkForErrors();

	if (millis() >= nextReset) {
		//first reset will happen immediately and midi will begin to accept after this
		resetAll();
		nextReset = millis() + Setting::autoResetMs;
	}
}
