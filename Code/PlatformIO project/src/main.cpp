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

void setup() 
{

  Serial.begin(9600);
  rtc.begin();

  pinMode(TH, OUTPUT);
  pinMode(H, OUTPUT);
  pinMode(TM, OUTPUT);
  pinMode(M, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);

}

void loop()
{

  Space = 1000;

  DateTime time = rtc.now();

  int TenHour = 0;
  int Hour = 0;
  int TenMinute = 0;
  int Minute = 0;

  TenHour = (time.hour() / 10) % 10;         
  Hour = (time.hour() % 10);
  TenMinute = (time.minute() / 10) % 10;
  Minute = (time.minute() % 10);

  switch(MultiPlexCounter)
  {
    case 0:
    WriteNumber(TenHour);
    digitalWrite(TH, HIGH);
    MultiPlexCounter++;
    Serial.print(TenHour);
    delay(Space);
    break;
    case 1:
    WriteNumber(Hour);
    digitalWrite(TH, LOW);
    digitalWrite(H, HIGH);
    MultiPlexCounter++;
    Serial.print(Hour);
    delay(Space);
    break;
    case 2:
    WriteNumber(TenMinute);
    digitalWrite(H, LOW);
    digitalWrite(TM, HIGH);
    MultiPlexCounter++;
    Serial.print(TenMinute);
    delay(Space);
    break;
    case 3:
    WriteNumber(Minute);
    digitalWrite(TM, LOW);
    digitalWrite(M, HIGH);
    MultiPlexCounter = 0;
    Serial.print(Minute);
    Serial.println();
    delay(Space);
    break;
  }
  digitalWrite(M, LOW);
}