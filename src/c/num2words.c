#include <pebble.h>
#include "num2words.h"
#include <string.h>
#include <stdio.h>

/* Days of the week in Italian Language */
static const char* const DAYS[] = {
  "domenica",
  "lunedi",
  "martedi'",
  "mercoledi",
  "giovedi'",
  "venerdi",
  "sabato"
};

/* 
   This function converts days of the week to napolitan words.
*/
void days_to_common_words(int days, char *words) {

  strcpy(words, "");
  /* add the corresponding Italian word */
  strcat(words, DAYS[days]);
}

/* 
   This function converts date to Italian format DD-MM-YYYY.
*/
void date_to_string(int year, int month, int num_day, char *words) {
  char year_char[5];
  char month_char[3]; 
  char num_day_char[3];  
  
  strcpy(words, "");

  snprintf(year_char,   sizeof(year_char),      "%d", year+1900);
  snprintf(month_char,  sizeof(month_char),     "%d", month+1);
  snprintf(num_day_char,sizeof(num_day_char),   "%d", num_day);
  
  strcat(words, num_day_char);  
  strcat(words, "-"); 
  strcat(words, month_char);  
  strcat(words, "-"); 
  strcat(words, year_char);
}