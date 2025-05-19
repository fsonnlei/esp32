# Adventures with the ESP32 (and its variants) integrating different accessories

## ESP32 Time and Temperature Display with LCD

This project uses an ESP32 SoC, especifically a [ESP-WROOM-32](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf) to display the current time and local temperature on an LCD screen. The ESP32 connects to Wi-Fi to synchronize time via NTP (Network Time Protocol) and retrieve weather data from the OpenWeather API.

### Key Features:
- Wi-Fi Time Sync: Uses the ESP32’s Wi-Fi capabilities to synchronize time with an NTP server for accurate real-time clock functionality.

- Live Weather Data: Fetches the current temperature using the OpenWeather API based on your chosen city or GPS coordinates.

- LCD Output: Displays the time and temperature on a 16x2 or 20x4 LCD using an I2C backpack for simplified wiring.

- Efficient Power Use: Ideal for low-power applications with deep sleep options for battery operation (optional).

### Tools & Technologies:
#### Hardware:

- ESP32 development board (e.g., ESP-WROOM-32)

- I2C-compatible 16x2 or 20x4 LCD screen

#### Software:

- Arduino IDE

- NTPClient and WiFi libraries

- HTTPClient and ArduinoJson for API calls

- LiquidCrystal_I2C for LCD interface

#### Service:

- OpenWeatherMap API (free API key required)

### Use Case:
This compact, Wi-Fi-enabled project is perfect for desks, workshops, or entryways, offering at-a-glance time and temperature without needing a smartphone or PC. 
