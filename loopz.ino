#include "FastLED.h"

FASTLED_USING_NAMESPACE

//thiscode is going to be for cycling through different animation patterns
#define DATA_PIN 4
#define CLOCK_PIN 5
#define LED_TYPE APA102
#define COLOR_ORDER GRB
#define NUM_LEDS 325
CRGB leds[NUM_LEDS]; //array of leds
#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

int delay_var = 8; 

void setup() {
  delay(3000); //3 second delay for recovery
  //initialize LED strip configuration 
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
}

typedef void (*PatternList[])();

PatternList gPatterns = { kissingsnakes,threesnakes, snake, rainbow, blinky, blinky2, rainbowWithGlitter, confetti, sinelon, juggle, bpm }; //all patterns
PatternList winPatterns = { threesnakes, rainbowWithGlitter, confetti}; //case 6
PatternList slowPatterns = {fading2, fading, fadingWithGlitter}; //slow default pattern
PatternList songPatterns = { kissingsnakes, snake, rainbow, blinky, blinky2, rainbowWithGlitter, confetti, sinelon, juggle, bpm }; //all patterns

int curPattern = 0;
int gHue = 0; 

void loop() {
    //update leds with current pattern
    int sound_var = 7;

    switch (sound_var) {
    //reset the delay to 120 frames per second 
    delay_var = 8;
    case 1:
      kissingsnakes();
      FastLED.show();
      FastLED.delay(delay_var);
      break;
    case 2:
      blinky2();
      FastLED.show();
      FastLED.delay(delay_var);
      break;
    case 9:
      threesnakes();
      FastLED.show();
      FastLED.delay(delay_var);
      break;
     case 3:
      juggle();
      FastLED.show();
      FastLED.delay(delay_var);
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow      
      break;

     case 4:
      bpmWithGlitter();
      FastLED.show();
      FastLED.delay(delay_var);
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow      
      break;
      
     case 5:
      sinelon();
      FastLED.show();
      FastLED.delay(delay_var);
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow      
      break;
      
     case 6:
     /**
      winPatterns[curPattern](); 
      EVERY_N_SECONDS( 7 ) { nextPattern2(); } //slowly cycle through the patterns
      FastLED.show();
      //FastLED.delay(delay_var);
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow 
      break;
      **/
      threesnakes();
      FastLED.show();
      EVERY_N_MILLISECONDS( 20 ) { gHue++; }
      break;
      
     case 7:
        songPatterns[curPattern](); 
        EVERY_N_SECONDS( 10 ) { nextPattern(); } //slowly cycle through the patterns
        FastLED.show();
        FastLED.delay(delay_var);
        EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow      
     break;
     
     default:
        slowPatterns[curPattern](); 
        EVERY_N_SECONDS( 10 ) { nextPattern(); } //slowly cycle through the patterns
        FastLED.show();
        FastLED.delay(delay_var);
        EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow      
    break;
  }
  

}

//************************// pattern switching //************************//

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
void nextPattern(){
  delay_var = 8;
  curPattern = curPattern + 1 % ARRAY_SIZE(slowPatterns);
  }
void nextPattern2(){
  delay_var = 8;
  curPattern = curPattern + 1 % ARRAY_SIZE(winPatterns);
  }

  void nextPattern3(){
  delay_var = 8;
  curPattern = curPattern + 1 % ARRAY_SIZE(songPatterns);
  }


//****////****////****////****////****// PATTERNS!! //****////****////****////****////****////

//************************// rainbow //************************//

void rainbow(){
  fill_rainbow(leds, NUM_LEDS, gHue, 7);  
  }

//************************// fading uniform //************************//

void fading2(){
  delay_var = 50;
  CRGBPalette16 palette = PartyColors_p;

  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = ColorFromPalette(palette, gHue, i);    
    }  
  }

//************************// fading by led pixel //************************//

void fading(){
  delay_var = 8;
  CRGBPalette16 palette = PartyColors_p;

  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = ColorFromPalette(palette, gHue+(i*2), i);    
    }  
  }

//************************// fading by led pixel with glitter //************************//

void fadingWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  fading();
  addGlitter(80);
}

//************************// fading by led pixel with glitter //************************//

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  FastLED.delay(8);
  addGlitter(80);
}

//************************// addGlitter //************************//

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

//************************// confetti //************************//

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

//************************// sinelon //************************//

int add = 0;
void sinelon()
{
  delay_var = 15;
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(5,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
  leds[(pos+100)%NUM_LEDS] += CHSV( gHue, 255, 192);
}

//************************// bpm //************************//

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

//************************// juggle //************************//

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+2,0,NUM_LEDS)] |= CHSV(dothue, 200, 255); //7
    dothue += 32;
  }
}

//************************// juggleWithGlitter //************************//

void juggleWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  juggle();
  addGlitter(80);
}

//************************// bpmWithGlitter //************************//

void bpmWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  bpm();
  addGlitter(80);
}


//************************// kissingsnakes //************************//
 //initializers
 int beg1 = NUM_LEDS/4;
 int end1 = NUM_LEDS/4;
 int end2 = 3*NUM_LEDS/4;
 int beg2 = 3*NUM_LEDS/4; 
 bool reverse = true;
   
 void kissingsnakes() {
  CRGBPalette16 palette = ForestColors_p;
  //set speed 
  delay_var = 20; 
  if (!reverse) {
  //first half of lights 
    leds[beg1] = CRGB(0,0,0);
    leds[end1] = CRGB(0,0,0);
    beg1+=1;
    end1-=1;

   //second half of lights
    leds[beg2] = CRGB(0,0,0);
    leds[end2] = CRGB(0,0,0);
    beg2+=1;
    end2-=1;
    
  }
  else {
  //first half of lights   
    leds[beg1] = ColorFromPalette(palette, (beg1*2));
    leds[end1] = ColorFromPalette(palette, (beg1*2));
    beg1-=1;
    end1+=1;
   //second half of lights 
    leds[beg2] = ColorFromPalette(palette, (beg2*2));
    leds[end2] = ColorFromPalette(palette, (beg2*2));
    beg2-=1;
    end2+=1;
    
    }
    if (beg1 > end1 ) reverse = true;
    if (beg1 == 0 ) reverse = false;
  
 }

//************************// SNAKE //************************//
 int color_idx = 0;
 int tail =0; 
 int speed = 1;
 
 void snake() {
  delay_var=speed;
  CRGBPalette16 palette = PartyColors_p;
  for (int i = tail; i<tail+20; i++){   
    leds[i]=ColorFromPalette(palette, gHue+i*2);
    color_idx +=1;
    }
    color_idx = 0; 

    leds[tail] = CRGB::Black;
    Serial.write(tail);
    
    //FastLED.show();
    //FastLED.delay(speed);
  
    if (++tail >= NUM_LEDS-20) {
       for (int z = tail; z<NUM_LEDS; z++){
          leds[z] = CRGB::Black;
          leds[z-NUM_LEDS+20]=ColorFromPalette(palette, gHue+z*2);
          FastLED.show();
          FastLED.delay(speed);
        }
             
        tail = 0;
        speed-=5;
        if (speed < 0){
          blinky();
          speed = 20; 
        }
      }
  }



//************************// BLINKY //************************//
  void blinky(){
    CRGBPalette16 palette = PartyColors_p;
  for (int k = 0 ; k<24; k++){
  for (int i=0; i<NUM_LEDS; i++){
    leds[i]= CRGB(255,i,i);
    leds[i]=ColorFromPalette(palette, i*2);
    }
  FastLED.show();                     // Refresh strip
  FastLED.delay(80);

  for (int i=0; i<NUM_LEDS; i++){
    leds[i]=CRGB(0,0,0);
    
    }
  FastLED.show();                     // Refresh strip
  FastLED.delay(50);
  }
  }


//************************// BLINKY2 //************************//
  void blinky2(){
  for (int k = 1 ; k<12; k++){
  for (int i=0; i<NUM_LEDS; i++){
    if ((i+k)%2==0){
    leds[i] = CRGB(255,i,i);
    
    }
    else{
      
      leds[i] = CRGB(0,0,0);
    }
    }
   FastLED.show();                     // Refresh strip
   FastLED.delay(80);

  }
  }


//************************// 3 SNAKE //************************//
//color_idx = 0;
int tail1 = 0; //starting positions of the snake 1
int tail2 = 100; //starting position of snake 2
int tail3 = 200; //starting position of snake 3
//speed = 1;
 
 void threesnakes() {
  delay_var=speed;
  CRGBPalette16 palette = PartyColors_p;
  for (int i = tail1; i<tail1+20; i++){   
    leds[i]=ColorFromPalette(palette, gHue+i*2);
    color_idx +=1;
    }
    color_idx = 0; 
    leds[tail1] = CRGB::Black;
    Serial.write(tail1);
  for (int i = tail2; i<tail2+20; i++){   
    leds[i]=ColorFromPalette(palette, gHue+i*2);
    color_idx +=1;
    }
    color_idx = 0; 
    leds[tail2] = CRGB::Black;
    Serial.write(tail2);

      for (int i = tail3; i<tail3+20; i++){   
    leds[i]=ColorFromPalette(palette, gHue+i*2);
    color_idx +=1;
    }
    color_idx = 0; 
    leds[tail3] = CRGB::Black;
    Serial.write(tail3);
    

  if (++tail3 >= NUM_LEDS-20) {
       for (int z = tail3; z<NUM_LEDS; z++){
          leds[z] = CRGB::Black;
          leds[z-NUM_LEDS+20]=ColorFromPalette(palette, gHue+z*2);
          FastLED.show();
          FastLED.delay(speed);
        }
             
        tail3 = 0;}
        
  if (++tail2 >= NUM_LEDS-20) {
       for (int z = tail2; z<NUM_LEDS; z++){
          leds[z] = CRGB::Black;
          leds[z-NUM_LEDS+20]=ColorFromPalette(palette, gHue+z*2);
          FastLED.show();
          FastLED.delay(speed);
        }
             
        tail2 = 0;}


        
    if (++tail1 >= NUM_LEDS-20) {
       for (int z = tail1; z<NUM_LEDS; z++){
          leds[z] = CRGB::Black;
          leds[z-NUM_LEDS+20]=ColorFromPalette(palette, gHue+z*2);
          FastLED.show();
          FastLED.delay(speed);
        }
             
        tail1 = 0;
        speed-=5;
        if (speed < 0){
          blinky();
          speed = 10; 
        }
      }
  }




