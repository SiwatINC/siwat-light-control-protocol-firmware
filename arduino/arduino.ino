#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 3
#define NUMPIXELS 240

const int BUFFER_SIZE = 1024;
char read_buffer[BUFFER_SIZE];

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void execute(char data[], int length);

void handle_serial(char data[], int length);

uint32_t rgb_to_uint32(int r, int g, int b);

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial1.begin(115200);
  Serial2.begin(115200);
  pixels.begin();
}

void loop() {
  if(Serial1.available()) {
    int databyte = 0;
    while(databyte != 255) {
      int data_length = Serial1.readBytesUntil(0xFF,read_buffer,BUFFER_SIZE);
      handle_serial(read_buffer,data_length);
    }
  }
  Serial1.println("waiting...");
}

void handle_serial(char data[], int length) {
  if(data[0] > 0x00) {
    data[0]-=1;
    Serial2.print(data);
  }
  else if(data[0] == 0x00) {
    execute(data,length);
  }
}

void execute(char data[], int length) {
  Serial1.println(data[1]);
  switch (data[1])
  {
  case 0x00: {
    digitalWrite(LED_BUILTIN,1);
    sleep_ms(300);
    digitalWrite(LED_BUILTIN,0);
    break;
  }
  case 0x01: {
    pixels.clear();
    break;
  }
  case 0x02: {
    pixels.show();
    break;
  }
  case 0x03: {
    pixels.setBrightness(data[2]);
    break;
  }
  case 0x04: {
    uint32_t color = rgb_to_uint32(data[3],data[4],data[5]);
    pixels.setPixelColor(data[2],color);
    break;
  }
  case 0x05: {
    uint32_t color = rgb_to_uint32(data[2],data[3],data[4]);
    pixels.fill(color);
    break;
  }
  case 0x06: {
    uint32_t color = rgb_to_uint32(data[4],data[5],data[6]);
    pixels.fill(color,data[2],data[3]);
    break;
  }
  case 0x07: {
    pixels.rainbow();
    break;
  }
  default:
    break;
  }
}

uint32_t rgb_to_uint32(int r, int g, int b) {
  return pixels.Color(int(r/250.0*255.0),int(g/250.0*255.0),int(b/250.0*255.0));
}