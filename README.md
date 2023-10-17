# simple_nixie_clock

Simple Nixie Clock with Arduino Nano

## Goal

The goal of this project is to make a simple, reproducible, versatile, expandable and easy to understand Nixie Tube clock using as little parts possible.

## Current state of the project

Its been quite a while since I worked on this project. During this time I was able to learn quite alot about multiplexing and just electronics in general which allomed me to see deeper into the problem.
The prototype currently on hand works as follows.

I decided to ditch the Arduino Nano method and just went for a ESP-32 based prototype. Right now the ESP-32 is quite overkill, but it allowed me to experiment with the multiplexing method using a MH74141 and a TLP627. TLDR the multiplexing works by using the MH74141 as a decimal decoder, used for the nixie's kathodes, and the TLP627 (a optocoupler transistor package), to switch between the nixie's anodes.
This method is effecient when it comes to the ammout of IC's used, but it comes with drawbacks.

The first problem is that the ESP-32 itself has to do all the multiplexing, thus when you want to perform other functions, the multiplexing stops. As far as I am concerned there really isn't any other **simple** way to multiplex nixies. 

The other way of driving nixies is through full-duplexing. The idea is that you use a 10bit Serial-in Paralell-Out Shift Register for each nixie daisychained one to another, and using an adjectant transistor array package for the high voltage switching side for the kathodes. I find this method a bit messy compared to the multiplexing but it has its great advantages. The first one is the amount of IO used. As of now I use 8 IO for the multiplexing alone, but full-duplexing would only require 3 thanks to the possibility of chaining shift registers. Another advantage is that a shift register can hold its state. This means that the microcontroller doesn't have to constantly update the display.


First thing is getting a list of parts:

| Part | Model/Type | Reason |
|:-----|:----------|-------:|
| Controller | Arduino Nano | accessible, easy to develop for, cheap, small, has a lot of outputs |
| Nixie Drivers | MH74141 | accessible |
| Nixies | ZM1020 | accessible, decently sized digits, are socketed |
| Optocouplers | TLP-627 | convenient, simple |
| Resistors | **todo** | current protection for nixies, optocoupler diode ends |
| RTC module | DS1307 Board | convenient, will make the clock easier to code and more accurate |
| Step-Down converter for Controller | HX Mini360 | necessary, small size |
| High-voltage source for nixies | **todo** | necessary |
