#include <Arduino.h>
#include <DHT.h>

#define POT_PIN 34 // potentiometer wiper input
#define FAN_PIN 16 // fan output
#define TEMP_PIN 17

const int PWM_FREQ = 5000; // PWM frequency (Hz)
const int PWM_RES = 12; // 12 bits: duty 0 – 4095
const int PWM_CH = 0; // Channel set to 0

void fanPWM()
{
    int potValue = analogRead(POT_PIN); // reading the potentiometer value
    int duty = map(potValue, 0, 4095, 0, 4095);

    Serial.print("duty cycle = ");  
    Serial.print(duty * 100 / 4095);
    Serial.println(" %");

    ledcWrite(PWM_CH, duty);
    delay(100);
}

void setup() // runs once
{
    Serial.begin(115200); // initializing serial communication
    
    pinMode(POT_PIN, INPUT); // defining the potentiometer pin as an input
    pinMode(FAN_PIN, OUTPUT); // defining the fan pin as an output
    pinMode(TEMP_PIN, OUTPUT); // defining the temperature sensor as an output

    analogReadResolution(12); // setting the ADC resolution to 12 bits (0-4095)

    ledcSetup(PWM_CH, PWM_FREQ, PWM_RES);
    ledcAttachPin(FAN_PIN, PWM_CH);
}

void loop() // runs continuously
{
    int potValue = analogRead(POT_PIN); // reading the potentiometer value
    Serial.print("pot:");  Serial.println(potValue); // printing the potentiometer value on the serial monitor
    fanPWM(); // calling the fanPWM() function
}
