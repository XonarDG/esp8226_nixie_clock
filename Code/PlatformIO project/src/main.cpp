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
int MinutesPlus = 0;
int MultiPlexCounter = 0;
int Space = 0;

bool Switch = true;

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
void AllNumbers(size_t number_of_repeats, size_t digit_display_time_ms, int frequency_in_minutes, int currentMinute)
{ 
  int MultiPlexCounter = 0;

  /*int setfrequency = currentMinute % frequency_in_minutes;
  if (setfrequency == 0)
  {
    for (size_t i = 0; i < number_of_repeats; i++)
    {
      for (size_t number = 0; number < 9; number++)
      { 
        for (size_t i = 0 ; i < Delay; i++)
        {
          switch(MultiPlexCounter)
          {
            case 0:
            digitalWrite(TH, HIGH);
            WriteNumber(number);
            MultiPlexCounter++;
            delay(Space);
            digitalWrite(TH, LOW);
            break;
            case 1:
            digitalWrite(H, HIGH);
            WriteNumber(number);
            MultiPlexCounter++;
            delay(Space);
            digitalWrite(H, LOW);
            break;
            case 2:
            digitalWrite(TM, HIGH);
            WriteNumber(number);
            MultiPlexCounter++;
            delay(Space);
            digitalWrite(TM, LOW);
            break;
            case 3:
            digitalWrite(M, HIGH);
            WriteNumber(number);
            MultiPlexCounter = 0;
            delay(Space);
            digitalWrite(M, LOW);
            break;
          }
        }
      }
    }
  }*/  
  
  if (AllNumbersSwitch == true)
  {
     if (MinutesPlus < currentMinute)
    {
      MinutesPlus = currentMinute + frequency_in_minutes;
    }
    AllNumbersSwitch = false;
  }

  if (currentMinute == MinutesPlus)
  {
    for (size_t i = 0; i < number_of_repeats; i++)
    {
      for (size_t number = 0; number < 9; number++)
      { 
        for (size_t i = 0 ; i < (digit_display_time_ms / 4); i++)
        {
          switch(MultiPlexCounter)
          {
            case 0:
            digitalWrite(TH, HIGH);
            WriteNumber(number);
            MultiPlexCounter++;
            delay(Space);
            digitalWrite(TH, LOW);
            break;
            case 1:
            digitalWrite(H, HIGH);
            WriteNumber(number);
            MultiPlexCounter++;
            delay(Space);
            digitalWrite(H, LOW);
            break;
            case 2:
            digitalWrite(TM, HIGH);
            WriteNumber(number);
            MultiPlexCounter++;
            delay(Space);
            digitalWrite(TM, LOW);
            break;
            case 3:
            digitalWrite(M, HIGH);
            WriteNumber(number);
            MultiPlexCounter = 0;
            delay(Space);
            digitalWrite(M, LOW);
            break;
          }
        }
      }
    }
    MinutesPlus = MinutesPlus + frequency_in_minutes; 
  }
    if (MinutesPlus >= 60)
  {
    MinutesPlus = MinutesPlus - 60;
  }
}

void Colon(int currentSecond)

{
  int second = currentSecond; 
  if (second % 2 == 0)
  {
    digitalWrite(colon, HIGH);
  }
  else
  digitalWrite(colon, LOW);  
}

void ShowDate(int day, int month, int year, int currentMinute, int frequency_in_minutes, size_t display_time)
{

MultiPlexCounter = 0;

int TenDay = (day / 10) % 10;         
int Day = (day % 10);
int TenMonth = (month / 10) % 10;
int Month = (month % 10);
 for (size_t i = 0; i < 3600; i++)
 {
  switch(MultiPlexCounter)
  {
    case 0:
    digitalWrite(TH, HIGH);
    WriteNumber(TenDay);
    MultiPlexCounter++;
    delay(Space);
    digitalWrite(TH, LOW);
    break;
    case 1:
    digitalWrite(H, HIGH);
    WriteNumber(Day);
    MultiPlexCounter++;
    delay(Space);
    digitalWrite(H, LOW);
    break;
    case 2:
    digitalWrite(TM, HIGH);
    WriteNumber(TenMonth);
    MultiPlexCounter++;
    delay(Space);
    digitalWrite(TM, LOW);
    break;
    case 3:
    digitalWrite(M, HIGH);
    WriteNumber(Month);
    MultiPlexCounter = 0;
    delay(Space);
    digitalWrite(M, LOW);
    break;
    }
 }
 
  
  }

void loop()
{

  Space = 1;

  DateTime time = rtc.now();

  int TenHour = 0;
  int Hour = 0;
  int TenMinute = 0;
  int Minute = 0;

  TenHour = (time.hour() / 10) % 10;         
  Hour = (time.hour() % 10);
  TenMinute = (time.minute() / 10) % 10;
  Minute = (time.minute() % 10);

  /*Serial.print(TenHour);
  Serial.print(Hour);
  Serial.print(TenMinute);
  Serial.print(Minute);
  Serial.println();*/
    if (Switch == true)
  {
    ShowDate(time.day(), time.month(), time.year(), time.minute(), 30, 300);
    Switch = false;
  }
  AllNumbers(3, 200, 7, time.minute());
  Colon(time.second());

  switch(MultiPlexCounter)
  {
    case 0:
    digitalWrite(TH, HIGH);
    WriteNumber(TenHour);
    MultiPlexCounter++;
    delay(Space);
    digitalWrite(TH, LOW);
    break;
    case 1:
    digitalWrite(H, HIGH);
    WriteNumber(Hour);
    MultiPlexCounter++;
    delay(Space);
    digitalWrite(H, LOW);
    break;
    case 2:
    digitalWrite(TM, HIGH);
    WriteNumber(TenMinute);
    MultiPlexCounter++;
    delay(Space);
    digitalWrite(TM, LOW);
    break;
    case 3:
    digitalWrite(M, HIGH);
    WriteNumber(Minute);
    MultiPlexCounter = 0;
    delay(Space);
    digitalWrite(M, LOW);
    break;
  }
  //AllNumbers(10, 10);
}