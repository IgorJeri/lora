#pragma once
#include "config.hpp"
#include <SPI.h>
#include <LoRa.h>

void onReceive(int packetSize);

struct GetDistanceResult
{
    int cm;
    int packetRssi;
    bool success;
    int lastUpdate;
};

void (*funct)(GetDistanceResult*);
//static GetDistanceResult distanceResult;
SPIClass *vspi = NULL;
void loraSetup(void (*callFunction)(GetDistanceResult*))
{
    funct = callFunction;

    Serial.println("LoRa Initializing...");
    vspi = new SPIClass(HSPI);
    vspi->begin(SCK_GPIO, MISO_GPIO, MOSI_GPIO, NSS_GPIO); //SCLK, MISO, MOSI, SS

    pinMode(NSS_GPIO, OUTPUT); //VSPI SS

    //setup LoRa transceiver module
    LoRa.setPins(NSS_GPIO, RESET_GPIO, DIO0_GPIO);
    LoRa.setSPI(*vspi);

    // replace the LoRa.begin(---E-) argument with your location's frequency
    // 433E6 for Asia
    // 866E6 for Europe
    // 915E6 for North America
    while (!LoRa.begin(433E6))
    {
        Serial.println(".");
        delay(500);
    }
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");
    LoRa.onReceive(onReceive);
    //LoRa.setGain(6);
    LoRa.receive();
}

void pushTask(void * params)
{

    GetDistanceResult *distanceResult = (GetDistanceResult*) params;
    
    funct(distanceResult);
    delete distanceResult;
    vTaskDelete(NULL);

}

void onReceive(int packetSize)
{

    GetDistanceResult *distanceResult = new GetDistanceResult();
    // received a packet
    while (LoRa.available())
    {
        String LoRaData = LoRa.readString();
        //Save result data
        distanceResult->cm = LoRaData.toInt();
        distanceResult->packetRssi = LoRa.packetRssi();
        distanceResult->success = true;
        distanceResult->lastUpdate = esp_timer_get_time();
        //Serial out for debug
        Serial.print("LoRaData '" + LoRaData);
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }

    xTaskCreatePinnedToCore(pushTask, "pushTask",5000,distanceResult, 1,NULL,1);
    
}



