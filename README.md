# simple_nixie_clock
Simple Nixie Clock with Arduino Nano

## Goal
The goal of this project is to make a simple, reproducable, versatile, expandable and easy to understand Nixie Tube clock using as little parts possible.

## The Plan
The plan that will carry this project to its completion, has 8 parts:

1. Brainstorming (features, problem solving etc.)
2. Choosing the best ideas
3. Breadboard prototyping (includes proof of concept code)
4. Fixing issues
6. Getting a statisfactory prototype
7. PCB Desing
8. Assembly

## Brainstorming

Since I already had a somewhat functional prototype that didn't utilize multiplexing, this project **WILL** make use of it. While it does make the code and PCB a little more complicated, these are negligible downsides to having much more IO availible. And for expandibility, this is something that will matter when adding more functions.
Therefore, **this project will use multiplexing**, altough that might not be the simplest way of doing things.

Another crucial feature is a **RTC** module as a accurate time source for our clock.  Its a must have because otherwise we would have to fiddle around with the microcontrollers crystal, and that's tedious work which I wanna avoid.

Power source is still in the mist for me. I could just use a proprietary 12V power source, but I feel like in this modern age, I might as well make use of USB-PD with its 12V output. This is still something that I have to thing about, but for now the USB-PD option seems to be the winner.
## WIP

First thing is getting a list of parts:

| Part | Model/Type | Reason |
|:-----|:----------:|-------:|
| Controller | Arduino Nano | accessible, easy to devleop for, cheap, small, has a lot of outputs |
| Nixie Drivers | MH74141 | accessible |
| Nixies | ZM1020 | accessible, decently sized digits, are socketed |
| Trasistors | **todo** | for multiplexing |
| Resistors | **find a good rating** | currect protection for nixies |
| RTC module | **todo** | convenient, will make the clock easier to code and more accurate |
| Step-Down converter for Controller | **todo** | necessary |
| High-voltage source for nixies | **todo** | necessary |
