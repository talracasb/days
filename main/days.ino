#include "calendar.h"

void split44(uint8_t byte, uint8_t* high, uint8_t* low) {
    *high = byte >> 4;
    *low = byte & 0x0F;
}

void split53(uint8_t byte, uint8_t* high, uint8_t* low) {
    *high = byte >> 3;
    *low = byte & 0x07;
}

uint8_t get_asb_day(uint8_t i_day, uint8_t i_month, uint8_t i_year_diff) {
  for (uint32_t i = 0; i < sizeof(data); i++) {
    uint8_t p_year_diff;
    uint8_t p_month;
    uint8_t p_day;
    uint8_t p_asb_day;

    split44(data[i], &p_year_diff, &p_month);
    split53(data[i+1], &p_day, &p_asb_day);

    if (p_day == i_day && p_month == i_month && p_year_diff == i_year_diff) {
      return p_asb_day;
    }
  }

  return 0;
}