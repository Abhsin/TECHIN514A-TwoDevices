
ColorPick!

ColorPick is a device that picks up colors from the real world, from physical objects and anything you put it on. It then displays the color you picked on a display device, which shows its corresponding hex code, and color harmonies as you desire, all based on the color theory. It is a perfect tool for industrial designers and makers to make their products as visually appealing as possible.

The Sensor Device![](images/image1.png)
---------------------------------------

![](images/image8.jpg)

![](images/image5.jpg)
======================

The sensor device makes use of the AS7341  color sensor to capture over 11 channels of light - giving us the most accurate color data.

The Display Device ![](images/image3.png)
=========================================

The Display device uses 28BYJ-48 Stepper Motor, Controlled by a ULN200X driver with a

Adafruit 1.27" and 1.5" Color OLED Breakout Board, to create the color harmonies on the color wheel and display accurate colors and hex code on the Oled display.

![](images/image7.jpg)![](images/image9.jpg)
============================================

The Communication Channel of the TwoDevices ![](images/image2.png)
==================================================================

I have selected Bluetooth Low Energy as the communication protocol for the two devices. The new BLE technology is fast, reliable and the connection rarely drops. It also consumes very low energy as the name suggests and is able to frequently share data.

![](images/image4.png)

Credits for this image - [https://randomnerdtutorials.com/esp32-ble-server-client/](https://www.google.com/url?q=https://randomnerdtutorials.com/esp32-ble-server-client/&sa=D&source=editors&ust=1705479624821817&usg=AOvVaw0WYItbRsTZ2SFTMVR7wsUY)

How it Works
============

The Color Sensor gives me 11 channels (Wavelengths) of color data, which then can be converted into RGB data which in turn can be used to create matching color hex codes.

![](images/image6.png)![](images/image10.png)
