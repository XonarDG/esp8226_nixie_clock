#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <Update.h>
#include "time.h"
#include "arduino_secrets.h"

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


//NTP server config variables
long gmtoffset_sec = 3600; 
int daylightoffset_sec = 3600;
const char* ntpserver = "0.cz.pool.ntp.org";

//operation config variables
const long multiplex_timing = 2;
bool doonceswitch = true;
int nextrtcupdate = 0;
int wifitimeout = 0;
bool connection_fault = false;

void setup() 
{
  Wire.begin(SDA,SCL); //starts I2C communication on defined GPIO
  rtc.begin(); //starts communication with the RTC module
  Serial.begin(9600); //debug
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //updates the RTC's time according to the time of the code compilation
  

  //declaring pin modes
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

  //settings all output pins LOW for a good measure
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
  //this method contains the truth table for the MH/SN74141 and sets the pins according to given number
  //if needed this can be rewritten to work with other Nixie Drivers 
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
  //this method handles the multiplexing of the nixies, the delays are there to slow the ESP32 down, since at full speed, the MH74141 is not able to keep up with the ESP32
  //eventually this has to be rewritten to use millis() instead of delay()
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

void Colon(int unixtime)
{
  //this method operates the colon, using the function "%" (modulo) to determine if the number is even or odd
  //in the future this method will be changed to allow individual control over the LED's as to allow for signaling faults
  if (connection_fault == true)
  {
      if (unixtime % 2 == 0)
  {
    digitalWrite(COLON_BOTTOM, HIGH);
    digitalWrite(COLON_TOP, HIGH);
  }
  else
  {
    digitalWrite(COLON_BOTTOM, LOW);
    digitalWrite(COLON_TOP, HIGH);  
  }
  }
  else
  {
    if (unixtime % 2 == 0)
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
}
void ShowDate(int day, int month)
{
  //not used currently
  int TenDay = (day / 10) % 10;         
  int Day = (day % 10);
  int TenMonth = (month / 10) % 10;
  int Month = (month % 10);

  digitalWrite(COLON_BOTTOM, HIGH); 
  MultiPlex(TenDay, Day, TenMonth, Month);
}
void ShowTime(int hour, int minute)
{
  //this method converts the RTC's time output into separate numbers that are then fed to the MultiPlex() method to show the current time
  //the use of modulo is taken from GreatScott's project
  int TenHour = (hour / 10) % 10;         
  int Hour = (hour % 10);
  int TenMinute = (minute / 10) % 10;
  int Minute = (minute % 10);

  MultiPlex(TenHour, Hour, TenMinute, Minute);
}

int GetNTPTime()
{
  //this method connects to Wi-Fi and gets the current NTP time
  DateTime rtctime = rtc.now();
  wifitimeout = rtctime.unixtime();
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (rtctime.unixtime() - wifitimeout > 15)
    {
      return 0;
    }
  }
  configTime(gmtoffset_sec, daylightoffset_sec, ntpserver);
  return 1;
}
void WriteRTC() {
   struct tm timeinfo;
   if(!getLocalTime(&timeinfo)) {
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
    nextrtcupdate = unixtime + updateinterval_sec;
    doonceswitch = false;
  }

  if (unixtime == nextrtcupdate)
  {
    if (GetNTPTime() == 0)
    {
      connection_fault = true;
      nextrtcupdate = unixtime + updateinterval_sec;
      return;
    }
    WriteRTC();
    nextrtcupdate = unixtime + updateinterval_sec;
  }
}

void loop()
{
  DateTime rtctime = rtc.now();

  if (rtctime.second() >= 55)
  {
    ShowTime(rtctime.minute(), rtctime.second());
  }
  else
  {
    ShowTime(rtctime.hour(), rtctime.minute());
  }
  
  UpdateRTC(rtctime.unixtime(), 21600);
  Colon(rtctime.unixtime());
}