#include <Arduino.h>
#include <U8g2lib.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>

/* This code is guaranteed nasty.
 * Written by Jaap-Willem Dooge / DoogeJ / NoxiousPluK
 * https://github.com/DoogeJ/MiniClock
 * August 1, 2020
 * 
 * Released under Zlib license:
 * https://github.com/DoogeJ/MiniClock/blob/master/licence.txt
 */

const char *ssid = "Your SSID";
const char *password = "Your Password";

// Glyphs in u8g2_font_open_iconic_all_4x_t
const char *CLOCK_ICON = "{";
const char *CALENDAR_ICON = "k";

// The display is wired to GPIO 0 (SDA) and GPIO 2 (SCL)
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, /* clock=*/2, /* data=*/0);

Timezone Netherlands;

void drawScreen(const char *icon, const String &text)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_open_iconic_all_4x_t);
  u8g2.drawStr(0, 32, icon);
  u8g2.setFont(u8g2_font_inr16_mn);
  u8g2.drawStr(48, 24, text.c_str());
  u8g2.sendBuffer();
}

void setup()
{
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 20, "Syncing time...");
  u8g2.sendBuffer();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  waitForSync();
  Netherlands.setLocation("Europe/Amsterdam");
}

void loop()
{
  events(); // lets ezTime periodically re-sync with NTP

  if (!secondChanged())
  {
    delay(20);
    return;
  }

  // Every 10 seconds: show the time for 5 seconds, then the date for 5 seconds
  uint8_t second = Netherlands.second();
  if (second % 10 < 5)
  {
    // Blink the colon: shown on even seconds, hidden on odd ones
    drawScreen(CLOCK_ICON, Netherlands.dateTime(second % 2 ? "H i" : "H:i"));
  }
  else
  {
    drawScreen(CALENDAR_ICON, Netherlands.dateTime("d-m"));
  }
}
