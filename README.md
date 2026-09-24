# MiniClock
A small desk clock powered by an ESP8266/ESP-01 and a 128x32 I2C 0.91" OLED Display.

![Clock in action](/MiniClock.gif?raw=true "Clock in action")

# Introduction
For a while I wanted a nice small clock on my desk that would tell me both the date and time.

Now the specific code for this project is only ready-for-use if you (like me) live in the Netherlands (or the same timezone). To use another timezone, change `TIMEZONE_POSIX` in the code.
For anyone else, consider it a starting point for a project like this. I hope you find it useful.

# Parts list (with links to AliExpress)
* [0.91 inch 128x32 I2C Blue OLED Display (12832 SSD1306)](https://www.aliexpress.com/item/32788923016.html)
* [ESP8266 ESP-01 Module](https://www.aliexpress.com/item/32948054694.html)
* [CJMCU USB Micro B Breakout](https://www.aliexpress.com/item/32815913372.html)
* [AMS1117 800MA DC-DC 5V to 3.3V Step-down converter](https://www.aliexpress.com/item/32829322098.html)
* A 3D-printed enclosure (See [Clock case.stl](/Clock%20case.stl) and [Clock case lid.stl](/Clock%20case%20lid.stl))
* Some wires
* Some code (See [MiniClock.ino](/MiniClock.ino))

# WiFi setup
The first time the clock starts (or whenever it can't connect to the saved network, for example after a password change), it opens its own WiFi network called `MiniClock`. The display tells you when that happens.

Connect to it with your phone or laptop and a setup page opens automatically. Pick your WiFi network, enter the password and save. The clock then connects and remembers it.

If nobody sets it up within 5 minutes, the clock restarts and tries the saved network again.

# Libraries
* [U8g2](https://github.com/olikraus/u8g2)
* [ezTime](https://github.com/ropg/ezTime)
* [WiFiManager](https://github.com/tzapu/WiFiManager)

# Assembly
Please note that on this display, only VCC, NC (GND), SDA and SCL are exposed.
![How to connect all the things](/schematic.png?raw=true "How to connect all the things")

# Remarks and potential improvements
* I found out on mine that the lid was a little too big. I have corrected the STL file for that, but not checked if it is actually okay now. Please let me know if you have issues with it.
* If you want to go the extra mile (be less lazy than me) and include a button and a [CH340](https://www.aliexpress.com/item/32816118069.html) you can keep it programmable.
* You can also implement OTA updates by making some minor changes to the code: https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
* I used a 3D pen to mount all the parts inside the enclosure. A hot glue gun will work just as fine.
