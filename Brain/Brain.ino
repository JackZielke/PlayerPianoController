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

// Sustain pedal

// The sustain pedal is not something that will be changed on the fly.  It requires a hardware change.
// Set it at compile time, not from the Control Box.

// sustain pedal as key 88?
//  const bool SUSTAIN_KEY = true;
//  const int MAX_NOTE = 87
//  const int MAX_NOTE_PITCH = 107

// sustain pedal as pin 13?
const bool SUSTAIN_KEY = false;
const int MAX_NOTE = 88;
const int MAX_NOTE_PITCH = 108;

#define LED_BUILTIN 2

void resetAll() {
	// turn midi off during reset to prevent errors
	acceptMidi = false;

	for (int noteIndex = 0; noteIndex < MAX_NOTE; noteIndex++) {
		notes[noteIndex].resetSchedule();
    delay(1);
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
	const int SUSTAIN_PIN = 13;

	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(38400);

	//create sustain PWM output. this can't be done by the Pro Micro because the shift registers are filled up
	if (!SUSTAIN_KEY) {
		ledcSetup(0, 100, 8);
		ledcAttachPin(SUSTAIN_PIN, 0);
	}

	initializeSettings();
	if (Setting::enableBLE)
		initializeBluetooth();
	if (Setting::enableWifi)
		initializeWifi();
}

void loop() {
	checkForSerial();
	if (Setting::enableWifi)
		loopWifi();

	for (int noteIndex = 0; noteIndex < MAX_NOTE; noteIndex++) {
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
