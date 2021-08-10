#pragma once

#include <TFT_eSPI.h>
#include "string"
#define TEXT "aA MWyz~12" // Text that will be printed on screen in any font
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include <mutex>          // std::mutex

std::mutex tftMtx;           // mutex for critical section


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

#define TEXT_HEIGHT 8    // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0 // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 0 // Number of lines in top fixed area (lines counted from top of screen)

uint16_t yStart = TOP_FIXED_AREA;
uint16_t yArea = 320 - TOP_FIXED_AREA - BOT_FIXED_AREA;
uint16_t yDraw = 320 - BOT_FIXED_AREA - TEXT_HEIGHT;
byte pos[42];
uint16_t xPos = 0;
int selectedFont = 2;
uint16_t setBackground;

void setupScrollArea(uint16_t TFA, uint16_t BFA);
int scroll_slow(int lines, int wait);
void scrollAddress(uint16_t VSP);
//void showFont();

#define ILI9341_BLACK TFT_BLACK
#define ILI9341_WHITE TFT_WHITE

void tftInit()
{

  //randomSeed(analogRead(A0));
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  //showFont();
}

void tftClear(uint16_t color)
{
  tftMtx.lock();
  setBackground=color;
  tft.fillScreen(color);
  tft.setCursor(0, 0, selectedFont);
   tftMtx.unlock();
}


void tftPrint(String text, int line, uint16_t color, size_t size = 2)
{

 tftMtx.lock();
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(color, setBackground);
  tft.setTextSize(size);
  int fontHeight = tft.fontHeight(selectedFont);
   tft.setCursor(0, line*fontHeight);
  // We can now plot text on screen using the "print" class
  tft.println( text.c_str());
  tftMtx.unlock();
}

void tftFlash(uint16_t color1, uint16_t color2, int times)
{

   tftMtx.lock();
  for (int i = times - 1; i >= 0; i--)
  {
    delay(100);
    tft.fillScreen(color1);
    delay(100);
    tft.fillScreen(color2);
  }
   tft.fillScreen(setBackground);
    tftMtx.unlock();
}

// void showFont()
// {
//   tft.setTextDatum(MC_DATUM);

//   // Set text colour to orange with black background
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);

//   tft.fillScreen(TFT_BLACK);             // Clear screen
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF1, 100, 20, GFXFF);  // Print the string name of the font
//   tft.setFreeFont(FF1);                  // Select the font
//   tft.drawString(TEXT, 100, 60, GFXFF); // Print the string name of the font
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF2, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF2);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF3, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF3);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF4, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF4);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF5, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF5);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF6, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF6);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF7, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF7);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF8, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF8);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                // Select the font
//   tft.drawString(sFF9, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF9);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF10, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF10);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF11, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF11);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF12, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF12);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF13, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF13);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF14, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF14);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF15, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF15);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF16, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF16);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF17, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF17);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF18, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF18);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF19, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF19);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF20, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF20);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF21, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF21);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF22, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF22);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF23, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF23);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF24, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF24);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF25, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF25);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF26, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF26);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF27, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF27);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF28, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF28);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF29, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF29);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF30, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF30);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF31, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF31);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF32, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF32);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF33, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF33);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF34, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF34);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF35, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF35);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF36, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF36);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF37, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF37);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF38, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF38);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF39, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF39);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF40, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF40);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF41, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF41);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF42, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF42);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF43, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF43);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF44, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF44);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF45, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF45);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF46, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF46);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF47, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF47);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
//   //tft.fillScreen(TFT_BLACK);
//   tft.setFreeFont(FF18);                 // Select the font
//   tft.drawString(sFF48, 100, 20, GFXFF); // Print the string name of the font
//   tft.setFreeFont(FF48);
//   tft.drawString(TEXT, 100, 60, GFXFF);
//   delay(1000);
// }