#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <RTClib.h>
#include <SPI.h>

#include "config.h"

#define DATA 14
#define CLOCK 12
#define LATCH 13

#define SDA 5
#define SCL 4

//configuration variables for NTP
long gmtoffset_sec = 3600; 
int daylightoffset_sec = 0;
const char* ntpserver = "0.cz.pool.ntp.org";

WiFiUDP ntpUDP;
RTC_DS3231 ds3231;

//integer arrays used as a truth table for the shift registers
int number_arr[] = {512,1,2,4,8,16,32,64,128,256};
int display_arr[] = {1,2,4,8,16,32};
int colon_arr[] = {1,2,4,8};

//shift register data variables
uint8_t shift_reg1;
uint8_t shift_reg2;
uint8_t shift_reg3;
uint8_t shift_reg_null;

void WiFiBegin()
{
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    for (size_t i = 0; i < 3; i++)
    {
      delay (500);
    }
  }
  //NTPC.begin();
}
void WiFiStop()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void RTCBegin()
{
  Wire.begin(SDA, SCL);
  ds3231.begin();
}
void RTCSync(DateTime time)
{
  ds3231.adjust(time);
}
DateTime NTPSync()
{
  WiFiBegin();
  struct tm timeinfo;
  configTime(gmtoffset_sec, daylightoffset_sec, ntpserver);
  getLocalTime(&timeinfo);
  DateTime time = DateTime(timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return time;
}
void TimeSync()
{
  WiFiBegin();
  RTCSync(NTPSync());
  WiFiStop();
}

void SendData(uint8_t data1, uint8_t data2, uint8_t data3)
{
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, data1);
  shiftOut(DATA, CLOCK, MSBFIRST, data2);
  shiftOut(DATA, CLOCK, MSBFIRST, data3);
  digitalWrite(LATCH, HIGH);
}
void Multiplex(int hh, int h, int mm, int m, int ss, int s, int colons, int multiplex_delay_micro, int blank_delay_micro)
{
  int time_arr[] = {hh,h,mm,m,ss,s};

  for (size_t i = 0; i <= 5; i++)
  {
  int cathodes = time_arr[i];
  int set_colons = colons;

  int shown_number = number_arr[cathodes];
  int shown_colons = colon_arr[set_colons];

  shift_reg1 = (shown_number >> 2); 
  shift_reg2 = ((shown_number << 6) | (display_arr[i]));
  shift_reg3 = shown_colons;

  shift_reg_null = ((shown_number << 6) | 0);
  SendData(shift_reg1, shift_reg2, shift_reg3);
  delayMicroseconds(multiplex_delay_micro);
  SendData(shift_reg1, shift_reg_null, shift_reg3);
  delayMicroseconds(blank_delay_micro);
  }
}
void Multiplex(int arr[6], int colons, int multiplex_delay_micro, int blank_delay_micro)
{
  for (size_t i = 0; i <= 5; i++)
  {
  int cathodes = arr[i];
  int set_colons = colons;

  int shown_number = number_arr[cathodes];
  int shown_colons = colon_arr[set_colons];

  shift_reg1 = (shown_number >> 2); 
  shift_reg2 = ((shown_number << 6) | (display_arr[i]));
  shift_reg3 = shown_colons;

  shift_reg_null = ((shown_number << 6) | 0);
  SendData(shift_reg1, shift_reg2, shift_reg3);
  delayMicroseconds(multiplex_delay_micro);
  SendData(shift_reg1, shift_reg_null, shift_reg3);
  delayMicroseconds(blank_delay_micro);
  }
}
void Animation(u_long speed, u_int repeats)
{
  u_long  current_millis = millis();
  u_long start_millis = millis();
  for (size_t y = 0; y <= repeats; y++)
  {
    for (size_t i = 0; i < 10;)
    {
      current_millis = millis();
      Multiplex(i,i,i,i,i,i, colon_arr[0], 1000, 500);
      yield();
      if (current_millis - start_millis >= speed)
      {
        start_millis = millis();
        i++;
      }
    }
  }
}

void setup() 
{
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  digitalWrite(DATA,LOW);
  digitalWrite(CLOCK,LOW);
  digitalWrite(LATCH,LOW);

  WiFi.setAutoReconnect(false);
  SendData(0,0,0);
  RTCBegin();
  TimeSync();

  DateTime rtc = ds3231.now();
  int unixtime = rtc.unixtime();
  animation_next = unixtime + animation_interval_sec;
  ntp_update_next = unixtime + ntp_update_interval_sec;
}

void loop()
{
  //NTPC.update();
  DateTime rtc = ds3231.now();

  /*
  int hh = (NTPC.getHours() / 10) % 10;         
  int h = (NTPC.getHours() % 10);
  int mm = (NTPC.getMinutes() / 10) % 10;
  int m = (NTPC.getMinutes() % 10);
  int ss = (NTPC.getSeconds() / 10) % 10;
  int s = (NTPC.getSeconds() % 10);
  */

  int hh = (rtc.hour() / 10) % 10;         
  int h = (rtc.hour() % 10);
  int mm = (rtc.minute() / 10) % 10;
  int m = (rtc.minute() % 10);
  int ss = (rtc.second() / 10) % 10;
  int s = (rtc.second() % 10);
  Multiplex(hh,h,mm,m,ss,s, colon_arr[1], 1000, 500);
  
  if (rtc.unixtime() >= animation_next)
  {
    Animation(10, 10);
    animation_next = rtc.unixtime() + animation_interval_sec;
  }
  
  if (rtc.unixtime() >= ntp_update_next)
  {
    TimeSync();
    ntp_update_next = rtc.unixtime() + ntp_update_interval_sec;
  }
}
