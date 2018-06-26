#include <avr/power.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUM_LEDS 50
#define PIXELS_PER_ROW 5
#define NEO_KHZ400 true
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);

void setup()
{
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  RGBLoop();
}

void loop() { 
  Fire(90,90,15);
  SparkleLoop();
  RainbowCycle(10);
}

void RGBLoop(){
  for(int j = 0; j < 4; j++ ) { 
    // Fade IN
    for(int k = 0; k < 256; k++) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
        case 3: setAll(k,k,k); break;
      }
      showStrip();
      delay(1);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
        case 3: setAll(k,k,k); break;
      }
      showStrip();
      delay(1);
    }
  }
}

void SparkleLoop(){
  for(int i = 0; i < 2048; i++){
    Sparkle(random(255), random(255), random(255), 1);
  }
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void RainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< (NUM_LEDS / PIXELS_PER_ROW); ++i) {
      c=Wheel(((i * 256 / (NUM_LEDS / PIXELS_PER_ROW)) + j) & 255);
      setRow(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;

  for(int timer =0; timer < 255; ++timer){
    // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
      
      if(cooldown>heat[i]) {
        heat[i]=0;
      } else {
        heat[i]=heat[i]-cooldown;
      }
    }
    
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k = NUM_LEDS - 1; k >= 2*PIXELS_PER_ROW; k--) {
      int tmp = heat[k - 1 * PIXELS_PER_ROW] + heat[k - 1 * PIXELS_PER_ROW + 1] + heat[k - 2 * PIXELS_PER_ROW] + heat[k - 2 * PIXELS_PER_ROW];
      if(k - 1 * PIXELS_PER_ROW + 1 >= 0){
        tmp += heat[k - 1 * PIXELS_PER_ROW + 1];
      } else {
        tmp += heat[PIXELS_PER_ROW];
      }
      heat[k] = tmp / 5;
    }
      
    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if( random(255) < Sparking ) {
      int y = random(3*PIXELS_PER_ROW);
      heat[y] = heat[y] + random(160,255);
    }
  
    // Step 4.  Convert heat to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      setPixelHeatColor(NUM_LEDS - j - 1, heat[j] );
    }
  
    showStrip();
    delay(SpeedDelay);
  }
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void setRow(int row, byte red, byte green, byte blue) {
  for( int i = 0; i < PIXELS_PER_ROW; ++i){
    setPixel( i + PIXELS_PER_ROW * row, red, green, blue);
  }
}

void setPixel(int row, int column, byte red, byte green, byte blue) {
  if(row %2 ==0){
    setPixel(PIXELS_PER_ROW * row + column, red, green, blue);
  } else {
    setPixel(PIXELS_PER_ROW * (row + 1) - column, red, green, blue);
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

