#include "OLED.h"
#include <U8g2lib.h>

#define HORIZONTAL 0
#define VERTICAL 1
#define BASE_HOME 0
#define BASE_MENU 10
#define BASE_BLE 20
#define BASE_FLAT_CALI 30
// #define OFFSET_VIKCY 0
#define OFFSET_ANGLE 0
#define OFFSET_SLOPE 1
#define OFFSET_FLATNESS 3
#define OFFSET_FLAT_SLOPE 4
#define ShowStableCountNum 10

/* display is primary;display2 is second*/
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI display(U8G2_R0, /*CLK*/ 36, /*SDA*/ 35,
                                               /*CS*/ 3, /*DC*/ 15, /*RST*/ 41);

void OLED::TurnOn(byte VCC) {
  IO_VCC = VCC;
  if (!isU8g2Begin) {
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    digitalWrite(CS1, LOW);
    digitalWrite(CS2, LOW);
    DoRST();
    pinMode(IO_VCC, OUTPUT);
    digitalWrite(IO_VCC, HIGH);
    delay(10);
    if (!display.begin()) {
      Serial.println(F("SSD1306 allocation failed"));
      return;
    }
    isU8g2Begin = true;
  }
  display.clearBuffer();
  display.setDisplayRotation(U8G2_R2);
  Show_System_Info();
  digitalWrite(CS1, LOW);
  digitalWrite(CS2, LOW);
  display.sendBuffer();
  digitalWrite(CS1, HIGH);
  digitalWrite(CS2, HIGH);
}

void OLED::TurnOff() {
  delay(200);
  display.clear();
  digitalWrite(CS1, LOW);
  digitalWrite(CS2, LOW);
  display.sendBuffer();
  // display.initDisplay();
  delay(1);
  display.setPowerSave(1);
  delay(1);
  digitalWrite(CS1, HIGH);
  digitalWrite(CS2, HIGH);
  if (IO_VCC != 0) {
    digitalWrite(IO_VCC, LOW);
    pinMode(IO_VCC, INPUT);
  }
}

void OLED::DoRST() {
  pinMode(IO_OLED_RST, OUTPUT);
  digitalWrite(IO_OLED_RST, LOW);
  delay(1);
  digitalWrite(IO_OLED_RST, HIGH);
  delay(1);
}



void OLED::Show_System_Info() {
  display.setFont(u8g2_font_9x18B_tr);
  display.drawStr(2, 10, "SystemInfo");
  display.drawLine(2, 12, 125, 12);
}

void OLED::Show_Str(uint16_t x,uint16_t y,const char *str){
  display.setFont(u8g2_font_9x18B_tr);
  display.drawStr(x, y, str);
  display.setCursor(x, y);
  display.print(0xAA,HEX);
}
void OLED::Update() {
  Show_System_Info();
  digitalWrite(CS1, LOW);
  display.sendBuffer();
  digitalWrite(CS1, HIGH);
}
void OLED::clearBuffer() {
  display.clearBuffer();
}

void OLED::ShowHex(uint16_t x,uint16_t y,const uint8_t *hex){
  display.setFont(u8g2_font_9x18B_tr);
  display.setCursor(x, y);
  display.print(*hex,HEX);
}