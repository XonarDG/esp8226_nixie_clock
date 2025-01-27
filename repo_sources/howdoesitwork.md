# How does it work?
This is a file that will explain how the clock works in detail, I might make this into separate files later on.

## Prequisits
Before we start talking about all the principles that allow this clock to work, its important that we set and define certain properties and goals of this project.

### The goal
The goal of this projct is to make a simple, easily modifiable nixie tube clock. In other words, im gonna try to use as little components as possible, while using ones that are readily available and ones that are easy to work with and allow for further modifications.
### Features and properties
1. 6 digits
* Since I've made a 4 digit clock before, I can tell you that watching the same 4 digits for a minute straight is nothing fun. A added benefit is that we can also show other interesing numbers. 
2. NTP synchronization and RTC
* The most common way of keeping somewhat precise time is by utilizing a RTC module, which keeps time using a dedicated IC powered by a battery (usually a CR2023 or similar). This is, relatively speaking, a accurate way of keeping time, but I personaly find these cheap modules too imprecise. I've measured a time delay of around 7 seconds per day, which isn't perticularly bad, but since we're using 6 digits I thought it would be rather nice to have a clock that would be constantly accurate over long periods of time. This is exactly why I decided that I want to synchronize with an NTP server.
3. Extra IO and simplicity
* Yes, I could've used a cheaper discrete MCU. Yes, I could've used SMD versions of IC's to make the footprint smaller. Of course, but that would make the project unfriendly for anyone wishing to modify it.
