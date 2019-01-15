---
title: Creating a reset button on your Pi
---

*Written by Anna Zeng*

To reboot the Pi without unplugging and replugging in the USB-serial adapter, we can make use of the reset pins on the Pi. In the photo below, look for the red circle around two holes labeled "RUN". Bridging those two holes will trigger a reset of the Pi.

[<img title="Things you will need" src="../images/reset.pin.ingredients.jpg" width="400">](../images/reset.pin.ingredients.jpg)


This guide walks you through soldering a 2-pin header onto the RUN contacts and running jumpers from the header pins to a button (or, really, anything that can act like a hardware or software switch). Pressing the button will connect the pins, which then resets your Pi.

Grab a 2-pin header (available in our lab cabinet) and place onto the RUN contacts as shown below:

[<img title="Pin placement" src="../images/reset.pin.placement.jpg" width="400">](../images/reset.pin.placement.jpg)

Heat the soldering iron to approximately 600 degrees F (60 on the dial), and wait until the light blinks rapidly.

[<img title="Soldering Light" src="../images/reset.pin.solder.light.jpg" width="400">](../images/reset.pin.solder.light.jpg)

While holding the soldering iron like a pen, touch the solder on one side of the pin and hold the soldering iron on the other side. The iron will heat up the pin and pad and the liquid solder will flow over the joint and forms a little volcano.

[<img title="Soldering Position" src="../images/reset.pin.soldering.jpg" width="50%" style="display:inline;">](../images/reset.pin.soldering.jpg)[<img title="Soldering Action" src="../images/reset.pin.action.jpg" width="50%" style="float:right;">](../images/reset.pin.action.jpg)

Once the joint is complete, remove the soldering iron and the solder. Repeat for the second pin. The contacts will look sort of like this:

[<img title="Finished Soldering" src="../images/reset.pin.done.jpg" width="400">](../images/reset.pin.done.jpg)

If you temporarily connect the two header pins (such as by using a jumper or a pair of metal tweezers), your Pi should restart. This is a good test to verify your solder connections.

To make your reset switch a little more convenient to activate, you can connect the header pins to a button on your breadboard like so:

[<img title="Button connected to reset" src="../images/reset.pin.button.jpg" width="400">](../images/reset.pin.button.jpg)

Pushing the button now restarts the Pi -- cool!

Rather than use a button, you can instead connect one of the header pins to the DTR pin on your USB-serial. If the USB-serial pulls the DTR pin to ground it will trigger your Pi to restart. Our `rpi-install.py` script pulls the DTR to ground before sending a program to the bootloader.