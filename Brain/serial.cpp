#include "serial.h"
#include "note.h"
#include "sustain.h"
#include "settings.h"
#include "main.h"

void sendMidiToProMicro(byte note, byte velocity);

//serial is always handled in bytes, not uint8_t because serial is dumb
void checkForSerial() {
	while(Serial.available() > 2) {
		uint8_t header = Serial.read();

		if (header >= NOTE_HEADER && header <= VOLUME_HEADER) { //make sure the first byte is a header
			uint8_t byte1 = Serial.read(); //only declare these if the first byte is a header
			uint8_t byte2 = Serial.read(); //otherwise the program will keep looping looking for one

			if (DEBUG_MODE) Serial.printf("Received serial header: %d, data1: %d, data2: %d\n", header, byte1, byte2); 

			switch (header) {
				case NOTE_HEADER:
					if(byte1 >= 0 && byte1 < 87 &&
						(byte2 >= Setting::minNoteVelocity || byte2 == 0) && byte2 <= 127)
						notes[byte1].prepareToSchedule(byte2);
					break;

				case SUSTAIN_HEADER:
					sustain.prepareToSchedule(byte2);
					break;

				case SETTING_HEADER:
					updateSetting(static_cast<SettingID::SettingID>(byte1), byte2);
					break;

				case RESET_HEADER:
					resetAll();
					break;

				case VOLUME_HEADER:
					setVolume(byte2);
					break;
			}
		}
	}
}

void sendMidiToProMicro(byte note, byte velocity)
{
	//invert notes to be compatible with shift registers
	byte compatibleNote = (note * -1) + 87;
	byte message[3] ={ NOTE_HEADER, compatibleNote, velocity };
	//note that velocity is conformed on Pro Micro
	Serial.write(message, 3);
	//Serial.write(END_HEADER);
}

void customSerialToProMicro(byte header, byte byte1, byte byte2)
{
	Serial.write(header);
	Serial.write(byte1);
	Serial.write(byte2);
}
