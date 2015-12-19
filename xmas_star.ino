#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

// adapted by MacSimski to a star with 5 leds per side and light movement from the center outward

// note: to automatically adapt brightness during day and night, attach LDR between gnd and analogPin 
// and pull up analogPin with 10k to Vcc. uncomment last line in loop() containing dimlevel()

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    50
CRGB up[NUM_LEDS];
#define groundpin 9 // gpio pin used as gnd for ldr
#define analogInPin A10  // Analog input pin that the potentiometer is attached to
#define minLight 35
#define maxLight 120

int BRIGHTNESS    =     minLight;
#define FRAMES_PER_SECOND  120

void setup() {
   pinMode(groundpin,OUTPUT);
  digitalWrite(groundpin, LOW); //provide ground for ldr
  delay(500); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(up, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //set light level
  dimLevel(); 
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(15200);
  Serial.println("ready");
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbowWithGlitter ,rainbowWithGlitter , confetti,rainbowWithGlitter};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 40 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 240 ) { nextPattern(); } // change patterns periodically
//  EVERY_N_SECONDS(10) {dimLevel();} //adjust lightlevel
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


void dimLevel()
{
  int lightlevel=analogRead(analogInPin);
  BRIGHTNESS = map(lightlevel,0,1024,maxLight,minLight);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println(BRIGHTNESS);
}
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_star( up, NUM_LEDS, gHue, 10);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(10);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    up[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( up, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  up[pos] += CHSV( gHue + random8(64), 200, 255);
}







void fill_star( struct CRGB * pFirstLED, int numToFill,
                  uint8_t initialhue,
                  uint8_t deltahue )
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 240;
    for( int i = 0; i < numToFill; i++) {
        pFirstLED[i] = hsv;
      if ( (i/5 % 2) == 0){
        hsv.hue += deltahue;
      }
      else {
        hsv.hue -= deltahue;
      }   
    }
}
