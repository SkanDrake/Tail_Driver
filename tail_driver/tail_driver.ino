#define NUM_OF_ROWS 10
#define NUM_OF_COL 5
#include <avr/power.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NEO_KHZ400 true
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, PIN, NEO_RGB + NEO_KHZ800);

void displaySetup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
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


void setPixel(int row, int column, byte red, byte green, byte blue) {
  if (row % 2 == 0) {
    setPixel(NUM_OF_COL * row + column, red, green, blue);
  } else {
    setPixel(NUM_OF_COL * (row + 1) - column - 1, red, green, blue);
  }
}

void setRow(int row, byte red, byte green, byte blue) {
  for ( int i = 0; i < NUM_OF_COL; ++i) {
    setPixel( i + NUM_OF_COL * row, red, green, blue);
  }
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_OF_ROWS * NUM_OF_COL; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
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

class Animation {
  public:
    virtual int Runtime() const = 0;
    virtual int ClockSpeed() const = 0;
    virtual void Tick(const int timestamp) = 0;
    void Animate() {
      for (int i = 0; i < Runtime(); ++i) {
        Tick(i);
        showStrip();
        delay(ClockSpeed());
      }
    }
};

class ColorFade : public Animation {
  public:
    ColorFade(byte red, byte green, byte blue):
      red_(255 * ((double)red * red / ((double)max(max(red * red, green * green), blue * blue)))),
      green_(255 * ((double)green * green / ((double)max(max(red * red, green * green), blue * blue)))),
      blue_(255 * ((double)blue * blue / ((double)max(max(red * red, green * green), blue * blue)))) {}

    int Runtime() const override {
      return 512;
    }

    int ClockSpeed() const override {
      return 1;
    }

    void Tick(const int timestamp) override {
      double fade_factor = timestamp > Runtime() / 2 ?
                           (double)(Runtime() - timestamp) / (double)(Runtime() / 2) :
                           (double)timestamp / (double)(Runtime() / 2);
      setAll((int)(red_ * fade_factor), (int)(green_ * fade_factor), (int)(blue_ * fade_factor));
    }

  private:
    byte red_;
    byte green_;
    byte blue_;
};

class Marquee : public Animation {
  public:
    Marquee(byte red, byte green, byte blue, const String& text):
      red_(255 * ((double)red * red / ((double)max(max(red * red, green * green), blue * blue)))),
      green_(255 * ((double)green * green / ((double)max(max(red * red, green * green), blue * blue)))),
      blue_(255 * ((double)blue * blue / ((double)max(max(red * red, green * green), blue * blue)))),
      text_("  " + text) {}

    int Runtime() const override {
      return 256 * text_.length();
    }

    int ClockSpeed() const override {
      return 1;
    }

    void Tick(const int timestamp) override {
      int offset = (int)(((double)timestamp / (double)Runtime()) *  text_.length() * 6) % (text_.length() * 6);
      for (int i = 0; i < text_.length(); ++i) {
        bool char_array[6][5];
        const char c = text_.charAt(i);
        if ( c == ' ') {
          const bool new_char_array[6][5] = {
            {false, false, false, false, false},
            {false, false, false, false, false},
            {false, false, false, false, false},
            {false, false, false, false, false},
            {false, false, false, false, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        } else if ( c == 'A' || c == 'a') {
          const bool new_char_array[6][5] = {
            {false, true, true, true, false},
            {true, false, false, false, true},
            {true, true, true, true, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'B' || c == 'b') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, false},
            {true, false, false, false, true},
            {true, true, true, true, false},
            {true, false, false, false, true},
            {true, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'C' || c == 'c') {
          const bool new_char_array[6][5] = {
            {false, true, true, true, false},
            {true, false, false, false, false},
            {true, false, false, false, false},
            {true, false, false, false, false},
            {false, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'D' || c == 'd') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, false},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {true, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'E' || c == 'e') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, true},
            {true, false, false, false, false},
            {true, true, true, true, false},
            {true, false, false, false, false},
            {true, true, true, true, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'F' || c == 'f') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, true},
            {true, false, false, false, false},
            {true, true, true, true, false},
            {true, false, false, false, false},
            {true, false, false, false, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'G' || c == 'g') {
          const bool new_char_array[6][5] = {
            {false, true, true, true, true},
            {true, false, false, false, false},
            {true, false, true, true, true},
            {true, false, false, false, true},
            {false, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'H' || c == 'h') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {true, false, false, false, true},
            {true, true, true, true, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'I' || c == 'i') {
          const bool new_char_array[6][5] = {
            {false, true, true, true, false},
            {false, false, true, false, false},
            {false, false, true, false, false},
            {false, false, true, false, false},
            {false, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'J' || c == 'j') {
          const bool new_char_array[6][5] = {
            {false, false, true, true, true},
            {false, false, false, true, false},
            {false, false, false, true, false},
            {true, false, false, true, false},
            {false, true, true, false, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'K' || c == 'k') {
          const bool new_char_array[6][5] = {
            {true, false, false, true, false},
            {true, false, true, false, false},
            {true, true, false, false, false},
            {true, false, true, false, false},
            {true, false, false, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'L' || c == 'l') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, false},
            {true, false, false, false, false},
            {true, false, false, false, false},
            {true, false, false, false, false},
            {true, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'M' || c == 'm') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {true, true, false, true, true},
            {true, false, true, false, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'N' || c == 'n') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {true, true, false, false, true},
            {true, false, true, false, true},
            {true, false, false, true, true},
            {false, false, false, false, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'O' || c == 'o') {
          const bool new_char_array[6][5] = {
            {false, true, true, true, false},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {false, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'P' || c == 'p') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, false},
            {true, false, false, false, true},
            {true, true, true, true, false},
            {true, false, false, false, false},
            {true, false, false, false, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'Q' || c == 'q') {
          const bool new_char_array[6][5] = {
            {false, true, true, false, false},
            {true, false, false, true, false},
            {true, false, false, true, false},
            {true, false, false, true, false},
            {false, true, true, true, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'R' || c == 'r') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, false},
            {true, false, false, false, true},
            {true, true, true, true, false},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'S' || c == 's') {
          const bool new_char_array[6][5] = {
            {false, true, true, true, true},
            {true, false, false, false, false},
            {false, true, true, true, false},
            {false, false, false, false, true},
            {true, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'T' || c == 't') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, true},
            {false, false, true, false, false},
            {false, false, true, false, false},
            {false, false, true, false, false},
            {false, false, true, false, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'U' || c == 'u') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {false, true, true, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'V' || c == 'v') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {true, false, false, false, true},
            {true, false, false, false, true},
            {false, true, false, true, false},
            {false, false, true, false, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'W' || c == 'w') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {true, false, true, false, true},
            {true, false, true, false, true},
            {true, false, true, false, true},
            {false, true, false, true, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'X' || c == 'x') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {false, true, false, true, false},
            {false, false, true, false, false},
            {false, true, false, true, false},
            {true, false, false, false, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'Y' || c == 'y') {
          const bool new_char_array[6][5] = {
            {true, false, false, false, true},
            {false, true, false, true, false},
            {false, false, true, false, false},
            {false, false, true, false, false},
            {false, false, true, false, false},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        else if ( c == 'Z' || c == 'z') {
          const bool new_char_array[6][5] = {
            {true, true, true, true, true},
            {false, false, false, true, false},
            {false, false, true, false, false},
            {false, true, false, false, false},
            {true, true, true, true, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        } else {
          const bool new_char_array[6][5] = {
            {true, true, true, true, true},
            {true, true, true, true, true},
            {true, true, true, true, true},
            {true, true, true, true, true},
            {true, true, true, true, true},
            {false, false, false, false, false}
          };
          memcpy(&char_array, &new_char_array, sizeof(char_array));
        }
        for (int j = 0; j < 6; ++j) {
          if (i * 6 - offset + j < NUM_OF_ROWS) {
            for (int k = 0; k < 5; ++k) {
              setPixel(i * 6 - offset + j, k,
                       char_array[j][k] ? (int)red_ : (int)0,
                       char_array[j][k] ? (int)green_ : (int)0,
                       char_array[j][k] ? (int)blue_ : (int)0);
            }
          }
        }
      }
    }

  private:
    const byte red_;
    const byte green_;
    const byte blue_;
    const String text_;
};

class RainbowCycle : public Animation {
  public:
    RainbowCycle() {}

    int Runtime() const override {
      return 256 * 5;
    }

    int ClockSpeed() const override {
      return 10;
    }

    void Tick(const int timestamp) override {
      for (int i = 0; i < NUM_OF_ROWS; ++i) {
        byte* c = Wheel(((i * 256 / NUM_OF_ROWS) + timestamp) & 255);
        setRow(i, *c, *(c + 1), *(c + 2));
      }
      showStrip();
    }

  private:
    byte * Wheel(byte WheelPos) {
      static byte c[3];

      if (WheelPos < 85) {
        c[0] = WheelPos * 3;
        c[1] = 255 - WheelPos * 3;
        c[2] = 0;
      } else if (WheelPos < 170) {
        WheelPos -= 85;
        c[0] = 255 - WheelPos * 3;
        c[1] = 0;
        c[2] = WheelPos * 3;
      } else {
        WheelPos -= 170;
        c[0] = 0;
        c[1] = WheelPos * 3;
        c[2] = 255 - WheelPos * 3;
      }

      return c;
    }
};

class Sparkle : public Animation {
  public:
    Sparkle(int fade_rate) : fade_rate_(fade_rate) {
      for (int i = 0; i < NUM_OF_ROWS * NUM_OF_COL; ++i) {
        for (int j = 0; j < 3; ++j) {
          current_display_[i][j] = 0;
        }
      }
    }

    int Runtime() const override {
      return 2048;
    }

    int ClockSpeed() const override {
      return 1;
    }

    void Tick(const int timestamp) override {
      for (int i = 0; i < NUM_OF_ROWS * NUM_OF_COL; ++i) {
        for (int j = 0; j < 3; ++j) {
          current_display_[i][j] = max(0, current_display_[i][j] - fade_rate_);
        }
      }
      byte red = random(255);
      byte blue = random(255);
      byte green = random(255);
      int pixel = random(NUM_OF_ROWS * NUM_OF_COL);
      current_display_[pixel][0] = red;
      current_display_[pixel][1] = green;
      current_display_[pixel][2] = blue;
      for (int i = 0; i < NUM_OF_ROWS * NUM_OF_COL; ++i) {
        setPixel(i, current_display_[i][0], current_display_[i][1], current_display_[i][2]);
      }
    }

  private:
    const int fade_rate_;
    byte current_display_[NUM_OF_ROWS * NUM_OF_COL][3];
};

class RunningLights : public Animation {
  public:
    RunningLights(byte red, byte green, byte blue):
      red_(255 * ((double)red * red / ((double)max(max(red * red, green * green), blue * blue)))),
      green_(255 * ((double)green * green / ((double)max(max(red * red, green * green), blue * blue)))),
      blue_(255 * ((double)blue * blue / ((double)max(max(red * red, green * green), blue * blue)))) {}

    int Runtime() const override {
      return 100;
    }

    int ClockSpeed() const override {
      return 50;
    }

    void Tick(const int timestamp) override {
      for (int i = 0; i < NUM_OF_ROWS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+timestamp) * 127 + 128;
        setRow(i, ((sin(i + timestamp) * 127 + 128) / 255)*red_,
               ((sin(i + timestamp) * 127 + 128) / 255)*green_,
               ((sin(i + timestamp) * 127 + 128) / 255)*blue_);
      }
    }

  private:
    byte red_;
    byte green_;
    byte blue_;
};

class BarbersPole : public Animation {
  public:
    BarbersPole(byte red, byte green, byte blue):
      red_(255 * ((double)red * red / ((double)max(max(red * red, green * green), blue * blue)))),
      green_(255 * ((double)green * green / ((double)max(max(red * red, green * green), blue * blue)))),
      blue_(255 * ((double)blue * blue / ((double)max(max(red * red, green * green), blue * blue)))) {}

    int Runtime() const override {
      return 100;
    }

    int ClockSpeed() const override {
      return 50;
    }

    void Tick(const int timestamp) override {
      for (int row = 0; row < NUM_OF_ROWS; ++row) {
        for (int column = 0; column < NUM_OF_COL; ++column) {
          if (column < NUM_OF_COL / 2) {
            setPixel(row, ((row + column + timestamp) % NUM_OF_COL), red_, green_, blue_);
          } else {
            setPixel(row, ((row + column + timestamp) % NUM_OF_COL), 0, 0, 0);
          }
        }
      }
    }

  private:
    byte red_;
    byte green_;
    byte blue_;
};

class Chevrons : public Animation {
  public:
    Chevrons(byte red, byte green, byte blue):
      red_(255 * ((double)red * red / ((double)max(max(red * red, green * green), blue * blue)))),
      green_(255 * ((double)green * green / ((double)max(max(red * red, green * green), blue * blue)))),
      blue_(255 * ((double)blue * blue / ((double)max(max(red * red, green * green), blue * blue)))) {}

    int Runtime() const override {
      return 100;
    }

    int ClockSpeed() const override {
      return 50;
    }

    void Tick(const int timestamp) override {
      for (int row = 0; row < NUM_OF_ROWS; ++row) {
        switch ((row + timestamp) % 5) {
          case 0:
            setPixel(row, 0, 0, 0, 0);
            setPixel(row, 1, 0, 0, 0);
            setPixel(row, 2, red_, green_, blue_);
            setPixel(row, 3, 0, 0, 0);
            setPixel(row, 4, 0, 0, 0);
            break;
          case 1:
            setPixel(row, 0, 0, 0, 0);
            setPixel(row, 1, red_, green_, blue_);
            setPixel(row, 2, red_, green_, blue_);
            setPixel(row, 3, red_, green_, blue_);
            setPixel(row, 4, 0, 0, 0);
            break;
          case 2:
            setPixel(row, 0, red_, green_, blue_);
            setPixel(row, 1, red_, green_, blue_);
            setPixel(row, 2, 0, 0, 0);
            setPixel(row, 3, red_, green_, blue_);
            setPixel(row, 4, red_, green_, blue_);
            break;
          case 3:
            setPixel(row, 0, red_, green_, blue_);
            setPixel(row, 1, 0, 0, 0);
            setPixel(row, 2, 0, 0, 0);
            setPixel(row, 3, 0, 0, 0);
            setPixel(row, 4, red_, green_, blue_);
            break;
          case 4:
            setPixel(row, 0, 0, 0, 0);
            setPixel(row, 1, 0, 0, 0);
            setPixel(row, 2, 0, 0, 0);
            setPixel(row, 3, 0, 0, 0);
            setPixel(row, 4, 0, 0, 0);
            break;
        }
      }
    }

  private:
    byte red_;
    byte green_;
    byte blue_;
};
void setup() {
  displaySetup();
  //ColorFade(255, 0, 0).Animate();
  //ColorFade(0, 255, 0).Animate();
  //ColorFade(0, 0, 255).Animate();
  Marquee(255, 128, 0, "Tails By Skan").Animate();
  randomSeed();
}

void loop() {
  switch (random(7)) {
    case 0:
      RunningLights(random(255), random(255), random(255)).Animate();
      break;
    case 1:
      RainbowCycle().Animate();
      break;
    case 2:
      Sparkle(10).Animate();
      break;
    case 3:
      ColorFade(random(255), random(255), random(255)).Animate();
      break;
    case 4:
      Marquee(random(255), random(255), random(255), "Luca").Animate();
      break;
    case 5:
      BarbersPole(random(255), random(255), random(255)).Animate();
      break;
    case 6:
      Chevrons(random(255), random(255), random(255)).Animate();
      break;
  }
}
