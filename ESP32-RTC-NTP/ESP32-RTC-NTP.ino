#include <WiFi.h>
#include "time.h"
#include "sntp.h"
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

int timeDelay = 1000; // 1 second

//0x3F or 0x27
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);   //LCD Object

const char* ssid       = "xxxxx";
const char* password   = "yyyyy";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600*(-8); // PDT
const int   daylightOffset_sec = 3600;



void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }

  //Display Date & Time
  lcd.setCursor(1, 0);
  lcd.print(&timeinfo, "%b %d, %Y");    //Nov 22, 2022
  lcd.setCursor(1,1);
  lcd.print(&timeinfo, "%A, %H:%M"); //Tuesday, 10:30
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  lcd.clear();
  printLocalTime();
}

void setup()
{
  Serial.begin(115200);

  // Setup LCD with backlight and initialize
  lcd.init();
  lcd.backlight();

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Connecting to ");
  delay(timeDelay);
  lcd.setCursor(1, 1);
  lcd.print(ssid);
  delay(timeDelay);

  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(timeDelay/2);
        Serial.print(".");
    }
  Serial.println(" CONNECTED");

  sntp_set_time_sync_notification_cb( timeavailable );
  sntp_servermode_dhcp(1);    // (optional)

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); 

  lcd.clear();
  lcd.print("CONNECTED");
  delay(timeDelay*2);
  lcd.clear();
}

void loop()
{
  delay(timeDelay);
  printLocalTime();     // it will take some time to sync time :)
}
