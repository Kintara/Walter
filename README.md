# Walter
Software for ESP32 for the control of a BeamZ 1800Snow machine for Walter the Snowblowing Man

Walter: An Introduction

I’m so pleased to finally get the hardware and 
software fully operational on this project.

Walter is a Corro prop shaped like a snowman designed
by myself and produced by Build A Lightshow.

He is fitted with 12v seed pixels and a P5 32x64 matrix.

Walter has a hole for his mouth through which artificial
snow can be blown.

The Snow blower is a Blitz Snow 1800 with mods
and upgrades.

It’s has an auto filling system.
It has a bespoke digital handset powered by
an ESP32 that can set the interval and duration
of snow production as well as the force and
volume of snow.

All values are easily seen on an OLED screen
and are set through a touch keypad.

Plus it produces a wifi accessible webpage to
monitor and set the values for various parameters:
interval, duration, volume and force of snow. It can produce snow using a timer, or
manual function to give snow on demand.

It can be set up to be accessible though the home WIFI network or by its own access point.

Inside the body of the snow blower is a Raspberry Pi Zero2 WH with a Hanson RPi-MFC cape,
which I use to operate the pixel lights and the P5 panel on Walter the Snowman.
The RPi can run stored sequences and also connect to a show network to be part of a show.

Another neat thing about the Hanson MFC are the exposed GPIO pins. I use these to
communicate with the hand controller. When the hand controller commands snow it
signals the Rpi, which then plays a “Snow” sequence.
A sequence can include commands to overide the hand controller, to produce Snow
or No Snow. 
I’m pleased that I’ve worked through this project from scratch and produced a great
addition to my show. 

Hope you like it.
