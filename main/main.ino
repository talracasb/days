#include "build.h"

// We do a little bit of trolling, and use the arduino itself in place of an actual... date module. Just, don't cut the power :)
uint16_t year = (BUILD_YEAR_CH0 - '0') * 1000 + (BUILD_YEAR_CH1 - '0') * 100 + (BUILD_YEAR_CH2 - '0') * 10 + (BUILD_YEAR_CH3 - '0');
uint8_t month = BUILD_MONTH;
uint8_t hour = (BUILD_HOUR_CH0 - '0') * 10 + (BUILD_HOUR_CH1 - '0');
uint8_t minutes = (BUILD_MIN_CH0 - '0') * 10 + (BUILD_MIN_CH1 - '0');
uint8_t seconds = (BUILD_SEC_CH0 - '0') * 10 + (BUILD_SEC_CH1 - '0');
uint8_t day = (BUILD_DAY_CH0 - '0') * 10 + (BUILD_DAY_CH1 - '0');

// In order:           a, b, c, d, e, f, g
uint8_t segments[] = { 8, 9, 6, 5, 4, 3, 2 };

struct Day {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t asb_day;
};

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segments[i], OUTPUT);
  }
  Serial.begin(9600);
}

void turnOff() {
  for (int i = 0; i < 8; i++) {
    pinMode(segments[i], LOW);
  }
}

void displayDigit(int digit) {
  if (digit != 1 && digit != 4)
    digitalWrite(segments[0], HIGH);
  if (digit != 5 && digit != 6)
    digitalWrite(segments[1], HIGH);
  if (digit != 2)
    digitalWrite(segments[2], HIGH);
  if (digit != 1 && digit != 4 && digit != 7)
    digitalWrite(segments[3], HIGH);
  if (digit == 2 || digit == 6 || digit == 8 || digit == 0)
    digitalWrite(segments[4], HIGH);
  if (digit != 1 && digit != 2 && digit != 3 && digit != 7)
    digitalWrite(segments[5], HIGH);
  if (digit != 0 && digit != 1 && digit != 7)
    digitalWrite(segments[6], HIGH);
}

void loop() {
  update_time();

  displayDigit(seconds % 10);
  delay(1);
  turnOff();
}