/***************************************************************************
BOOTS EPOD Monitor - Feb 16, 2023

Use BME280 pressure, temp and humidity sensors to measure these parameters in the BOOTS EPOD

Use BlueRobotics SOS Leak Sensor to detect leaks

Concatenate all data into a string, to be sent our via Arduino Nano hardware serial port (pins 0 & 1) at 9600-8-N-1:

$BOOTS_EPOD_MONITOR,Temp,Pressure,Humidity,Leak_Status.

Send message every 1 sec.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

int leakPin = 2;
int leak = 0; //0 is dry, 1 is wet

Adafruit_BME280 bme; // I2C

unsigned long delayTime;

void setup() {

    pinMode (leakPin, INPUT);
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    unsigned status;
    
    // default settings
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();
}


void loop() { 
    printValues();
    leak = digitalRead(leakPin);
    if(leak == 1){
      Serial.println(",Leak Detected");
    }
    else{
      Serial.println(",OK");      
    }
        
    delay(delayTime);
}


void printValues() {
    Serial.print("$BOOTS_EPOD_MONITOR,");
    Serial.print(bme.readTemperature());
    Serial.print(",");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(",");
    Serial.print(bme.readHumidity());
}
