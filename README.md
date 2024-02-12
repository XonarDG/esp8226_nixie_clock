# esp8266_nixie_clock

Simple Nixie Clock with an ESP8266 board

## Info

This project has been ongoing ever since late 2021, that's because I decided to ditch the ESP32 and replace it with an ESP8266. And there's more. I also got rid of the old 74141 and replaced it with a 74HC595.
Long story short, I felt that the traditional way of multiplexing was working well enough, and I wanted to get my hands on something new, something I've not done before. I didn't upload any of my ongoing progress so far since it was more or less just a try and fail method, but now that I've got a working prototype up and running I felt it was a good time putting it out there.

## Why?

The original project used the traditional 74141 BCD to Parallel IC in combination with TLP627 optocouplers. This method is completely viable as long as you don't mind buying IC's of ebay or wherever, but one of my project's goals was making it easy to put together, and I just felt this violated that goal. *(and I wanted a challenge)*

So the new project is using 74HC595 shift registers as a replacement for the 74141 IC's. Yes, there are other ways of doing this but let me explain.

Another reason why I changed the fundamentals of this project are that I didn't want to use obsolete IC's or parts. Obviously nixie tubes are obsolete but there's nothing I can do about those. I have to admit that at this point this project isn't fully devoid of any of these problems*(coughs in HV power supply)*.

## Parts

### 74HC595

This is one of the most accessible shift registers out there and it comes with all the feature that you would ever need for just multiplexing a couple of nixie cathodes.
It does come with its drawbacks though.

The most apparent problem is that the 74HC595 only has **8 bits**, in other words 8 IO. Because of this you can't just use one IC to drive **10 cathodes**. One way of going around this is just borrowing two GPIO pins on the MCU, but you would bring along some timing problems, so the only other way is to use more than one IC.

Another problem that arises it that the built-in transistors of the 74HC595 cannot handle **voltages above 7v**. Yes, there are IC's capable of high voltage drive, but as far as I am aware, none of these are as cheap or accessible. This means that we have to somehow bypass the internal transistors and use external drivers.

### MPSA42

Speaking of external drivers, this is the one! Again, there are many other options out there that will do the job. You can get an high voltage darlington-array IC like the ULN2003, or just a high voltage shift register e.g. TPIC6B595. I decided some good old discrete parts won't do any bad. They are relatively cheap and are readily available. You also don't have to deal with the IC's low clamp voltage.

### TLP627

The only part that ended up the same in this project is the high voltage optocoupler TLP627. As far as I am concerned it does everything just right. There were some concerns about leakage current of the swithing part of the IC, but after testing it with my IN-12 nixies, I can confirm that ghosting was barely visible. Keep in mind that my prototype is on a breadboard, so I expect that a PCB will be way better when it comes to current leakage and a GND connection (which seems like a real problem for my prototype).

### Dicrete parts

There are only a couple of discrete components used, but I'll try my best to explain why are they there.

## Resistors

Exclusively used as a way of limiting current.