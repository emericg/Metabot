Metabot
=======

## Introduction

The [Metabot](http://metabot.cc/) is a cool open-source DIY parametric robot created by [Rhoban](http://rhoban.com/).  

This Raspberry Pi version unlock some nice features:
* Wifi hotspot, so every device can connect directly to the Metabot
* High quality camera streaming
* New control modes using keyboard or gamepads:
  * "Direct" control mode (by connecting input device(s) to the RPi)
  * "Network" control mode (using another computer/mobile device connected to the RPi wifi hotspot)

[without the electronic](https://goo.gl/photos/y5Mf6iVfevKr2Hrw7)

## How to

You'll need a local copy of SmartServoFramework, and the [SFML 2+](https://github.com/SFML/SFML) library installed into your system.
You can build both the client and the server on pretty much any OS / HW platform you want.

### Setting up the Raspberry Pi

I have set up a google doc for my [Raspberry Pi stuffs](https://docs.google.com/document/d/1sD5Fkbx5KoUHv-pklI8Ck-DyjcLpjsvI0ImGqL_Kb-Q/edit?usp=sharing). It explain how to install and set up an Arch Linux onto a Pi, use the camera... Its mostly a memo for myself but it could be useful to others.

### Server

Make sure you checkouted a copy of the [SmartServoFramework](https://github.com/emericg/SmartServoFramework) at ``metabot_server/src/SmartServoFramework``:
* cd metabot_server/src
* git clone https://github.com/emericg/SmartServoFramework.git

* cd metabot_server
* qmake
* make

### Client

* cd metabot_client
* qmake
* make

## License

This project is under CC BY-NC license: http://creativecommons.org/licenses/by-nc/3.0/
