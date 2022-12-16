# simple_nixie_clock

Simple Nixie Clock with Arduino Nano

## Goal

The goal of this project is to make a simple, reproducible, versatile, expandable and easy to understand Nixie Tube clock using as little parts possible.

## The Plan

The plan that will carry this project to its completion, has 8 parts:

1. Brainstorming (features, problem solving etc.)
2. Choosing the best ideas
3. Breadboard prototyping (includes proof of concept code)
4. Fixing issues
6. Getting a satisfactory prototype
7. PCB Design
8. Assembly

## Brainstorming

The first decision I have to make is what microcontroler to use. While I have thought about using bare-bones microcontrolers, a DIP20 ATtiny2313 for example, it occurred to me that those would be a bit of a pain to flash and use in the project. So instead I went for the **Arduino Nano**. It's main advantages are simple flashing and a durable body that protects it from static electricity and such. And yeah, if you wanted to make a compact PCB you could use a SMD AU package, but this project is about making a simple and modular clock.

Nixie drivers are complicated. There are virtually no modern alternatives, at least to my knowledge, and the SN74141 or the russian K155ID1, as well as their clones, are the only simple and efficient way of driving nixies. Since I'm based in the Czech Republic, I will be using Tesla MH74141 that I have from my previous prototype.

Nixies are annoying. Again there are virtually no alternatives other then the old stock russian Gazotron, Philips, Tesla or other manufacturers tubes *(unless you are prepared to play upwards of hundreds euros on nixies alone)* so I'm just using **Tesla ZM1020** which I have on hand. I do not plan making this project specifically just for these tubes, so you're free to use **IN-12**, IN-14, ZM1040 or whatever you find. But keep in mind that you might need to change the project here and there for your nixies to work properly.

Since I already had a somewhat functional prototype that didn't utilize multiplexing, this project **WILL** make use of it. While it does make the code and PCB a little more complicated, these are negligible downsides to having much more IO available. And for expandability, this is something that will matter when adding more functions.
Therefore, **this project will use multiplexing**, although that might not be the simplest way of doing things.

Another crucial feature is a **RTC** module as a accurate time source for our clock.  Its a must have because otherwise we would have to fiddle around with the Nano crystal, and that's tedious work which I wanna avoid.

Power source is still in the mist for me. I could just use a proprietary 12V power source, but I feel like in this modern age, I might as well make use of USB-PD with its 12V output. This is still something that I have to thing about, but for now the USB-PD option seems to be the winner.

Even though the power source for this project isn't yet decided, I know for sure that it has to be 12V since that's the lowest voltage that my High-voltage source works at. This of course means that I have to somehow lower the voltage for the Nano since that is only really capable running on around 6V. *(Yes, you can indeed run them on 12V as well, but the voltage regulator of the Nano isn't made for those voltages, so you are practically hazarding with burning down your board.)* After a bit of searching I found a really [tiny DC/DC stepdown converter](https://www.ebay.com/itm/401605679624?) that is very much capable of supplying the Nano, while being quite efficient. An alternative to this module would be a linear converter, but those are very inefficient. Its also a tiny module so it's not taking up a lot of space on the PCB.

First thing is getting a list of parts:

| Part | Model/Type | Reason |
|:-----|:----------:|-------:|
| Controller | Arduino Nano | accessible, easy to develop for, cheap, small, has a lot of outputs |
| Nixie Drivers | MH74141 | accessible |
| Nixies | ZM1020 | accessible, decently sized digits, are socketed |
| Transistors | **todo** | for multiplexing |
| Resistors | **find a good rating** | current protection for nixies |
| RTC module | **todo** | convenient, will make the clock easier to code and more accurate |
| Step-Down converter for Controller | **todo** | necessary |
| High-voltage source for nixies | **todo** | necessary |
