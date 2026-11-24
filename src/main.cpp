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

// Global variables
const int num_readings = 10;
float temp_readings[num_readings];
int temp_count = 0;     // Used to cycle through array positions
float average_temp = 0;

// Periodic temperature reading concept taken from Lab 9 [#]
// Lines 28-29, 32 and 98 (if statement)
unsigned long tempReadInterval = 2000;  // In ms
unsigned long lastTempReadTime;     // Will store value from millis()

void readTemp() {
    lastTempReadTime = millis();

    // Saves reading to current array position
    temp_readings[temp_count] = bmp.readTemperature();

    // Once reaches end of array, starts saving readings from beginning again
    if (temp_count == 9){
        temp_count = 0;
    }
    else {
        temp_count ++;
    }
}

void calAvgTemp() {
    float total_temp = 0;
    // Add up the values of the array
    for (int i = 0; i < 10; i++) {
        total_temp += temp_readings[i];
    }
    average_temp = total_temp / num_readings;
}

void tempToOLED() {
    // Display cmds from Lab 8 [#]
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Room status");
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
    
    display.setCursor(0, 16);
    display.print("Temp.: ");
    display.print(average_temp);
    display.println(" C");
    display.display();
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
    
    display.begin(SSD1306_SWITCHCAPVCC);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Room status");
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
    display.setCursor(0, 16);
    display.println("Initializing...");
    display.display();
}

void loop() {
    // Wait until we have the first 10 readings before printing to OLED
    // Otherwise average temp. printed is inaccurate (takes 20s)
    while (temp_readings[9] == 0) {
        if (millis() - lastTempReadTime >= tempReadInterval) {
            readTemp();
        }  
    }

    if (millis() - lastTempReadTime >= tempReadInterval) {
        readTemp();
        calAvgTemp();
        tempToOLED();
    }
}
