#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

Adafruit_BMP280 bmp;

// OLED values, pins, object (till line #) - Lab 8 [#]
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_DC 16
#define OLED_RESET 17
#define OLED_CS 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

float temp_readings[10];
int temp_count = 0;     // Used to cycle through array positions

// Periodic temperature reading concept taken from Lab 9 [#]
// Lines 35-36 and #-#
unsigned long tempReadInterval = 2000;
unsigned long lastTempReadTime;     // Will store value from millis()

void readTemp() {
    lastTempReadTime = millis();
    temp_readings[temp_count] = bmp.readTemperature();
    if (temp_count == 9){
        temp_count = 0;
    }
    else {
        temp_count ++;
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // BMP Initialization taken from Lab 7 [#]
    if(bmp.begin(0x76)) {
        Serial.println("BMP280 Found");
    }
    else {
        Serial.println("BMP280 Not Found");
    }

    // Display cmds from Lab 8 [#]
    display.begin(SSD1306_SWITCHCAPVCC);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Room status");
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
    display.display();
}

void loop() {
    if (millis() - lastTempReadTime >= tempReadInterval) {
        readTemp();
        // Temporarily printing to serial until OLED is fully setup
        // Note: reads an invalid position when resets to 0
        // and does not read position 9, but will not fix since temporary
        Serial.print("Temp: ");
        Serial.println(temp_readings[temp_count - 1]);
    }
  
}
