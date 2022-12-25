# How does it work?

This is gonna be a short(long) paper about how the project works.

## The short version (TLDR)

**Read this if you already have so previous expercience!**

### Part I - **Hardware**

Hardware vise our project is actually very simple.
We have 4 nixie tubes, which we want to drive with a single 74141 BCD driver. This of course means that we are gonna make use of multiplexing.
For this project I've decided that the easiest way of doing this would be to use a High Voltage capable Optocoupler. This is actually **not my idea**, I got inspired by a different open source project, which happened to utilize the TLP627 Optocoupler. An old way of doing this would be having a transistor setup, but this way its cleaner and makes the PCB less cramped.
Anyway in the end we need 4 IO for the 74141 and another 4 for our Optocouplers, making for a total of 8 IO pins just for driving the nixies. We have to make sure our optocouplers don't get the full 5v coming out of our Arduino Nano, this will be done by 4x450R resistors.