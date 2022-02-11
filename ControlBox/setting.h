#ifndef SETTING_H
#define SETTING_H

#include <Arduino.h>

#define S_HANDLE_NOTES       0
#define S_SCHEDULE_NOTES     1
#define S_MIN_ACCEPTED_VEL   2
#define S_PWM_PERCENT        3
#define S_MIN_STARTUP_MS     4
#define S_MAX_STARTUP_MS     5
#define S_VELOCITY_VAR       6
#define S_MIN_DEACTIVATE_MS  7
#define S_MAX_DEACTIVATE_MS  8
#define S_FAST_DEACTIVATE_MS 9
#define S_SUSTAIN_ON_MS      10
#define S_SUSTAIN_OFF_MS     11
#define S_NOTE_TIMEOUT_MS    12
#define S_SUSTAIN_TIMEOUT_MS 13
#define S_AUTO_RESET_MS      14
#define S_MAX_LEFT_NOTES     15
#define S_MAX_RIGHT_NOTES    16
#define S_ENABLE_WIFI				 17
#define S_WIFI_AP            18
#define S_ENABLE_BLE         19

bool menuIsBool(int menu);
void changeSetting(int changeBy);

#endif