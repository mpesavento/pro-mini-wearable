//************************************************
// uses the pro-mini wearable device
// arduino pro mini on a custom shield from Chromatech
// Use "Pro Trinket 5V/16 MHz USB" board
// Programmer: USBtinyISP
//
// Make sure have Additional Board Manager URI installed in Preferences:
//    https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
//
// In Boards Manager, select Contributed, and install Adafruit AVR boards by Adafruit
//
//************************************************
#include <FastLED.h>
#include "btn.h"

#define DATA_PIN   9
#define CLOCK_PIN  8
#define CHIPSET    APA102
#define COLOR_ORDER BGR  // most of the 10mm black APA102

#define FRAMES_PER_SECOND  120

#define SWITCH_PIN 10
#define LONG_PRESS_TIME 1000

#define BRIGHTNESS_PIN  A0

#define NUM_LEDS  150

CRGB leds[NUM_LEDS];
Btn btn(SWITCH_PIN);

uint8_t num_leds = 64;//NUM_LEDS;
bool boa_mode = true;

void setup() {
  // Leds
  FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);

  // Switch
  pinMode(SWITCH_PIN, INPUT_PULLUP); 
}

void rainbow(bool sparkle) {
  static int offset = 0;
  fill_rainbow(leds, num_leds, offset, 7);
  if (sparkle && random8() < 100) {
    leds[ random16(num_leds) ] += CRGB::White;
  }
  offset++;
}

void loop() {

  if (boa_mode) {
      num_leds = 32;
  }

  uint16_t brightness = analogRead(BRIGHTNESS_PIN);
  FastLED.setBrightness(map(brightness, 0, 1023, 0, 255));

  btn.poll(NULL, NULL);

  rainbow(btn.pressed());

  if (boa_mode) {
      for (int i=0; i<32; i++) {
          leds[63-i] = leds[i];
      }
  }
  
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


