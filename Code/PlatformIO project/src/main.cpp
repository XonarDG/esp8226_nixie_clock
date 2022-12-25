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

//#define FAST 4
//#define SLOW 3
//#define VERYSLOW 2

//declaring the RTC
RTC_DS3231 rtc;

int MultiPlexCounter = 0;
int Space = 0;

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

void AllNumbers(int delay_length, int seconds)
{

for (size_t i = 0; i < 9; i++)
{
  digitalWrite(TH, HIGH);  
  WriteNumber(i);
  delay(delay_length);
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

  digitalWrite(TH, LOW);
  digitalWrite(H, LOW);
  digitalWrite(TM, LOW);
  digitalWrite(M, LOW);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

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