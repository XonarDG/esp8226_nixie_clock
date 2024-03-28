#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <RTClib.h>
#include <SPI.h>

#include "config.h"

#define DATA D5
#define CLOCK D7
#define LATCH D6

#define SDA D1
#define SCL D2

//configuration variables for NTP


RTC_DS3231 ds3231;

//integer arrays used as a truth table for the shift registers
int cathode_arr[] = {512,1,2,4,8,16,32,64,128,256};
int anode_arr[] = {1,2,4,8,16,32};
int colon_arr[] = {0b0000000, 0b00000011, 0b00001100,0b00001010, 0b00000101, 0b00000011, 0b00001100, 0b00001010, 0b00001111};

//shift register data variables
uint8_t shift_reg1;
uint8_t shift_reg2;
uint8_t shift_reg3;
uint8_t shift_reg_null;

//connects to Wi-Fi
void WiFiBegin()
{
  WiFi.begin(SSID, PASS);
}
//defines custom I2C pins, establishes I2C comms with RTC
void RTCBegin()
{
  Wire.begin(SDA, SCL);
  ds3231.begin();
}
//gets NTP time and returns it in DateTime object
DateTime GetNTPTime()
{
  struct tm ntp_time;
  configTime(gmtoffset_sec, daylightoffset_sec, ntpserver);
  getLocalTime(&ntp_time);
  DateTime time = DateTime(ntp_time.tm_year+1900, ntp_time.tm_mon+1, ntp_time.tm_mday, ntp_time.tm_hour, ntp_time.tm_min, ntp_time.tm_sec);
  return time;
}
//connects to Wi-Fi, gets NTP time, updates RTC, disconnects from Wi-Fi
void TimeSync()
{
  ds3231.adjust(GetNTPTime());
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}
//handles data sent to shift register
void SendData(uint8_t data1, uint8_t data2, uint8_t data3)
{
  digitalWrite(LATCH, LOW);               //sets the LATCH pin LOW, allows shift registers to read data
  shiftOut(DATA, CLOCK, MSBFIRST, data1); //sends first 8 bits
  shiftOut(DATA, CLOCK, MSBFIRST, data2); 
  shiftOut(DATA, CLOCK, MSBFIRST, data3);
  digitalWrite(LATCH, HIGH);              //sets the LATCH pin HIGH, shift registers now act as memory
}
//multiplexes given data
void Multiplex(int hh, int h, int mm, int m, int ss, int s, int multiplex_delay_micro, int blank_delay_micro)
{
  int time_arr[] = {hh,h,mm,m,ss,s};

  for (size_t i = 0; i <= 5; i++)
  {
  
  //simple logic to blink the colons on even numbers
  int set_colons;
  if (WiFi.status() == WL_NO_SSID_AVAIL)
  {
  if (s % 2 == 1)
  set_colons = 0;
  else
  set_colons = 7;
  }
  else
  {
  if (s % 2 == 1)
  set_colons = 0;
  else
  set_colons = 8;
  }
  
  //sets the cathodes according to the multiplex cycle
  int cathodes = time_arr[i];
  int cathode_state = cathode_arr[cathodes];

  //bit manipulation to give each shift register correct bits
  shift_reg1 = (cathode_state >> 2);                       //takes the first 8 bits from cathode_state        
  shift_reg2 = ((cathode_state << 6) | (anode_arr[i]));  //takes the last 2 bits cathode_state and all six from display_arr(which is set by the current multiplex cycle)
  shift_reg3 = colon_arr[set_colons];

  //setting 
  shift_reg_null = ((cathode_state << 6) | 0);

  //send data with blanking interval
  SendData(shift_reg1, shift_reg2, shift_reg3);           
  delayMicroseconds(multiplex_delay_micro);             
  SendData(shift_reg1, shift_reg_null, colon_arr[0]);   //blanking data
  delayMicroseconds(blank_delay_micro);                 //length of blanking interval
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
      Multiplex(i,i,i,i,i,i, 1000, 500);
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
  //declares pinmodes
  pinMode(DATA, OUTPUT);    
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);

  //sets all pins LOW
  digitalWrite(DATA,LOW);   
  digitalWrite(CLOCK,LOW);
  digitalWrite(LATCH,LOW);

  WiFi.setAutoReconnect(false); 
  SendData(0,0,0);  //sends empty data
  RTCBegin();       //starts I2C session with the RTC module
  WiFiBegin();

  //sets time of next trigger for timed methods
  DateTime rtc = ds3231.now();
  int unixtime = rtc.unixtime();
  animation_next = unixtime + animation_interval_sec;
  ntp_update_next = unixtime + ntp_update_interval_sec;
}

void loop()
{
  DateTime rtc = ds3231.now();

  int hh = (rtc.hour() / 10) % 10;         
  int h = (rtc.hour() % 10);
  int mm = (rtc.minute() / 10) % 10;
  int m = (rtc.minute() % 10);
  int ss = (rtc.second() / 10) % 10;
  int s = (rtc.second() % 10);
  Multiplex(hh,h,mm,m,ss,s, 1000, 500);
  
  if (rtc.unixtime() >= animation_next)
  {
    Animation(10, 10);
    animation_next = rtc.unixtime() + animation_interval_sec;
  }
  
  if (rtc.unixtime() >= ntp_update_next)
  {
    WiFiBegin();
    ntp_update_next = rtc.unixtime() + ntp_update_interval_sec;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    TimeSync();
  }
}
