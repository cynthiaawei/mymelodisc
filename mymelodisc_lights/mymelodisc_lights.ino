#include <FastLED.h>

// INPUT PINS
#define LED_PIN    2
#define D0_PIN     3
#define D1_PIN     4
#define D2_PIN     5

// COLOR SET UP
#define NUM_LEDS    100
#define BRIGHTNESS  250
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    pinMode(LED_PIN, OUTPUT);
    pinMode(D0_PIN, INPUT);
    pinMode(D1_PIN, INPUT);
    pinMode(D2_PIN, INPUT);
    Serial.begin(9600); 
}

void FillAll(CRGB Color){
  uint8_t brightness = 255;
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = Color;
  }
  FastLED.show();
}

unsigned long lastBeat = 0;
bool beatState = false;
bool paused = false;
bool pausePressed = false;

void loop() {
  int D0_input = digitalRead(D0_PIN);
  int D1_input = digitalRead(D1_PIN);
  int D2_input = digitalRead(D2_PIN);
  bool PAUSE_input = false;

  bool APT_input = (D0_input == 1 && D1_input == 0 && D2_input == 0);
  bool BAAB_input = (D0_input == 0 && D1_input == 1 && D2_input == 0);
  bool BOAF_input = (D0_input == 0 && D1_input == 0 && D2_input == 1);
  bool GLB_input = (D0_input == 1 && D1_input == 1 && D2_input == 0);
  bool WIWYM_input = (D0_input == 1 && D1_input == 0 && D2_input == 1);
  PAUSE_input = (D0_input == 1 && D1_input == 1 && D2_input == 1);

  CRGB color1, color2;
  int BPM = 0;
  int beatTime = 0;

  if(APT_input){
    color1 = CRGB::Maroon;
    color2 = CRGB::Brown;
    BPM = 149;
  } else if (BAAB_input){
    color1 = CRGB::Cyan;
    color2 = CRGB::Brown;
    BPM = 128;
  } else if (BOAF_input){
    color1 = CRGB::Violet;
    color2 = CRGB::SkyBlue;
    BPM = 105;
  } else if (GLB_input){
    color1 = CRGB::OrangeRed;
    color2 = CRGB::Red;
    BPM = 117;
  } else if (WIWYM_input){
    color1 = CRGB::Gold;
    color2 = CRGB::DarkGreen;
    BPM = 73;
  } else if(PAUSE_input){
    FillAll(color1);
  }
  beatTime = 60000 / BPM;
  if (!paused){
    if (APT_input == 1 || BAAB_input == 1 || BOAF_input == 1 || GLB_input == 1 || WIWYM_input == 1) {
      unsigned long currentMillis = millis();
    if (currentMillis >= lastBeat + beatTime) {
        lastBeat += beatTime; 
        FillAll(beatState ? color1 : color2); // Alternate between color1 and color2
        beatState = !beatState;
    }
      
    } else {
      FillAll(CRGB::Black);
      lastBeat = millis();  // reset beat clock when idle
    }
  }
}
