# Code Explanation
This is where I explain how, and why, my code works.
## Includes, Definitions, Variables
First I we have to include a couple of libraries to make this project work.
1. RTCLib - this library allows us to communicate with our RTC through a object
2. WiFi - this library enables us to use ESP-32's built in WiFi module
3. Time - this library adds NTP functionality
4. arduino_secrets.h - is a file that contains your WiFi's SSID and PASSPHRASE

To make things easier and more modifiable I defined all the I/O pins with names. They are all modifiable for your liking, however, you must make sure that they are all defined correctly. For example, wrongly defining the 74141 completely mixes up the outputs. Another thing to note of is that the ESP-32 has some **input only pins**, so make sure not to define these up for output pins.

[On line 34](https://github.com/XonarDG/simple_nixie_clock/blob/43f55dfb689373b85f15f2fa469af4e483e7dcaf/Code/ESP32/src/main.cpp#L34), I'm creating an object with `RTC_DS3231 rtc;` the `rtc` object is than later used to get time from the RTC.
This line can be changed to `RTC_DS1307 rtc;` or `PCF8523 rtc;` if you are using a different RTC. I have only tested the `DS1307`.

### Variables
[There are NTP setting variables.](https://github.com/XonarDG/simple_nixie_clock/blob/43f55dfb689373b85f15f2fa469af4e483e7dcaf/Code/ESP32/src/main.cpp#L36)
The first one `gmtoffset_sec` is a variable that sets your timezone in seconds.
`daylightoffset_sec` is a setting for the winter time timezone.
`ntpserver` simply sets the NTP server to use. I recommend using `pool.ntp.org`.

Lines 40 to x are variables that are due to change.
