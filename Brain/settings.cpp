#include <EEPROM.h>
#include "settings.h"
#include "settings.h"
#include "note.h"
#include "serial.h"

//settings within the program
int           fullDelay  = 0;
bool          acceptMidi = false;
unsigned long nextReset  = 0; //first reset will happen immediately

// settings changed by the control box
namespace Setting {
  bool    handleNotes      = true;
  bool    scheduleNotes    = true;
  int     minNoteVelocity  = 4;
  // forwarded pwm percent
  int     minStartupMs     = 18;
  int     maxStartupMs     = 18;
  int     velocityVar      = 35;
  int     minDeactivateMs  = 75;
  int     maxDeactivateMs  = 80;
  int     fastDeactivateMs = 52;
  int     sustainOnMs      = 91;
  int     sustainOffMs     = 50;
  int     noteTimeoutMs    = 10000;
  int     sustainTimeoutMs = 30000;
  int     autoResetMs      = 360000;
  int     maxLeftNotes     = 10;
  int     maxRightNotes    = 10;
  bool    enableWifi       = true;
  bool    wifiAp           = false;
  bool    enableBLE        = true;
  int     volume           = 100;
}

void setVolume(int newVolume) {
  Setting::volume = newVolume;
}

void initializeSettings() {
  using namespace Setting;
  const int defaults[] = {
     1,   1,  4,  0, 18, 18, 35, 75, 80, 52,
     91, 50, 10, 30,  6, 10, 10,  1,  0,  1,
     100, 0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  42
  };

  EEPROM.begin(32);
  // init settings if needed
  if (EEPROM.read(31) != 42) {
    for (int i = 0; i < 32; i++) {
      EEPROM.write(i, defaults[i]);
    }
    EEPROM.commit();
  }

  handleNotes      = EEPROM.read(0);
  scheduleNotes    = EEPROM.read(1);
  minNoteVelocity  = EEPROM.read(2);
  // we do not store PWM_PERCENT
  minStartupMs     = EEPROM.read(4);
  maxStartupMs     = EEPROM.read(5);
  velocityVar      = EEPROM.read(6);
  minDeactivateMs  = EEPROM.read(7);
  maxDeactivateMs  = EEPROM.read(8);
  fastDeactivateMs = EEPROM.read(9);
  sustainOnMs      = EEPROM.read(10);
  sustainOffMs     = EEPROM.read(11);
  noteTimeoutMs    = EEPROM.read(12) * 1000;
  sustainTimeoutMs = EEPROM.read(13) * 1000;
  autoResetMs      = EEPROM.read(14) * 60000;
  maxLeftNotes     = EEPROM.read(15);
  maxRightNotes    = EEPROM.read(16);
  enableWifi       = EEPROM.read(17);
  wifiAp           = EEPROM.read(18);
  enableBLE        = EEPROM.read(19);
  volume           = 100;
}


void updateSetting(SettingID::SettingID setting, int value) {
  using namespace SettingID;
  using namespace Setting;

  switch (setting) {
    case HANDLE_NOTES:
      handleNotes = value;
      break;
    case SCHEDULE_NOTES:
      scheduleNotes = value;
      break;
    case MIN_ACCEPTED_VEL:
      minNoteVelocity = value;
      break;
    case PWM_PERCENT:
      //only setting passed to pro micro
      customSerialToProMicro(SETTING_HEADER, value, value);
      return;
      break;
    case MIN_STARTUP_MS:
      minStartupMs = value;
      msReset();
      break;
    case MAX_STARTUP_MS:
      maxStartupMs = value;
      msReset();
      break;
    case VELOCITY_VAR:
      velocityVar = value;
      msReset();
      break;
    case MIN_DEACTIVATE_MS:
      minDeactivateMs = value;
      msReset();
      break;
    case MAX_DEACTIVATE_MS:
      maxDeactivateMs = value;
      msReset();
      break;
    case FAST_DEACTIVATE_MS:
      fastDeactivateMs = value;
      break;
    case SUSTAIN_ON_MS:
      sustainOnMs = value;
      break;
    case SUSTAIN_OFF_MS:
      sustainOffMs = value;
      break;
    case NOTE_TIMEOUT_MS:
      noteTimeoutMs = value * 1000;
      break;
    case SUSTAIN_TIMEOUT_MS:
      sustainTimeoutMs = value * 1000;
      break;
    case AUTO_RESET_MS:
      autoResetMs = value * 60000;
      break;
    case MAX_LEFT_NOTES:
      maxLeftNotes = value;
      break;
    case MAX_RIGHT_NOTES:
      maxRightNotes = value;
      break;
    case ENABLE_WIFI:
      enableWifi = value;
      break;
    case WIFI_AP:
      wifiAp = value;
      break;
    case ENABLE_BLE:
      enableBLE = value;
      break;
    default:
      return;
      break;
  }
  EEPROM.write(setting, value);
  EEPROM.commit();
}

void msReset() {
  using namespace Setting;
  for (int noteIndex = 0; noteIndex < 88; noteIndex++) {
    // calculate the ms for each key linearly. Calculate this now to process less later
    notes[noteIndex].setStartupMs((noteIndex * (minStartupMs - maxStartupMs) / 88) + maxStartupMs);
    notes[noteIndex].setDeactivateMs((noteIndex * (maxDeactivateMs - minDeactivateMs) / 88) + minDeactivateMs);
  }

  for (int velocityIndex = 0; velocityIndex < 127; velocityIndex++) {
    // function created through graphing velocity times and creating a function that best fit
    Note::setNoteVelocityMs(velocityIndex, round(((-25 * velocityIndex) / (double)127) + velocityVar));
  }

  //calculate the total maximum time for a note cycle as a reference when scheduling keys
  fullDelay = maxStartupMs + Note::getNoteVelocityMs(0) + maxDeactivateMs;
}
