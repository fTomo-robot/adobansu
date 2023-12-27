#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 7
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.setBrightness(50);
  strip.show(); 
}

void loop() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.show();
  }
}
