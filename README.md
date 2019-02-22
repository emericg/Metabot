Metabot
=======

The [Metabot](http://www.metabot.fr/) is a cool open-source DIY parametric robotic platform.  

But this one is a bit special...
* Custom electronics
* Autonomous with its Raspberry Pi Zero W
  * Real time 1080p camera streaming
  * Wifi hotspot, so any computer can connect directly, without the need for an access point
* New control modes using keyboards and gamepads:
  * "Direct" control (when connecting input device(s) directly to the RPi)
  * "Network" control (when using another computer/mobile device connected to the RPi wifi hotspot)

[See the Metabot in action in this epic spherical video!](https://www.youtube.com/watch?v=DfROMLoRV_Q)

![Metabot with its electronic](https://i.imgur.com/uBipQtQ.jpg)
![Metabot skeleton](https://i.imgur.com/F7R5CUV.jpg)

[Photo and video gallery of my Metabot](https://goo.gl/photos/wsRBAoyg2RaMAJFPA)

## About

You want your very own Metabot? Great idea!  
First, you'll need 12 Dynamixel XL-320 servos and the Metabot's 3D printed parts (see parts/metabot_v1 or directly on the Metabot website).  
Then one tiny computer. I used a Raspberry Pi Zero W, but any small computer with USB will do (I started with an RPi A model coupled with an USB hub and external wifi card). And of course, a [USB2AX](http://www.xevelabs.com/doku.php?id=product:usb2ax:usb2ax) to connect your servos to the onboard computer.  
Also, it will work better with a power source. The XL-320 servos need 6 to 8V to work (but the higher the better) and the RPi needs 5V. I personally used two 18650 batteries (~7.4V) and made a tiny motherboard with a step down voltage regulator for the RPi and a 6 ports hub for the servos (pics are in the gallery).  

Finally, the control softwares. That's what this repository is all about.  

## How to

### Setting up the Raspberry Pi

I have set up a shared Google doc for my [Raspberry Pi stuffs](https://docs.google.com/document/d/1sD5Fkbx5KoUHv-pklI8Ck-DyjcLpjsvI0ImGqL_Kb-Q/edit?usp=sharing). It explains how to install and set up an Arch Linux onto a Pi, stream the camera... It's mostly a memo for myself, but maybe it can be useful to others too.

### Metabot control softwares

You can build both the client and the server on pretty much any OS / HW platform you want.  

Make sure you got a copy of this repository, obviously, and of the [SmartServoFramework](https://github.com/emericg/SmartServoFramework) in the Metabot root directory.  
You will need the [SFML 2+](https://github.com/SFML/SFML) library installed in your system, that's the only other dependency I think.

> $ git clone https://github.com/emericg/Metabot.git  
> $ git clone https://github.com/emericg/SmartServoFramework.git Metabot/  

### Metabot server

Then you can build the metabot server:
> $ cd Metabot/metabot_server  
> $ qmake  
> $ make  

Start the server by simply running:
> $ ./build/MetaBotServer

### Metabot client

> $ cd Metabot/metabot_client  
> $ qmake  
> $ make  

Start the client by simply running:
> $ ./build/MetaBotClient

### Settings

By default the server will autodetect the serial port and servos availability, then start listening for network connections on port 5555. The client will try to connect to 'robot.local'. If you don't have Zeroconf domaine name resolution on you devices or network, you can hardcode an IP address in the networkClient::autodetect() function. Both client and server will autodetect input devices like keyboards and gamepads.

Systemd unit files are available in the raspberry/ directory. You can use them to automate the startup of some of our components: the Metabot server, camera streaming and wifi hotspot.

## License

This code is licensed under CC BY-NC license: https://creativecommons.org/licenses/by-nc/3.0/  
Metabot project by http://www.metabot.fr/  
