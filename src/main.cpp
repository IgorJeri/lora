/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>
#include "../.pio/libdeps/esp32dev/TinyGPSPlus/src/TinyGPS++.h"

#include "../.pio/libdeps/esp32dev/EspSoftwareSerial/src/SoftwareSerial.h"
#include <HardwareSerial.h>
#include "config.hpp"
#include "power.hpp"
#include "gps.hpp"
#include "lora.hpp"
#ifndef D5
#if defined(ESP8266)
#define D5 (14)
#define D6 (12)
#elif defined(ESP32)
#define D5 (18)
#define D6 (19)
#endif
#endif

void buildPacket(uint8_t txBuffer[]); // needed for platformio

int counter = 0;

// SPIClass *vspi = NULL;

void setup()
{
  //initialize Serial Monitor
 Serial.begin(115200);

  while (!Serial)
    ;

 Wire.begin(I2C_SDA, I2C_SCL);
scanI2Cdevice();

  axp192Init();

  // Buttons & LED
  pinMode(BUTTON_PIN, INPUT_PULLUP);

#ifdef LED_PIN
  pinMode(LED_PIN, OUTPUT);
#endif

// Init GPS
  gps_setup();
  loraSetup();
 
}

double maxSpeed = 0;
bool sendMode = false;

void loop()
{

  gps_loop();



  if (sendMode == true)
  {
    Serial.print("Sending packet: ");
    Serial.println(counter);
    double currentSpeed;

    // byte gpsData = Serial2.read();
    // Serial.println("GPS Stream:");
    // Serial.write(gpsData);
    // Serial.println("");

    bool recebido = false;
  
    if (_gps.location.isUpdated() && _gps.altitude.isUpdated())
    {
      currentSpeed = _gps.speed.kmph();

      Serial.print("speed: ");
      Serial.println(currentSpeed);

     if(currentSpeed > maxSpeed) maxSpeed = currentSpeed;
      LoRa.beginPacket();
      LoRa.print("Speed: ");
      LoRa.print(currentSpeed);

       LoRa.print(" kmph  ---  Max Speed: ");
      LoRa.print(maxSpeed);
      LoRa.print(" kmph  ");
      LoRa.endPacket();

      


      Serial.print("D/M/A: ");
      Serial.print(_gps.date.value());
      Serial.print(" | alt: ");
      Serial.print(_gps.altitude.feet());
      Serial.print(" | satellites: ");
      Serial.println(_gps.satellites.value());
    }
    else
    {
      Serial.print("Sending packet: ");
      Serial.println("No GPS");

      LoRa.beginPacket();
      LoRa.print("No GPS - ");
      LoRa.print(counter);
      LoRa.endPacket();
    }

    counter++;
    delay(1000);
  }
  else
  {
    // delay(5000);

    // receive

    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
      // received a packet
      Serial.print("Received packet '");

      // read packet
      while (LoRa.available())
      {
        String LoRaData = LoRa.readString();
        Serial.print(LoRaData);
      }

      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    }
  }
}