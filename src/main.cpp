#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define POT_PIN 34 // potentiometer wiper input
#define FAN_PIN 16 // fan output

Adafruit_BMP280 bmp; // creating an object called bmp
float setPoint; // creating a variable called "setPoint"

const int PWM_FREQ = 5000; // PWM frequency (Hz)
const int PWM_RES = 12; // 12 bits: duty 0 – 4095
const int PWM_CH = 0; // Channel set to 0

void fanPWM() // fan control function
{
    int potValue = analogRead(POT_PIN); // reading the potentiometer value
    setPoint = potValue / 4095.0 * 10 + 20; // defining the setPoint variable (converting binary to temperature between 20 degrees and 30 degrees)
    float difference = bmp.readTemperature() - setPoint; // defining the difference between the temperature and setPoint variable

    // if the difference is negative, the LED is OFF
        if (difference <= 0)
        {
            potValue = 0;
        }
    // if the difference is above 5 degrees, the LED is completely ON
        else if (difference >= 5)
        {
            potValue = 4095;
        }
    // if the difference is between 0 and 5 degrees, divide the difference by 5 to find the duty cycle, then turn it to a binary value
        else
        {
            potValue = difference / 5 * 4095;
        }

    ledcWrite(PWM_CH, potValue); // writing the PWM

    // printing the duty cycle on the serial monitor
        Serial.print("Duty cycle = ");
        Serial.print(potValue * 100.0 / 4095);
        Serial.println(" %");
}

void setup() // runs once
{
    Serial.begin(115200); // initializing serial communication
    Wire.begin(); // initializing the wire library for I2C communication
    Serial.println("Program Started"); // defines the start of the program on the serial monitor

    // detecting the I2C address being used and checks if bmp is initialized
        if (bmp.begin(0x76)){
        Serial.println("BMP280 Found at 0x76");
        }
        else if (bmp.begin(0x77)){
        Serial.println("BMP280 Found at 0x77");
        }
        else{
        Serial.println("BMP280 Not Found");
        }

    pinMode(POT_PIN, INPUT); // defining the potentiometer pin as an input
    pinMode(FAN_PIN, OUTPUT); // defining the fan pin as an output

    analogReadResolution(12); // setting the ADC resolution to 12 bits (0-4095)

    ledcSetup(PWM_CH, PWM_FREQ, PWM_RES); // setting up the PWM channel
    ledcAttachPin(FAN_PIN, PWM_CH); // connecting the PWM pin to the fan's pin
}

void loop() // runs continuously
{
    int potValue = analogRead(POT_PIN); // reading the potentiometer value
    Serial.print("pot:"); // printing "pot: " on the serial monitor
    Serial.println(potValue); // printing the potentiometer value on the serial monitor
    Serial.print("Temperature: "); // printing "Temperature: " on the serial monitor
    Serial.print(bmp.readTemperature()); // printing the temperature value on the serial monitor
    Serial.println(" C"); // printing the temperature unit on the serial monitor

    fanPWM(); // calling the fanPWM function

    delay(1000); // wait 1 second before repeating loop
}
// end code
