#pragma once
#include "config.hpp"
#include <SPI.h>
#include <LoRa.h>

SPIClass *vspi = NULL;
void loraSetup()
{
     Serial.println("LoRa Initializing...");
    vspi = new SPIClass(VSPI);
    vspi->begin(SCK_GPIO, MISO_GPIO, MOSI_GPIO, NSS_GPIO); //SCLK, MISO, MOSI, SS

    pinMode(NSS_GPIO, OUTPUT); //VSPI SS

    //setup LoRa transceiver module
    LoRa.setPins(NSS_GPIO, RESET_GPIO, DIO0_GPIO);
    LoRa.setSPI(*vspi);

    // replace the LoRa.begin(---E-) argument with your location's frequency
    // 433E6 for Asia
    // 866E6 for Europe
    // 915E6 for North America
    while (!LoRa.begin(915E6))
    {
        Serial.println(".");
        delay(500);
    }
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");
}