int days_in_month(int month, int year) {
  switch (month) {
    case 1:   // January
    case 3:   // March
    case 5:   // May
    case 7:   // July
    case 8:   // August
    case 10:  // October
    case 12:  // December
      return 31;
    case 4:   // April
    case 6:   // June
    case 9:   // September
    case 11:  // November
      return 30;
    case 2:  // February
      // Leap year check
      if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return 29;
      } else {
        return 28;
      }
    default:
      return -1;  // Error: invalid month
  }
}

unsigned long previousMillis = 0;
const unsigned long interval = 1000;  // 1 second
uint8_t dim = days_in_month(month, year);

void update_time() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis += interval;

    seconds++;

    if (seconds >= 60) {
      seconds = 0;
      minutes++;
    }

    if (minutes >= 60) {
      minutes = 0;
      hour++;
    }

    if (hour >= 24) {
      hour = 0;
      day++;
    }

    if (day > dim) {
      day = 1;
      month++;

      if (month > 12) {
        month = 1;
        year++;
      }

      dim = days_in_month(month, year);
    }

    Serial.print("TIME: ");
    Serial.print(year);
    Serial.print("-");
    Serial.print(month);
    Serial.print("-");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.println();
  }

  displayDigit(seconds % 10);
  delay(1);
  turnOff();
}