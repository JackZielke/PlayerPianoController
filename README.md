# Player Piano Controller
Arduino/esp32 code for making a [DIY Player Piano](https://www.youtube.com/watch?v=S7Bd992k368).  
This is a fork from the [original project](https://github.com/bbswitzer/PianoProject), with some improvements:
- usage of ESP32-BLE-MIDI library (improving compatibility with windows / apple / android)
- adding wifi MIDI capability
- various bugfixes
- last key of piano (88) is used for sustain pedal instead of more electronics connected to ESP32
- schemas and solenoid board PCB that I created for making this project

Original project informations can be [found here](https://brandonswitzer.squarespace.com/player-piano).

Boards are connected using chained serial:  
`control-box TX -> RX brain TX -> RX note-handler`

##### Control Box (Pro Micro):
- Interface for changing settings
- Reset keys button (resets solenoids)
- Receives MIDI data from USB and sends it to ESP32

##### Brain (ESP32):
- Receives note data from Control Box
- Receives and decodes MIDI messages (BLE and/or WIFI)
- Schedules notes and sustain
- Send notes to the note handler

##### Note handler (Pro Micro):
- Receives note data from ESP32 
- Receives MIDI data from USB
- Activates notes using shift registers

##### Used Arduino Libraries:
- [Arduino Core for ESP32](https://github.com/espressif/arduino-esp32)
- [MIDIUSB](https://github.com/arduino-libraries/MIDIUSB) (Note handler & Control Box)
- [ShiftPWM](https://raw.githubusercontent.com/elcojacobs/ShiftPWM) (Note handler) (Already in project folder)
- [LiquidCrystal_I2C](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library) (Control Box) (Already in project folder)
- [ESP32-BLE-MIDI](https://github.com/max22-/ESP32-BLE-MIDI) (ESP32) (for bluetooth midi receiving)
- [Arduino AppleMIDI Library](https://github.com/lathoub/Arduino-AppleMIDI-Library/) (ESP32) (for wifi midi receiving from win/osx/android)
