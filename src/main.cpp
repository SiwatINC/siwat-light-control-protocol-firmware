#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 3
#define NUMPIXELS 250

#define FLOW_CONTROL false

UART Serial2(4, 5, 0, 0);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t rgb_to_uint32(int r, int g, int b);
void handle_serial(String data);
void execute(String data);

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  while(Serial1.available()) {
    String data = Serial1.readStringUntil(0xFF);
    handle_serial(data);
  }
  while(Serial2.available()) {
    Serial1.write(Serial2.read());
  }
}

void handle_serial(String data) {
  int rhl = int(data.charAt(0));
  if(rhl > 0) {
    rhl-=1;
    data.setCharAt(0,rhl);
    Serial2.print(data);
    Serial2.write(0xFF);
  }
  else if(rhl == 0) {
    execute(data);
    if(FLOW_CONTROL)
      Serial1.write(0xFF);
  }
  
}

void execute(String data) {
  switch (int(data.charAt(1)))
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
    pixels.setBrightness(int(data.charAt(2)));
    break;
  }
  case 0x04: {
    uint32_t color = rgb_to_uint32(int(data.charAt(3)),int(data.charAt(4)),int(data.charAt(5)));
    pixels.setPixelColor(int(data.charAt(2)),color);
    break;
  }
  case 0x05: {
    uint32_t color = rgb_to_uint32(int(data.charAt(2)),int(data.charAt(3)),int(data.charAt(4)));
    pixels.fill(color);
    break;
  }
  case 0x06: {
    uint32_t color = rgb_to_uint32(int(data.charAt(4)),int(data.charAt(5)),int(data.charAt(6)));
    pixels.fill(color,int(data.charAt(2)),int(data.charAt(3)));
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