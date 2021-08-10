/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/
#include <iostream>
#include <sstream>
#include "string"
#include <SPI.h>
#include <LoRa.h>
#include "../.pio/libdeps/esp32dev/TinyGPSPlus/src/TinyGPS++.h"

#include "../.pio/libdeps/esp32dev/EspSoftwareSerial/src/SoftwareSerial.h"
#include <HardwareSerial.h>
#include "config.hpp"
#include "power.hpp"
#include "gps.hpp"
#include "lora.hpp"
#include "led.hpp"
#include "tft.hpp"
#include "button.hpp"
#include <TFT_eSPI.h>
#include "esp_timer.h"

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
void DistanceReceived(GetDistanceResult *distRec);
void CheckSignal(void *params);

int counter = 0;
int red_light_pin = 11;
int green_light_pin = 10;
int blue_light_pin = 9;
int setDistance = 20;
int cm;

//#define MODESEND
double maxSpeed = 0;
bool sendMode = false;
String currentStatus;
volatile int64_t lastUpdate = 0;
int timeSinceLastUpdate = 0;
//static GetDistanceResult distanceResult;

// SPIClass *vspi = NULL;

template <typename T>
std::string ToString(T val)
{
  std::stringstream stream;
  stream << val;
  return stream.str();
}

void buttonTapped(Button2 &btn)
{
  setDistance = cm;
  Serial.printf("Distance set to [%d]", cm);
  Serial.println();
}

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

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

#ifdef LED_PIN
  pinMode(LED_PIN, OUTPUT);
#endif

  // Init GPS
  gps_setup();

  rgbInit();
  tftInit();
  loraSetup(DistanceReceived);
  buttonInit(buttonTapped);

  xTaskCreatePinnedToCore(CheckSignal, "CheckSignal", 5000, NULL, 1, NULL, 0);
}

int calculateTimeSinceLastUpdate()
{
  return (esp_timer_get_time() - lastUpdate) / 1000000;
}

void CheckSignal(void *params)
{
  Serial.println("CheckSignal() Started");
  while (1)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS); //wait x seconds

    //Serial.println("Running CheckSignal()");

    timeSinceLastUpdate = calculateTimeSinceLastUpdate();
    tftPrint("LU:" + String(timeSinceLastUpdate) + "                 ", 0, TFT_CYAN, 2);

    //Serial.println("Last Update :" + String(timeSinceLastUpdate));
    if (timeSinceLastUpdate > 20) //> 1 Second
    {
      Serial.println("Timout of signal!!");
      tftPrint("Signal Lost                    ", 1, TFT_PINK, 2);
      flashRGBDontWait({255, 0, 0}, {1, 1, 1}, 100);
      tftPrint("Door Status: Unknown            ", 2, TFT_RED, 2);
      tftPrint("cm: ---                         ", 3, TFT_WHITE, 2);
    }
  }
}

String getState(int cm)
{
  if (cm < setDistance && cm != 0)
  {
    return "Open";
  }
  else
  {
    return "Closed";
  }
}

void OutputStatus(String status)
{

  Serial.println("Status: " + status);
  uint16_t stateColor = TFT_GREENYELLOW;
  if (status == "Open")
  {
    flashRGBDontWait({255, 10, 200}, {0, 0, 1}, 100);
    stateColor = TFT_RED;
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
  else if (status == "Closed")
  {
    stateColor = TFT_GREEN;
  }
  else if (status = "Unknown")
  {
    flashRGBDontWait({255, 0, 0}, {1, 1, 1}, 100);
    stateColor = TFT_RED;
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
  tftPrint("Door Status:" + status + "            ", 2, stateColor, 2);
}

int distanceReceivedCounter = 0;
void DistanceReceived(GetDistanceResult *distRec)
{
  
  distanceReceivedCounter++;
   flashRGB({0, 255, 255}, {0, 0, 0}, 1);
   
  //setRGB(0, 255, 255);
  //Serial.println("on");
  //vTaskDelay(10000 / portTICK_PERIOD_MS);
  //Serial.println("off");
  //setRGB(0, 0, 0);

  // setRGB(0, 0, 0);
  //vTaskDelay(1000);
  Serial.println("---------------------------------------------------");
  Serial.printf("%d - DistanceReceived() \n", distanceReceivedCounter);
  Serial.println("---------------------------------------------------");


  tftPrint("LU: 0s                  ", 0, TFT_SKYBLUE);
  tftPrint("Signal Ok :" + String(distRec->packetRssi) + " Rssi                  ", 1, TFT_GREEN);
  tftPrint("Distance  :" + String(distRec->cm) + " cm                  ", 3, TFT_SKYBLUE);
  OutputStatus(getState(distRec->cm));
  lastUpdate = esp_timer_get_time();
}

void loop()
{
  vTaskDelay(100000 / portTICK_RATE_MS);
  Serial.println("loop");
}