#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <SD.h>
// #include <Fonts/Auflare.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define SD_CS 10   

#define TS_MINX 204
#define TS_MINY 195
#define TS_MAXX 948
#define TS_MAXY 910

#define YP A2
#define XM A3
#define YM 8
#define XP 9

#define banner 0x5C5F
#define background 0x18C3
#define component 0x31A6
#define shade1 0x2945
#define shade2 0x4208
#define shade3 0x4A69
#define text 0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, A4);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define BUFFPIXEL 80
float version = 3.2;
String current = "home";
String input = "";

int freeMemory() {
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
int updateScreen() {
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
}
int randColor() {
    return tft.color565(random(30, 230), random(30, 230), random(30, 230));
}

int unlock() {
  updateScreen();
  image("welcome.bmp", 0, 0);
  tft.setTextSize(2);

  current = "start";
}

void setup() {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x7575);
  tft.setRotation(0);
  tft.setFont();
  tft.setTextColor(text);

  if (!SD.begin(SD_CS)) {
    Serial.println(F("SD Error"));
    return;
  }
  tft.fillScreen(0x0000);

  image("brand.bmp", 26, 140);
  delay(300);
  unlock();
}

void loop() {
    TSPoint p = ts.getPoint();
    if (p.z > 0) {
        p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
        p.y = map(p.y, TS_MAXY, TS_MINY, 0, 240);

        // Serial.print("X = ");
        // Serial.print(p.x);
        // Serial.print("\tY = ");
        // Serial.print(p.y);
        // Serial.print("\tForce = ");
        // Serial.print(p.z);
        // Serial.print("\n");

        if (p.x > 300 && p.y > 70 && p.y < 200 && current != "home") home();
        if (p.x < 25 && current != "start") unlock();

        // Row 1
        if (p.x > 60 && p.x < 110 && p.y > 140 && p.y < 240 && current == "home") {
            page(background);
            image("settings.bmp", 0, 0);
        }
        if (p.x > 60 && p.x < 110 && p.y > 20 && p.y < 120 && current == "home") {
            page(background);
            app("Notes", "Create a note");
        }
        // Row 2
        if (p.x > 125 && p.x < 180 && p.y > 140 && p.y < 240 && current == "home") {
            page(background);
            current = "calculator";
        }
        if (p.x > 125 && p.x < 180 && p.y > 20 && p.y < 120 && current == "home") {
            page(background);
            current = "timer";
        }
        // Row 3
        if (p.x > 190 && p.x < 245 && p.y > 140 && p.y < 240 && current == "home") {
            page(background);
            app("App", "Please install an app.");
        }
        if (p.x > 190 && p.x < 245 && p.y > 20 && p.y < 120 && current == "home") {
            page(background);
            app("App", "Please install an app.");
        }
        // Row 4
        if (p.x > 255 && p.x < 310 && p.y > 140 && p.y < 240 && current == "home") {
            page(background);
            app("App", "Please install an app.");
        }

        if (p.x > 255 && p.x < 310 && p.y > 20 && p.y < 120 && current == "home") {
            page(background);
            app("App", "Please install an app.");
        }

        if (p.x > 100 && p.x < 150 && p.y > 210 && p.y < 250 && current == "calculator") input += "1";
        if (p.x > 100 && p.x < 150 && p.y > 140 && p.y < 200 && current == "calculator") input += "2";
        if (p.x > 100 && p.x < 150 && p.y > 70 && p.y < 120 && current == "calculator") input += "3";
        if (p.x > 160 && p.x < 210 && p.y > 210 && p.y < 250 && current == "calculator") input += "4";
        if (p.x > 160 && p.x < 210 && p.y > 140 && p.y < 200 && current == "calculator") input += "5";
        if (p.x > 160 && p.x < 210 && p.y > 70 && p.y < 120 && current == "calculator") input += "6";
        if (p.x > 220 && p.x < 270 && p.y > 210 && p.y < 250 && current == "calculator") input += "7";
        if (p.x > 220 && p.x < 270 && p.y > 140 && p.y < 200 && current == "calculator") input += "8";
        if (p.x > 220 && p.x < 270 && p.y > 70 && p.y < 120 && current == "calculator") input += "9";
        if (p.x > 280 && p.x < 320 && p.y > 210 && p.y < 250 && current == "calculator") input += "0";
        if (p.x > 300 && p.x < 350 && p.y > 0 && p.y < 50 && current == "calculator") input = "";

        if (current == "calculator") {
            updateScreen();
            tft.fillRoundRect(8, 15, tft.width() - 15, 50, 5, shade1);
            tft.setCursor(15, 54);
            if (input.length() > 7) tft.print(input.substring(input.length() - 7, input.length()));
            else tft.print(input);
            if (input == "") tft.print("0");
        }
    }

    if (current == "timer") {
        updateScreen();
        int hours = millis() / 3600000;
        int minutes = (millis() % 3600000) / 60000;
        int seconds = (millis() % 60000) / 1000;
        int milliseconds = ((millis() % 1000) + 5) / 10;

        tft.fillRoundRect(10, 15, tft.width() - 20, 50, 5, component);
        tft.setCursor(20, 52);
        tft.print(hours); tft.print(":"); tft.print(minutes); tft.print(":"); tft.print(seconds); tft.print("."); tft.print(milliseconds);
    }
}

int app(String name, String desc) {
    tft.fillRoundRect(20, 70, tft.width() - 40, 2, 5, shade2);
    print(name, 25, 57, "title");
    print(desc, 20, 95, "normal");
}

int home() {
    page(background);
    apps();
    current = "home";
}

int page(uint16_t color) {
    updateScreen();
    current = "";
    tft.fillScreen(color);
    tft.fillRoundRect(60, tft.height() - 25, 120, 6, 100, text);
}

int print(String name, int x, int y, String size) {
    tft.setCursor(x, y);
    tft.setFont();
    tft.print(name);
}

int apps() {
    String names[8] = {"Settings", "Notes", "Calculator", "Timer", "App", "App", "App", "App"};
    int p = 0;
    tft.fillRect(0, 34, tft.width(), 1, shade3);
    print("Recent Apps", 15, 21, "normal");

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            tft.drawRoundRect(15 + j * 110, 50 + i * 60, 100, 50, 5, shade2);
            tft.fillCircle(40 + j * 110, 75 + i * 60, 18, randColor());
            print(names[p], 25 + j * 110, 80 + i * 60, "normal");
            p++;
        }
    }
}

void image(char *filename, int x, int y) {
  File     bmpFile;
  int      bmpWidth, bmpHeight;  
  uint8_t  bmpDepth;             
  uint32_t bmpImageoffset;       
  uint32_t rowSize;              
  uint8_t  sdbuffer[3*BUFFPIXEL];
  uint16_t lcdbuffer[BUFFPIXEL]; 
  uint8_t  buffidx = sizeof(sdbuffer);
  boolean  goodBmp = false;      
  boolean  flip    = true;       
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if ((x >= tft.width()) || (y >= tft.height())) return;
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.println(F("File not found"));
    return;
  }

  if (read16(bmpFile) == 0x4D42) {
    Serial.println(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile);
    bmpImageoffset = read32(bmpFile);
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));

    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) {
      bmpDepth = read16(bmpFile);
      
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) {

        goodBmp = true;
        rowSize = (bmpWidth * 3 + 3) & ~3;      
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x+w-1) >= tft.width())  w = tft.width()  - x;
        if ((y+h-1) >= tft.height()) h = tft.height() - y;

        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) {
          if (flip) pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) {
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer);
          }

          for (col=0; col<w; col++) {
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
            lcdbuffer[lcdidx++] = tft.color565(r,g,b);
          }
        }
        if (lcdidx > 0) tft.pushColors(lcdbuffer, lcdidx, first);
      }
    }
  }
  bmpFile.close();
  if (!goodBmp) Serial.println(F("BMP format not recognized."));
}

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}
