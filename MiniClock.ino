#include <Arduino.h>
#include <U8g2lib.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

/* This code is guaranteed nasty.
 * Written by Jaap-Willem Dooge / DoogeJ / NoxiousPluK
 * https://github.com/DoogeJ/MiniClock
 * August 1, 2020
 * 
 * Released under Zlib license:
 * https://github.com/DoogeJ/MiniClock/blob/master/licence.txt
 */

// Name of the setup network the clock opens when it can't connect to WiFi
const char *SETUP_AP_NAME = "MiniClock";
// Close the setup network and reboot after this many seconds without changes
const unsigned long SETUP_PORTAL_TIMEOUT = 300;
// Reboot if the time can't be fetched within this many seconds
const uint16_t SYNC_TIMEOUT = 60;

// Europe/Amsterdam, including summer time, so no online time zone lookup is needed
const char *TIMEZONE_POSIX = "CET-1CEST,M3.5.0,M10.5.0/3";

// Brightness from 0 to 255; lower slows down OLED burn-in
const uint8_t CONTRAST_DAY = 128;
const uint8_t CONTRAST_NIGHT = 1;
const uint8_t NIGHT_START_HOUR = 23;
const uint8_t NIGHT_END_HOUR = 7;

// Glyphs in u8g2_font_open_iconic_all_4x_t
const char *CLOCK_ICON = "{";
const char *CALENDAR_ICON = "k";

// The display is wired to GPIO 0 (SDA) and GPIO 2 (SCL)
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, /* clock=*/2, /* data=*/0);

Timezone Netherlands;

void drawMessage(const char *line1, const char *line2 = "")
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 12, line1);
  u8g2.drawStr(0, 28, line2);
  u8g2.sendBuffer();
}

void drawScreen(const char *icon, const String &text, uint8_t shift)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_open_iconic_all_4x_t);
  u8g2.drawStr(shift, 32, icon);
  u8g2.setFont(u8g2_font_inr16_mn);
  u8g2.drawStr(48 + shift, 24, text.c_str());
  u8g2.sendBuffer();
}

void showSetupInstructions(WiFiManager *wm)
{
  drawMessage("WiFi setup: connect", ("to '" + wm->getConfigPortalSSID() + "'").c_str());
}

void setup()
{
  u8g2.begin();
  drawMessage("Connecting to WiFi...");

  // Connects with the saved credentials. If that fails (first boot, or the
  // password changed), it opens a setup network with a captive portal where
  // the WiFi network and password can be chosen.
  WiFiManager wifiManager;
  wifiManager.setAPCallback(showSetupInstructions);
  wifiManager.setConfigPortalTimeout(SETUP_PORTAL_TIMEOUT);
  if (!wifiManager.autoConnect(SETUP_AP_NAME))
  {
    ESP.restart();
  }

  drawMessage("Syncing time...");
  if (!waitForSync(SYNC_TIMEOUT))
  {
    ESP.restart();
  }
  Netherlands.setPosix(TIMEZONE_POSIX);
}

void loop()
{
  events(); // lets ezTime periodically re-sync with NTP

  if (!secondChanged())
  {
    delay(20);
    return;
  }

  uint8_t hour = Netherlands.hour();
  bool night = hour >= NIGHT_START_HOUR || hour < NIGHT_END_HOUR;
  u8g2.setContrast(night ? CONTRAST_NIGHT : CONTRAST_DAY);

  // Move everything a few pixels sideways every minute so the same pixels aren't always lit
  uint8_t shift = Netherlands.minute() % 4;

  // Every 10 seconds: show the time for 5 seconds, then the date for 5 seconds
  uint8_t second = Netherlands.second();
  if (second % 10 < 5)
  {
    // Blink the colon: shown on even seconds, hidden on odd ones
    drawScreen(CLOCK_ICON, Netherlands.dateTime(second % 2 ? "H i" : "H:i"), shift);
  }
  else
  {
    drawScreen(CALENDAR_ICON, Netherlands.dateTime("d-m"), shift);
  }
}
