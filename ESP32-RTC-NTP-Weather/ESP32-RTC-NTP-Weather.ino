#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

// WiFi Credentials
const char* ssid = "xxxx";
const char* password = "yyyy";

// OpenWeather
const char* apiKey = "aaaaaa";
const float latitude = 47.68;
const float longitude = -122.16;

LiquidCrystal_I2C lcd(0x27, 16, 2);   //LCD Object

// Timezone Offset (in seconds)
const long gmtOffset_sec = -8 * 3600;          // Example: GMT-8 for PDT
const int daylightOffset_sec = 3600;           // DST (use 0 if not needed)
const char* ntpServer = "pool.ntp.org";

float currentTempF = 0.0;

void updateClockDisplay() {
  time_t now = time(nullptr);
  struct tm* timeInfo = localtime(&now);

  char timeBuffer[17];

  strftime(timeBuffer, sizeof(timeBuffer), "%b %d, %Y", timeInfo);
  lcd.setCursor(2, 0);
  lcd.print(timeBuffer);

  strftime(timeBuffer, sizeof(timeBuffer), "%a, %H:%M ", timeInfo);
  lcd.setCursor(0, 1);
  lcd.print(timeBuffer);
  lcd.print(currentTempF, 1);
  lcd.print("F");
}

void getWeather() {
  String weatherUrl = "http://api.openweathermap.org/data/2.5/weather?lat=" +
                      String(latitude, 4) + "&lon=" + String(longitude, 4) +
                      "&appid=" + String(apiKey) + "&units=imperial";

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(weatherUrl);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      currentTempF = doc["main"]["temp"];
      updateClockDisplay(); // Update LCD with latest temp too
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi lost!");
  }
}

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");

  // Sync time from NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print("#");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time synced");

  getWeather(); // Initial temperature fetch

  lcd.clear();
}

void loop() {
  time_t now = time(nullptr);
  struct tm* timeInfo = localtime(&now);

  if (timeInfo->tm_min % 15 == 0) {
    getWeather();                                                  
  }         

  updateClockDisplay();                              
  delay((60 - timeInfo->tm_sec) * 1000);
}
