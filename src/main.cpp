#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

Adafruit_BMP280 bmp;

// OLED values, pins, object (till line #) - Lab 6 [#]
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI 11
#define OLED_CLK 13
#define OLED_DC 9
#define OLED_CS 10
#define OLED_RST 8

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  OLED_MOSI,
  OLED_CLK,
  OLED_DC,
  OLED_RST,
  OLED_CS
);

float temp_readings[10];

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // BMP Initialization taken from Lab 7 [7]
    if(bmp.begin(0x76)) {
        Serial.println("BMP280 Found");
    }
    else {
        Serial.println("BMP280 Not Found");
    }

    // Display cmds from Lab 6 [#]
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
  
}
