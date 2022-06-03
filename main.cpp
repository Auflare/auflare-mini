/* ------------------------------------------------------------ 
 * Auflare Mini
 * Written by Cohen Coombs 2022 (c) All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and Confidential
------------------------------------------------------------ */

#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <SD.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define SD_CS 10

#define YP A2
#define XM A3
#define YM 8
#define XP 9

#define BUFFPIXEL 2
#define background 0x18C3
#define shade1 0x2945
#define shade2 0x4208
#define shade3 0x4A69
#define text 0xFFFF
#define yellow 0xFEC7

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, A4);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

String version = "4.20";
String current = "home";
String input = "";

int touched = 0;
int p1 = 0;
char oper = ' ';
String p2 = "";

int freeMemory() {
  extern int __heap_start, * __brkval;
  int v;
  return (int) & v - (__brkval == 0 ? (int) & __heap_start : (int) __brkval);
}

int updateScreen() {
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
}

int print(String value, int x, int y, int size) {
  tft.setTextColor(text);
  tft.setCursor(x, y);
  tft.setTextSize(size);
  tft.print(value);
}

int unlock() {
  updateScreen();
  tft.fillScreen(background);
  image("home.bmp", 0, 0);
  tft.fillRoundRect(60, tft.height() - 25, 120, 6, 100, text);
  current = "start";
}

void setup() {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x7575);
  tft.setRotation(0);
  SD.begin(SD_CS);
  unlock();
}

void loop() {
  if (touched > 0) touched--;
  TSPoint p = ts.getPoint();
  if (p.z > 0 && touched == 0) {
    touched = 80;
    p.x = map(p.x, 948, 204, 0, 320);
    p.y = map(p.y, 910, 195, 0, 240);

    // Serial.print("X = ");
    // Serial.print(p.x);
    // Serial.print("\tY = ");
    // Serial.print(p.y);
    // Serial.print("\tForce = ");
    // Serial.print(p.z);
    // Serial.print("\n");

    if (p.x > 300 && p.y > 70 && p.y < 200 && current != "home") apps();
    
    // Row 1
    if (p.x > 110 && p.x < 150 && p.y > 140 && p.y < 240 && current == "home") {
      page(background);
      image("settings.bmp", 10, 20);
      tft.setTextSize(2);
      for (int i = 0; i < 6; i++) {
        tft.fillRoundRect(10, 95 + (i * 32), tft.width() - 20, 25, 5, shade1);
        tft.setCursor(25, 100 + (i * 32));

        if (i == 0) tft.print("Version: " + version);
        else if (i == 1) tft.print("OS: Auflare");
        else if (i == 2) tft.print("Display: 240x320");
        else if (i == 3) {
          tft.print("EEPROM: ");
          tft.print(freeMemory());
        }
        else if (i == 4) tft.print("Memory: 32256");
        else if (i == 5) {
          tft.print("Runtime: ");
          tft.print(millis() / 1000);
          tft.print("s");
        }
      }
    }
    if (p.x > 110 && p.x < 150 && p.y > 10 && p.y < 110 && current == "home") {
      page(background);
      image("daymap.bmp", 0, 0);
    }
    // Row 2
    if (p.x > 175 && p.x < 220 && p.y > 140 && p.y < 240 && current == "home") {
      page(background);
      calculator();
      current = "calculator";
    }
    if (p.x > 175 && p.x < 220 && p.y > 10 && p.y < 110 && current == "home") {
      page(background);
      current = "timer";
    }
    // Row 3
    if (p.x > 240 && p.x < 285 && p.y > 140 && p.y < 240 && current == "home") {
      page(background);
      openFiles(SD.open("/"));
    }

    if (current == "calculator") {
      if (p.x > 85 && p.x < 125 && p.y > 210 && p.y < 245) {input += "1"; p2 += "1"; updateCalc(); }
      if (p.x > 85 && p.x < 125 && p.y > 150 && p.y < 185) {input += "2"; p2 += "2"; updateCalc(); }
      if (p.x > 85 && p.x < 125 && p.y > 85 && p.y < 120) {input += "3"; p2 += "3"; updateCalc(); }
      if (p.x > 85 && p.x < 125 && p.y > 10 && p.y < 45) {
        p1 = stringInt(input);
        input += "+"; updateCalc(); 
        oper = '+';
        p2 = "";
      }
      
      if (p.x > 145 && p.x < 185 && p.y > 205 && p.y < 245) {input += "4"; p2 += "4"; updateCalc(); }
      if (p.x > 145 && p.x < 185 && p.y > 150 && p.y < 185) {input += "5"; p2 += "5"; updateCalc(); }
      if (p.x > 145 && p.x < 185 && p.y > 85 && p.y < 120) {input += "6"; p2 += "6"; updateCalc(); }
      if (p.x > 145 && p.x < 185 && p.y > 10 && p.y < 45) {
        p1 = stringInt(input);
        input += "-"; updateCalc(); 
        oper = '-';
        p2 = "";
      }

      if (p.x > 205 && p.x < 245 && p.y > 210 && p.y < 245) {input += "7"; p2 += "7"; updateCalc(); }
      if (p.x > 205 && p.x < 245 && p.y > 150 && p.y < 185) {input += "8"; p2 += "8"; updateCalc(); }
      if (p.x > 205 && p.x < 245 && p.y > 85 && p.y < 120) {input += "9"; p2 += "9"; updateCalc(); }
      if (p.x > 205 && p.x < 245 && p.y > 10 && p.y < 45) {
        p1 = stringInt(input);
        oper = '*';
        input += "x"; updateCalc(); 
        p2 = "";
      }

      if (p.x > 265 && p.x < 305 && p.y > 210 && p.y < 245) { calculator(); }
      if (p.x > 265 && p.x < 305 && p.y > 150 && p.y < 185) { input += "0"; p2 += "0"; updateCalc(); }
      if (p.x > 265 && p.x < 305 && p.y > 85 && p.y < 120) { 
        input = calculate(p1, oper, stringInt(p2)); updateCalc(); // "="
      }
      if (p.x > 265 && p.x < 305 && p.y > 10 && p.y < 45) {
        p1 = stringInt(input);
        oper = '/';
        input += "/" ; updateCalc(); 
        p2 = "";
      }
    }
  }

  if (current == "timer" && millis() % 20 == 0) {
    updateScreen();
    int hours = millis() / 3600000;
    int minutes = (millis() % 3600000) / 60000;
    int seconds = (millis() % 60000) / 1000;
    int milliseconds = ((millis() % 1000) + 5) / 10;

    tft.fillRect(0, 0, tft.width(), 50, shade1);
    tft.fillRect(0, 48, tft.width(), 2, shade3);
    tft.setCursor(20, 15);
    tft.setTextSize(3);
    tft.print(hours);
    tft.print(":");
    tft.print(minutes);
    tft.print(":");
    tft.print(seconds);
    tft.print(".");
    tft.print(milliseconds);
  }
}

int stringInt(String str) {
  int num = 0;
  for (int i = 0; i < str.length(); i++) num = num * 10 + (str[i] - '0');
  return num;
}

int calculate(int one, char op, int two) {
  if (op == '+') return one + two;
  else if (op == '-') return one - two;
  else if (op == '*') return one * two;
  else if (op == '/') return one / two;
  else return 0;
}

int updateCalc() {
  updateScreen();
  tft.fillRect(0, 0, tft.width(), 50, shade1);
  tft.fillRect(0, 48, tft.width(), 2, shade3);
  tft.setCursor(20, 15);
  tft.setTextSize(3);
  if (input.length() > 11) tft.print(input.substring(input.length() - 11, input.length()));
  else tft.print(input);
  if (input == "") tft.print("0");
}

int page(uint16_t color) {
  updateScreen();
  current = "";
  tft.fillScreen(color);
  tft.fillRoundRect(60, tft.height() - 25, 120, 6, 100, text);
}

int apps() {
  page(background);
  image("apps.bmp", 10, 20);
  tft.fillRoundRect(10, 70, tft.width() - 20, 3, 5, shade2);
  image("r1.bmp", 10, 100);
  image("r2.bmp", 10, 160);
  image("r3.bmp", 10, 220);
  current = "home";
}

int calculator() {
  updateCalc();
  p1 = 0;
  oper = ' ';
  input = "";
  p2 = "";
  int btn = 0;
  int gap = 55;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {btn++;
      tft.setTextSize(2);
      tft.fillCircle(35 + (gap * j), 95 + (gap * i), 20, shade3);
      tft.setCursor(30 + (gap * j), 90 + (gap * i));
      tft.print(btn);
    }
  }
  tft.fillCircle(tft.width()-39, 95, 20, yellow);
  print("+", tft.width()-44, 88, 2);
  tft.fillCircle(tft.width()-39, 150, 20, yellow);
  print("-", tft.width()-44, 143, 2);
  tft.fillCircle(tft.width()-39, 205, 20, yellow);
  print("x", tft.width()-44, 198, 2);
  tft.fillCircle(tft.width()-39, 260, 20, yellow);
  print("/", tft.width()-44, 253, 2);
  tft.fillCircle(tft.width()-97, 260, 20, shade3);
  print("=", tft.width()-102, 253, 2);
  tft.fillCircle(88, 260, 20, shade3);
  print("0", 83, 253, 2);
  tft.fillCircle(35, 260, 20, shade3);
  print("C", 30, 253, 2);
}

void openFiles(File dir) {
  int inc = 15;
  int shelf = inc;
  int fullShelf = tft.height() - 40;
  bool first = true;

  while (true) { shelf += inc;
    File entry = dir.openNextFile();
    if (!entry) break;
    if (entry.name()[0] == '_') {
      entry.close();
      continue;
    }
    if (shelf < fullShelf && first) print(entry.name(), 10, shelf, 1);
    else {
      if (shelf > fullShelf) {
        shelf = inc*2;
        first = false;
      }
      print(entry.name(), 120, shelf, 1);
    }
    entry.close();
  }
}

void image(String filename, int x, int y) {
  File bmpFile;
  int bmpWidth, bmpHeight;
  uint8_t bmpDepth;
  uint32_t bmpImageoffset;
  uint32_t rowSize;
  uint8_t sdbuffer[3 * BUFFPIXEL];
  uint16_t lcdbuffer[BUFFPIXEL];
  uint8_t buffidx = sizeof(sdbuffer);
  boolean flip = true;
  int w, h, row, col;
  uint8_t r, g, b;
  uint32_t pos = 0;
  uint8_t lcdidx = 0;
  boolean first = true;

  bmpFile = SD.open(filename);

  if (read16(bmpFile) == 0x4D42) {
    read32(bmpFile);
    (void) read32(bmpFile);
    bmpImageoffset = read32(bmpFile);
    read32(bmpFile);

    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) {
      bmpDepth = read16(bmpFile);

      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) {
        rowSize = (bmpWidth * 3 + 3) & ~3;
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width()) w = tft.width() - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) {
          if (flip) pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) {
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer);
          }

          for (col = 0; col < w; col++) {
            if (buffidx >= sizeof(sdbuffer)) {

              if (lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0;
            }

            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r, g, b);
          }
        }
        if (lcdidx > 0) tft.pushColors(lcdbuffer, lcdidx, first);
      }
    }
  }
  bmpFile.close();
}
uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t * ) & result)[0] = f.read();
  ((uint8_t * ) & result)[1] = f.read();
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t * ) & result)[0] = f.read();
  ((uint8_t * ) & result)[1] = f.read();
  ((uint8_t * ) & result)[2] = f.read();
  ((uint8_t * ) & result)[3] = f.read();
  return result;
}
