#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <Update.h>
#include "time.h"
#include "arduino_secrets.h"

WiFiUDP ntpUDP;

//declaring optocoupler outputs
#define TH 13
#define H 12
#define TM 14
#define M 27

//declaring 74141's pins
#define A 33
#define B 26
#define C 32
#define D 25

//colon LED's
#define COLON_BOTTOM 19
#define COLON_TOP 18

//I2C
#define SDA 22
#define SCL 23

//declaring the RTC
RTC_DS3231 rtc;

long gmtoffset_sec = 3600; 
int daylightoffset_sec = 3600;
const char* ntpserver = "0.cz.pool.ntp.org";


const long multiplex_timing = 2;
bool doonceswitch = true;
int wantedunixtime = 0;

void setup() 
{
  Wire.begin(SDA,SCL);
  rtc.begin();
  Serial.begin(9600);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  

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
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
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
  /*switch(multiplex_counter)
  {
    case 0:
    WriteNumber(first_number);
    digitalWrite(TH, HIGH);
    multiplex_counter = 1;
    delay(multiplex_timing);
    digitalWrite(TH, LOW);
    break;
    case 1:
    WriteNumber(second_number);
    digitalWrite(H, HIGH);
    multiplex_counter = 2;
    delay(multiplex_timing);
    digitalWrite(H, LOW);
    break;
    case 2:
    WriteNumber(third_number);
    digitalWrite(TM, HIGH);
    multiplex_counter = 3;
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

  }*/
  
  WriteNumber(first_number);
  digitalWrite(TH, HIGH);
  delay(multiplex_timing);
  digitalWrite(TH, LOW);
  delay(1);

  WriteNumber(second_number);
  digitalWrite(H, HIGH);
  delay(multiplex_timing);
  digitalWrite(H, LOW);
  delay(1);

  WriteNumber(third_number);
  digitalWrite(TM, HIGH);
  delay(multiplex_timing);
  digitalWrite(TM, LOW);
  delay(1);

  WriteNumber(forth_number);
  digitalWrite(M, HIGH);
  delay(multiplex_timing);
  digitalWrite(M, LOW);
  delay(1);

}
void Colon(int second)
{
  
  if (second % 2 == 0)
  {
    digitalWrite(COLON_BOTTOM, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(COLON_BOTTOM, LOW);
    digitalWrite(LED_BUILTIN, LOW);  
  }

}

void ShowDate(int day, int month)

{

  int TenDay = (day / 10) % 10;         
  int Day = (day % 10);
  int TenMonth = (month / 10) % 10;
  int Month = (month % 10);

  digitalWrite(COLON_BOTTOM, HIGH); 
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

void GetNTPTime()
{
  DateTime rtctime = rtc.now();
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
  }
  configTime(gmtoffset_sec, daylightoffset_sec, ntpserver);
}
void WriteRTC() {
   struct tm timeinfo;
   if( !getLocalTime(&timeinfo) ) {
     return;
   }
   rtc.adjust(DateTime(timeinfo.tm_year +1900, timeinfo.tm_mon +1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
   WiFi.disconnect();
}

void UpdateRTC(int unixtime, int updateinterval_sec)
{
  if (doonceswitch == true)
  {
    GetNTPTime();
    WriteRTC();
    wantedunixtime = unixtime + updateinterval_sec;
  }

  if (unixtime == wantedunixtime)
  {
    GetNTPTime();
    WriteRTC();
    wantedunixtime = unixtime + updateinterval_sec;
  }
}

void loop()
{
  DateTime rtctime = rtc.now();

  if (doonceswitch == true)
  {
    UpdateRTC(rtctime.unixtime(), 6);
    doonceswitch = false;
  }
  Colon(rtctime.unixtime());

  if (rtctime.second() >= 55)
  {
    ShowTime(rtctime.minute(), rtctime.second());
  }
  else
  {
    ShowTime(rtctime.hour(), rtctime.minute());
  }
  UpdateRTC(rtctime.unixtime(), 21600);
}