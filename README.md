ol{margin:0;padding:0}table td,table th{padding:0}.c10{color:#000000;font-weight:400;text-decoration:none;vertical-align:baseline;font-size:20pt;font-family:"Arial";font-style:normal}.c8{padding-top:0pt;padding-bottom:3pt;line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}.c3{padding-top:20pt;padding-bottom:6pt;line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:center}.c13{color:#000000;font-weight:400;text-decoration:none;vertical-align:baseline;font-size:26pt;font-family:"Arial";font-style:normal}.c2{color:#000000;font-weight:400;text-decoration:none;vertical-align:baseline;font-size:11pt;font-family:"Arial";font-style:normal}.c14{padding-top:18pt;padding-bottom:6pt;line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:center}.c4{color:#000000;font-weight:400;text-decoration:none;vertical-align:baseline;font-size:16pt;font-family:"Arial";font-style:normal}.c11{padding-top:0pt;padding-bottom:3pt;line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:center}.c7{padding-top:0pt;padding-bottom:0pt;line-height:1.15;orphans:2;widows:2;text-align:center}.c5{padding-top:0pt;padding-bottom:0pt;line-height:1.15;orphans:2;widows:2;text-align:left}.c12{padding-top:0pt;padding-bottom:0pt;line-height:1.15;orphans:2;widows:2;text-align:justify}.c0{text-decoration-skip-ink:none;-webkit-text-decoration-skip:none;color:#1155cc;text-decoration:underline}.c1{background-color:#ffffff;max-width:468pt;padding:72pt 72pt 72pt 72pt}.c9{color:inherit;text-decoration:inherit}.c6{height:11pt}.title{padding-top:0pt;color:#000000;font-size:26pt;padding-bottom:3pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}.subtitle{padding-top:0pt;color:#666666;font-size:15pt;padding-bottom:16pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}li{color:#000000;font-size:11pt;font-family:"Arial"}p{margin:0;color:#000000;font-size:11pt;font-family:"Arial"}h1{padding-top:20pt;color:#000000;font-size:20pt;padding-bottom:6pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}h2{padding-top:18pt;color:#000000;font-size:16pt;padding-bottom:6pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}h3{padding-top:16pt;color:#434343;font-size:14pt;padding-bottom:4pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}h4{padding-top:14pt;color:#666666;font-size:12pt;padding-bottom:4pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}h5{padding-top:12pt;color:#666666;font-size:11pt;padding-bottom:4pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;orphans:2;widows:2;text-align:left}h6{padding-top:12pt;color:#666666;font-size:11pt;padding-bottom:4pt;font-family:"Arial";line-height:1.15;page-break-after:avoid;font-style:italic;orphans:2;widows:2;text-align:left}

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