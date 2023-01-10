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

#define COLON_BOTTOM 14
#define COLON_TOP 15

//#define FAST 4
//#define SLOW 3
//#define VERYSLOW 2

//declaring the RTC
RTC_DS3231 rtc;

//allnumber variables
bool all_number_switch = true;
int next_unix_time = 0;
int all_number_interval = 10;
int i = 0;

//multiplex variables
int unix_time_plus = 0;
int multiplex_counter = 0;
const int multiplex_timing = 2;

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
  pinMode(COLON_BOTTOM, OUTPUT);
  pinMode(COLON_TOP, OUTPUT);

  digitalWrite(TH, LOW);
  digitalWrite(H, LOW);
  digitalWrite(TM, LOW);
  digitalWrite(M, LOW);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(COLON_BOTTOM, LOW);
  digitalWrite(COLON_TOP, LOW);

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
    WriteNumber(first_number);
    digitalWrite(TH, HIGH);
    multiplex_counter++;
    delay(multiplex_timing);
    digitalWrite(TH, LOW);
    break;
    case 1:
    WriteNumber(second_number);
    digitalWrite(H, HIGH);
    multiplex_counter++;
    delay(multiplex_timing);
    digitalWrite(H, LOW);
    break;
    case 2:
    WriteNumber(third_number);
    digitalWrite(TM, HIGH);
    multiplex_counter++;
    delay(multiplex_timing);
    digitalWrite(TM, LOW);
    break;
    case 3:
    WriteNumber(forth_number);
    digitalWrite(M, HIGH);
    multiplex_counter = 0;
    delay(multiplex_timing);
    digitalWrite(M, LOW);
    break;
  }

}
void AllNumbers()
{ 
  
  MultiPlex(1, 1, 1, 1);
  MultiPlex(2, 2, 2, 2);
  MultiPlex(3, 3, 3, 3);
  MultiPlex(4, 4, 4, 4);
  MultiPlex(5, 5, 5, 5);
  MultiPlex(6, 6, 6, 6);
  MultiPlex(7, 7, 7, 7);
  MultiPlex(8, 8, 8, 8);
  MultiPlex(9, 9, 9, 9);
  MultiPlex(0, 0, 0, 0);
  i++;
}
void Colon(int second)
{
  
  if (second % 2 == 0)
  {
    digitalWrite(COLON_BOTTOM, HIGH);
    digitalWrite(COLON_TOP, HIGH);
  }
  else
  {
    digitalWrite(COLON_BOTTOM, LOW);
    digitalWrite(COLON_TOP, LOW);  
  }

}
void ShowDate(int day, int month, int year, int unix_time)

{

  int TenDay = (day / 10) % 10;         
  int Day = (day % 10);
  int TenMonth = (month / 10) % 10;
  int Month = (month % 10);

  MultiPlex(TenDay, Day, TenMonth, Month);

}
void ShowTime(int hour, int minute)
{

  int TenHour = (hour / 10) % 10;         
  int Hour = (hour % 10);
  int TenMinute = (minute / 10) % 10;
  int Minute = (minute % 10);

  MultiPlex(TenHour, Hour, TenMinute, Minute);

}
bool IntervalCheck(int current_unix_time, int next_unix_time)
{
  if (next_unix_time < current_unix_time)
  {
    return true;
  }
  else
  {
    return false;
  }
}
void loop()
{
  DateTime time = rtc.now();

  if (all_number_switch == true)
  {
    next_unix_time = time.unixtime() + all_number_interval;
    all_number_switch = false;
  }
  

  //AllNumbers();
  Colon(time.second());
  //ShowTime(time.hour(), time.minute());
  //ShowDate(time.day(), time.month(), time.year(), time.unixtime());

  if (IntervalCheck(time.unixtime(), next_unix_time) != true)
  {
    ShowTime(time.hour(), time.minute());
  }
  else
  {
    if (i < 400)
    {
      AllNumbers();
    }
    else
    {
      next_unix_time = time.unixtime() + all_number_interval;
    }
  }
  

}
