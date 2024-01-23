#include <Arduino.h>
#include "tusb.h"
#include "OnOff.h"
#include "OLED.h"
#include "SLED.h"
#include "Battery.h"
#include "Button.h"

OLED oled;
OnOff Swich;
Battery Bat;
SLED led;
Button But;

#define CDC_RX_BUFSIZE 64
#define CDC_TX_BUFSIZE 64
#define analogInputToDigitalPin(p)  (((p)<20)?(analogChannelToDigitalPin(p)):-1)
#define digitalPinToInterrupt(p)    (((p)<48)?(p):-1)
#define digitalPinHasPWM(p)         (p < 46)

TaskHandle_t *send_handle;

const byte FRAME_HEADER = 0xAA;  // 帧头
const byte FRAME_FOOTER = 0xFF;  // 帧尾
const int MAX_FRAME_SIZE = 64;   // 最大帧大小

char frame[MAX_FRAME_SIZE];      // 存储接收到的帧数据
int frameIndex = 0;              // 当前接收到的帧的索引
bool isReceiving = false;        // 是否正在接收帧数据  
void cdc_task(void)
{
  // 尝试读取CDC接收缓冲区中的数据
  oled.Show_Str( 2,27,String(frame).c_str());
  if (Serial.available())
  {
    // byte data = Serial.read();
    int data = Serial.parseInt();
    frame[frameIndex] = data;
    frameIndex++;
    Serial.printf("[rx] %5d",data);
    // if (data == FRAME_HEADER) {
    //   // 接收到帧头，开始接收帧数据
    //   frameIndex = 0;
    //   isReceiving = true;
    //   oled.Show_Str(2,40,"start");
    // } else if (data == FRAME_FOOTER && isReceiving) {
    //   // 接收到帧尾，完成帧数据接收
    //   isReceiving = false;
    //   frame[frameIndex] = '\n';
    //   oled.Show_Str(2,40,"end");
    //   // 处理接收到的完整帧数据
    //   // processFrame();
    // } else if (isReceiving) {
    //   // 接收到帧数据，存储到缓冲区
    //   oled.Show_Str(2,40,"receiving");
    //   frame[frameIndex] = data;
    //   frameIndex++;
      
    //   // 检查帧大小是否超过最大限制
    //   if (frameIndex >= MAX_FRAME_SIZE) {
    //     // 清除缓冲区，重新开始接收帧数据
    //     frameIndex = 0;
    //     isReceiving = false;
    //   }
    // }
    // TODO 导致重启死机
    // Serial.printf("[rx]%s",cdc_rx_buf[0]);
  }
  else{oled.Show_Str(2,40,"no data");}
}


const byte IO_11V = 37;
const byte IO_Button0 = 6;
const byte IO_EN = 18;
const byte IO_Button2 = 0;
const byte IO_Battery = 1;
const byte IO_Button1 = 7;
const byte IO_Long_Button = 2;

static void SEND(void *pvParameter) {}
void ButtonPress0() {
  But.Press[0] = true;
  Swich.LastEdit = millis();
}

void setup() {
  // Power On
  Swich.On(IO_Button0, IO_EN, led, oled);
  Serial.setHwFlowCtrlMode(0,0);
  // OLED display initialize
  oled.TurnOn(IO_11V);
  // Battery Set Up
  Bat.SetPin(IO_Battery);
  Bat.Update();
  // Wait for button release
  while (digitalRead(IO_Button0)) {
  }
  led.Set(0, 0, 0, 4);
  led.Update();
  oled.Update();
}


void loop()
{
  while (1){
    oled.clearBuffer();
    // 处理CDC通信任务
    cdc_task();
    oled.Update();
  }
}

bool SerialRxString(HardwareSerial serial,String *rx_string){ 
  int count = serial.available();
  if (count) {
   *rx_string = serial.readString();
  }
  return count > 0;
}

bool SerialRxUInt8(HardwareSerial serial,uint8_t *rx_byte){
  int count = serial.available();
  if (count) {
    *rx_byte = serial.read();  // 读取接收到的字节数据
  }
  return count > 0;
}

bool SerialRxFloat(HardwareSerial serial,float *rx_float){
  int count = serial.available();
  if (count) {
    Serial.readBytes((char *)&rx_float, sizeof(rx_float));
  }
  return count > 0;
}

uint16_t AssembleUInt16(uint8_t highByte, uint8_t lowByte) {
  return (static_cast<uint16_t>(highByte) << 8) | lowByte;
}

int AssembleInt(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
  return (static_cast<int>(byte1) << 24) 
  | (static_cast<int>(byte2) << 16) 
  | (static_cast<int>(byte3) << 8) 
  | byte4;
}

