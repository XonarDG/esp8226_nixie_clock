#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>

//declaring optocoupler outputs
#define TH 9
#define H 10
#define TM 11
#define M 12

//declaring 74141's pins
#define A 5
#define B 6
#define C 7
#define D 8

#define colon 14

//#define FAST 4
//#define SLOW 3
//#define VERYSLOW 2

//declaring the RTC
RTC_DS3231 rtc;

bool AllNumbersSwitch = true;
int unix_time_plus = 0;
int multiplex_counter = 0;
int multiplex_timing = 0;

void setup() 
{

  //Serial.begin(9600);
  rtc.begin();

  pinMode(TH, OUTPUT);
  pinMode(H, OUTPUT);
  pinMode(TM, OUTPUT);
  pinMode(M, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(colon, OUTPUT);

  digitalWrite(TH, LOW);
  digitalWrite(H, LOW);
  digitalWrite(TM, LOW);
  digitalWrite(M, LOW);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(colon, LOW);

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}
void WriteNumber(int number)
{

  switch (number) {
    case 0:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
    case 9:
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      break;
    case 8:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      break;
    case 7:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
    case 6:
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
    case 5:
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
    case 4:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
    case 3:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
    case 2:
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
    case 1:
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;

  }
}
void MultiPlex(int first_number, int second_number, int third_number, int forth_number)
{

  switch(multiplex_counter)
  {
    case 0:
    digitalWrite(TH, HIGH);
    WriteNumber(first_number);
    multiplex_counter++;
    delay(multiplex_timing);
    digitalWrite(TH, LOW);
    break;
    case 1:
    digitalWrite(H, HIGH);
    WriteNumber(second_number);
    multiplex_counter++;
    delay(multiplex_timing);
    digitalWrite(H, LOW);
    break;
    case 2:
    digitalWrite(TM, HIGH);
    WriteNumber(third_number);
    multiplex_counter++;
    delay(multiplex_timing);
    digitalWrite(TM, LOW);
    break;
    case 3:
    digitalWrite(M, HIGH);
    WriteNumber(forth_number);
    multiplex_counter = 0;
    delay(multiplex_timing);
    digitalWrite(M, LOW);
    break;
  }

}
void AllNumbers(size_t number_of_repeats, size_t digit_display_time_ms, int frequency_in_seconds, int unix_time)
{ 
  if (AllNumbersSwitch == true)
  {
     if (unix_time_plus < unix_time)
    {
      unix_time_plus = unix_time + frequency_in_seconds;
    }
    AllNumbersSwitch = false;
  }

  if (unix_time == unix_time_plus)
  {
    for (size_t i = 0; i < number_of_repeats; i++)
    {
      for (size_t number = 0; number < 9; number++)
      { 
        for (size_t i = 0 ; i < (digit_display_time_ms / 4); i++)
        {
          MultiPlex(number, number, number, number);
        }
      }
    }
    unix_time_plus = unix_time_plus + frequency_in_seconds; 
  }
}
void Colon(int unix_time)
{

  int second = unix_time; 
  if (second % 2 == 0)
  {
    digitalWrite(colon, HIGH);
  }
  else
  digitalWrite(colon, LOW);  

}
void ShowDate(int day, int month, int year, int currentMinute, int frequency_in_minutes, size_t display_time)
{

  int TenDay = (day / 10) % 10;         
  int Day = (day % 10);
  int TenMonth = (month / 10) % 10;
  int Month = (month % 10);

  for (size_t i = 0; i < 3600; i++)
  {
    MultiPlex(TenDay, Day, TenMonth, Month);
  }

}
void ShowTime(int hour, int minute)
{

  int TenHour = (hour / 10) % 10;         
  int Hour = (hour % 10);
  int TenMinute = (minute / 10) % 10;
  int Minute = (minute % 10);

  MultiPlex(TenHour, Hour, TenMinute, Minute);

}
void loop()
{

  multiplex_timing = 1;

  DateTime time = rtc.now();

  AllNumbers(3, 200, 7, time.minute());
  Colon(time.second());
  ShowTime(time.hour(), time.minute());

}