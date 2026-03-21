# WIFI Switch User/ Assembly Guide

This project is designed to control a device via WIFI using the provided web app.

![ATtiny85 installation orientation](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032044.jpg?raw=true)

---

## Assembly

before powering on the device, attach either your low power device that can be powered directly form the microcontroller (ie devices that need less than 3.3V to operate) or the included 3.3V relay into the
screw terminals if your device requires more power.

The screw terminal is wired to the microcontroller in the following way and is marked:
- **-** : is connected to ground
- **in** : is connected to a gpio pin that controls the switching for your device
- **+** : is connected to a 3.3v pin ONLY meant to power the relay if you choose to use it

![ATtiny85 installation orientation](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032626.jpg?raw=true)

after your device is properly screwed into the screw terminal, power on the device by plugging in the provided buckle to usbc adaptor into the microcontroller. this converter features a buck converter to convert
a 9V batterries output down to a safer 5V for the microcontroller. You can power the system with any 5V source through the usbc port on the microcontroller but the power sourse should NOT exeed 5V.

![ATtiny85 installation orientation](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032636.jpg?raw=true)
