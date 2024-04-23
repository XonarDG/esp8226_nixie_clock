# esp8266_nixie_clock

Simple Nixie Clock with an ESP8266 board

![alt text](https://github.com/XonarDG/esp8226_nixie_clock/blob/main/repo_sources/finished.png?raw=true "picture of the completed project")

## Info

This project has been ongoing ever since late 2021, that's because I decided to ditch the ESP32 and replace it with an ESP8266. And there's more. I also got rid of the old 74141 and replaced it with a 74HC595.
Long story short, I felt that the traditional way of multiplexing was working well enough, and I wanted to get my hands on something new, something I've not done before. I didn't upload any of my ongoing progress so far since it was more or less just a try and fail method, but now that I've got a working prototype up and running I felt it was a good time putting it out there.

## Why?
Like many others, I've fallen 

The original project used the traditional 74141 BCD to Parallel IC in combination with TLP627 optocouplers. This method is completely viable as long as you don't mind buying IC's of ebay or wherever, but one of my project's goals was making it easy to put together, and I just felt this violated that goal. *(and I wanted a challenge)*

So the new project is using 74HC595 shift registers as a replacement for the 74141 IC's. Yes, there are other ways of doing this but let me explain.

Another reason why I changed the fundamentals of this project are that I didn't want to use obsolete IC's or parts. Obviously nixie tubes are obsolete but there's nothing I can do about those. I have to admit that at this point this project isn't fully devoid of any of these problems*(coughs in HV power supply)*.

## Parts
Name | Designation | #
--- | --- | ---
MCU | Wemos D1 Mini | 1
RTC module | generic DS3231 | 1
Shift Register | 74HC595 | 3
Cathode transistors | MPSA42 | 10
Anode optoucouplers | TLP627 | 6
Voltage Regulator | AMS1117 3.3 | 1
Resistor | various values | 26
Pin header (male) | generic 2.54mm | 21
Pin header (famale) | generic 2.54mm | 46
