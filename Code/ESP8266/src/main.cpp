#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>

#include "config.h"

#define DATA 14
#define CLOCK 12
#define LATCH 13
#define OE 5

//configuration variables for NTP
long gmtoffset_sec = 3600; 
int update_interval_sec = 60000;
const char* ntpserver = "0.cz.pool.ntp.org";

//NTPClient object creation
WiFiUDP ntpUDP;
NTPClient NTPC(ntpUDP, ntpserver, gmtoffset_sec, update_interval_sec);


//int number_arr[] = {1,2,4,8,16,32,64,128,256,512};
int number_arr[] = {512,1,2,4,8,16,32,64,128,256};
int display_arr[] = {1,2,4,8,16,32};
int colon_arr[] = {1,2,4,8};

uint8_t shift_reg1;
uint8_t shift_reg2;
uint8_t shift_reg3;

uint8_t shift_reg_null;

bool wifi_failure = false;

void WiFiBegin()
{
  int counter = 0;
  if (counter <= 3)
  {
    WiFi.begin(SSID, PASS);
    while ( WiFi.status() != WL_CONNECTED ) 
    {
      delay (2000);
      counter++;
    }
  }
  else
  {
    wifi_failure = true;
  }
  NTPC.begin();
}

void SendData(uint8_t data1, uint8_t data2, uint8_t data3)
{
  digitalWrite(OE, HIGH);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, data1);
  shiftOut(DATA, CLOCK, MSBFIRST, data2);
  shiftOut(DATA, CLOCK, MSBFIRST, data3);
  digitalWrite(LATCH, HIGH);
  digitalWrite(OE, LOW);
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

void setup() 
{
  pinMode(OE, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  digitalWrite(DATA,LOW);
  digitalWrite(CLOCK,LOW);
  digitalWrite(LATCH,LOW);
  digitalWrite(OE, HIGH);

  SendData(0,0,0);
  WiFiBegin();
}

void loop()
{
  if (wifi_failure != true || WiFi.status() == WL_CONNECTED)
  {
    NTPC.update();
  }
  int hh = (NTPC.getHours() / 10) % 10;         
  int h = (NTPC.getHours() % 10);
  int mm = (NTPC.getMinutes() / 10) % 10;
  int m = (NTPC.getMinutes() % 10);
  int ss = (NTPC.getSeconds() / 10) % 10;
  int s = (NTPC.getSeconds() % 10);

  Multiplex(hh,h,mm,m,ss,s, colon_arr[1], 1000, 500);
 //Multiplex(0,0,0,0,2,4,1,2000,0);
}
