#ifndef OLED_H
#define OLED_H
#include <U8g2lib.h>
#ifndef CS1
#define CS1 10
#endif
#ifndef CS2
#define CS2 42
#endif
#ifndef IO_OLED_RST
#define IO_OLED_RST 41
#endif

class OLED
{
private:
    uint8_t IO_VCC        = 0;
    bool    isU8g2Begin   = false;
    // Special Operation
public:
    void TurnOn(byte VCC);
    void InitDisplay();
    void TurnOff();
    void DoRST();
    void Update();
    void Show_System_Info();
    void Show_Str(uint16_t x,uint16_t y,const char *str);
    void ShowHex(uint16_t x,uint16_t y,const uint8_t *hex);
    void clearBuffer(); 
};

#endif