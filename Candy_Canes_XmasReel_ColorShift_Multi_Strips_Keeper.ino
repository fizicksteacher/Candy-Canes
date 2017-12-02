#include "FastLED.h"

FASTLED_USING_NAMESPACE

 
//

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN1    5
#define DATA_PIN2    6
#define DATA_PIN3    7
#define DATA_PIN4    8


#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    300
#define WIDTH 1
#define WaveDELAY 20
#define DIM 50
#define BRIGHT 100
#define PulseW 10
#define Switch 20
#define FRAMES_PER_SECOND 60
int colorIndex = 1;

bool gReverseDirection = false;
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];

#define BRIGHTNESS         150
#define FRAMES_PER_SECOND  30
CRGBPalette16 gPal;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


//-------------------------------------------------------------------
void setup() {
  delay(15000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN1,COLOR_ORDER>(leds1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN2,COLOR_ORDER>(leds2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN3,COLOR_ORDER>(leds3, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN4,COLOR_ORDER>(leds4, NUM_LEDS).setCorrection(TypicalLEDStrip);




  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);
  //Palette set up
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
//FIRE SECTION----------------------------------------------------
  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.
  //gPal = HeatColors_p;
  
  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
     gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  
  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  
  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

}

//----------------------------------------------------------------------------------
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { CandyChase, WaveFill, WaveFillRev, WaveFillSepDir, WaveFill, WaveFillRev, CHASE,  rainbowWithGlitter, bpm, confetti, confetti, sinelon, juggle,  };
//SimplePatternList gPatterns = { WaveFill,};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  //FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 50 ) { gHue=gHue+3; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( Switch ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds1, NUM_LEDS, gHue, 3);
  fill_rainbow( leds2, NUM_LEDS, gHue, 3);
  fill_rainbow( leds3, NUM_LEDS, gHue, 3);
  fill_rainbow( leds4, NUM_LEDS, gHue, 3);

}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(255);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds1[ random16(NUM_LEDS) ] += CRGB::White;
    leds2[ random16(NUM_LEDS) ] += CRGB::White;
    leds3[ random16(NUM_LEDS) ] += CRGB::White;
    leds4[ random16(NUM_LEDS) ] += CRGB::White;

  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds1, NUM_LEDS, 1);
  fadeToBlackBy( leds2, NUM_LEDS, 1);
  fadeToBlackBy( leds3, NUM_LEDS, 1);
  fadeToBlackBy( leds4, NUM_LEDS, 1);

  int pos = random16(NUM_LEDS);
  leds1[pos] += CHSV( gHue + random8(64), 255, 255);
  leds2[pos] += CHSV( gHue + random8(10), 255, 255);
  leds3[pos] += CHSV( gHue + random8(64), 255, 255);
  leds4[pos] += CHSV( gHue + random8(10), 255, 255);

}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds1, NUM_LEDS, 5);
  fadeToBlackBy( leds2, NUM_LEDS, 5);
  fadeToBlackBy( leds3, NUM_LEDS, 5);
  fadeToBlackBy( leds4, NUM_LEDS, 5);

  int pos = beatsin16(13,0,NUM_LEDS);
  leds1[pos] += CHSV( gHue, 255, 192);
  leds2[pos] += CHSV( gHue, 255, 192);
  leds3[pos] += CHSV( gHue, 255, 192);
  leds4[pos] += CHSV( gHue, 255, 192);

}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 32;
  CRGBPalette16 palette = RainbowColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds1[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds2[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds3[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds4[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));

  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds1, NUM_LEDS, 20);
  fadeToBlackBy( leds2, NUM_LEDS, 20);
  fadeToBlackBy( leds3, NUM_LEDS, 20);
  fadeToBlackBy( leds4, NUM_LEDS, 20);

  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds1[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    leds2[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    leds3[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    leds4[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);

    dothue += 32;
  }
}

void WaveFill() 
{
    ChangePalettePeriodically();
   uint8_t brightness = 255;
  for(int c = 0; c < PulseW; c++) { 
    //int colorIndex2 = colorIndex+64; 
    for(int i = 0; i < NUM_LEDS/PulseW; i++) {
    int Z = i*PulseW;
    int b = 0;
      for(int j = 0; j<PulseW; j++) {
      // set our current dot to red, white
        b = b+(BRIGHT/PulseW);
        if ( Z+j+c <= NUM_LEDS-1 ) {
          int POS = Z+j+c;
          leds1[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          int colorIndex2 = colorIndex+16;
          leds2[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          leds3[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          leds4[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          //leds5[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          //leds6[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          //leds7[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          //leds8[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
        }
      }
      
    }
colorIndex += 1;
addGlitter(255);
FastLED.show();
delay(WaveDELAY);
  }
    }


void WaveFillRev()
{
  ChangePalettePeriodically();
  for(int c = 0; c < PulseW; c++) {  
    for(int i = 0; i < NUM_LEDS/PulseW; i++) {
    int Z = i*PulseW;
    int b = 0;
      for(int j = 0; j<PulseW; j++) {
      // set our current dot to red, white
        b = b+(BRIGHT/PulseW);
        if ( Z+j+c <= NUM_LEDS-1 ) {
          int POS = NUM_LEDS-1-(Z+j+c);
          leds1[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          int colorIndex2 = colorIndex+16;
          leds2[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          leds3[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          leds4[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          //leds5[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          //leds6[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          //leds7[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          //leds8[POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
        //Serial.println (POS);
        }
      }
    }
colorIndex += 1;
addGlitter(255);
FastLED.show();
delay(WaveDELAY);
  }
}

void WaveFillSepDir()
{
  ChangePalettePeriodically();
  for(int c = 0; c < PulseW; c++) {  
    for(int i = 0; i < NUM_LEDS/PulseW; i++) {
    int Z = i*PulseW;
    int b = 0;
      for(int j = 0; j<PulseW; j++) {
      // set our current dot to red, white
        b = b+(BRIGHT/PulseW);
        if ( Z+j+c <= NUM_LEDS-1 ) {
          int POS = Z+j+c;
          leds1[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          int colorIndex2 = colorIndex+150;
          leds2[NUM_LEDS-1-POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          leds3[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          leds4[NUM_LEDS-1-POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          //leds5[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          //leds6[NUM_LEDS-1-POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
          //leds7[POS] = ColorFromPalette( currentPalette, colorIndex, b, currentBlending);
          //leds8[NUM_LEDS-1-POS] = ColorFromPalette( currentPalette, colorIndex2, b, currentBlending);
        }
      }
    }
colorIndex += 1;
addGlitter(255);
FastLED.show();
//delay(WaveDELAY);
  }
}
void CHASE()
{
  ChangePalettePeriodically();
  colorIndex = colorIndex + 16;
  for(unsigned int M = 0; M < NUM_LEDS-WIDTH-1; M++) {
   for(unsigned int N = 0; N<WIDTH; N++) {
    // set our current dot to red, white
          leds1[M+N] = ColorFromPalette( currentPalette, colorIndex, BRIGHT, currentBlending);
          leds2[M+N] = CHSV(0,0,BRIGHT);
          leds3[M+N] = ColorFromPalette( currentPalette, colorIndex, BRIGHT, currentBlending);
          leds4[M+N] = CHSV(0,0,BRIGHT);

   }
    FastLED.show();

    // clear our current dot before we move on
    for(int N = 0; N<WIDTH; N++) {
          leds1[M+N] = ColorFromPalette( currentPalette, colorIndex, DIM+25, currentBlending);
          leds2[M+N] = CHSV(0,0,DIM+25);
          leds3[M+N] = ColorFromPalette( currentPalette, colorIndex, DIM+25, currentBlending);
          leds4[M+N] = CHSV(0,0,DIM)+25;

    }
 

  }
    ChangePalettePeriodically();
    colorIndex = colorIndex + 50;
    for(unsigned int M = NUM_LEDS-1; M > 0; M--) {
    // set our current dot to red, white
    for(unsigned int N = WIDTH; N>0; N--) {
          leds1[M-N] = ColorFromPalette( currentPalette, colorIndex, BRIGHT, currentBlending);
          leds2[M+N] = CHSV(0,0,BRIGHT);
          leds3[M-N] = ColorFromPalette( currentPalette, colorIndex, BRIGHT, currentBlending);
          leds4[M+N] = CHSV(0,0,BRIGHT);

    }
    FastLED.show();
    
    // clear our current dot before we move on
    for(unsigned int N = WIDTH; N>0; N--) {   
          leds1[M-N] = ColorFromPalette( currentPalette, colorIndex, DIM, currentBlending);
          leds2[M+N] = CHSV(0,0,DIM+25);
          leds3[M-N] = ColorFromPalette( currentPalette, colorIndex, DIM, currentBlending);
          leds4[M+N] = CHSV(0,0,DIM+25);

    }
  }
}

void CandyChase()
{
for(unsigned int i = 0; i < NUM_LEDS-1-WIDTH; i++) {
   for(unsigned int j = 0; j<WIDTH; j++) {
    // set our current dot to red, white
    leds1[i+j] = CRGB(BRIGHT, BRIGHT, BRIGHT); //Sets strip to bright white
    leds2[i+j] = CRGB(BRIGHT, 0, 0); //Sets strip to bright red
    leds3[i+j] = CRGB(BRIGHT, BRIGHT, BRIGHT); //Sets strip to bright white
    leds4[i+j] = CRGB(BRIGHT, 0, 0); //Sets strip to bright red

   }
    FastLED.show();

    // clear our current dot before we move on
    for(int j = 0; j<WIDTH; j++) {
    leds1[i+j] = CRGB(DIM-10, DIM, DIM-5); //Sets to dim white
    leds2[i+j] = CRGB(DIM, 0, 0); //Sets strip to dim red
    leds3[i+j] = CRGB(DIM-10, DIM, DIM-5); //Sets to dim white
    leds4[i+j] = CRGB(DIM, 0, 0); //Sets strip to dim red

    }
 
    //delay(DELAYMIX);
  }
  //delay(COLORDELAY);
  for(unsigned int i = NUM_LEDS-1; i > 0; i--) {
    // set our current dot to red, white
    for(unsigned int j = WIDTH; j>0; j--) {
    leds1[i-j] = CRGB(BRIGHT, 0, 0); //Sets strip to bright Red
    leds2[i-j] = CRGB(BRIGHT, BRIGHT, BRIGHT); //Sets strip to bright White
    leds3[i-j] = CRGB(BRIGHT, 0, 0); //Sets strip to bright Red
    leds4[i-j] = CRGB(BRIGHT, BRIGHT, BRIGHT); //Sets strip to bright White
    
    }
    FastLED.show();
    
    // clear our current dot before we move on
    for(unsigned int j = WIDTH; j>0; j--) {   
    leds1[i-j] = CRGB(DIM, 0, 0);  //Sets to dim Red
    leds2[i-j] = CRGB(DIM-10, DIM, DIM-5); //Sets strip to dim White
    leds3[i-j] = CRGB(DIM, 0, 0);  //Sets to dim Red
    leds4[i-j] = CRGB(DIM-10, DIM, DIM-5); //Sets strip to dim White

    }
     //delay(DELAYMIX);
  }
}

void Fire()
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  //   static uint8_t hue = 0;
  //   hue++;
  //   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

  Fire2012WithPalette(); // run simulation frame, using palette colors
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

// Fire2012 by Mark Kriegsman, July 2012
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  100

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 50

void Fire2012WithPalette()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    for( int j = 0; j < NUM_LEDS; j++) {
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds1[pixelnumber] = color;
      leds2[pixelnumber] = color;
      leds3[pixelnumber] = color;
      leds4[pixelnumber] = color;
  
    }
}

//--------------------------------------------------------------
//   Color Palette set-up

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        //if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = NOBLEND; }
        if( secondHand == 0)  { SetupRedAndGreenPalette();             currentBlending = NOBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { SetupBlueAndWhitePalette();         currentBlending = NOBLEND; }
        //if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = NOBLEND; }
        //if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
 }

void SetupRedAndGreenPalette()
{
    CRGB red = CHSV( HUE_RED
    , 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB white  = CRGB::White;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  red, white,
                                   green, green, red, white,
                                   green, green, red, white,
                                   green, green, red, white);
}

void SetupBlueAndWhitePalette()
{
    CRGB blue = CHSV( HUE_BLUE
    , 255, 255);
    CRGB purple  = CHSV( HUE_PURPLE, 255, 255);
    CRGB white  = CRGB::White;
    
    currentPalette = CRGBPalette16(
                                   blue,  blue,  purple, white,
                                   blue,  blue,  purple, white,
                                   blue,  blue,  purple, white,
                                   blue,  blue,  purple, white);
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Gray,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Gray,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Gray,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Gray
};


