#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pin on which the Arduino is connected to the NeoPixels.
#define STRIP_PIN       6
#define GAME_LIGHT_PIN  7
#define STRIP_BRIGHTNESS 50
#define BRIGHTNESS 90

// Number of NeoPixels attached to the Arduino.
// The strip has 60 NeoPixels but theres some magic going on.
// Lighting the 44th light lights the last one on strip ¯\_(ツ)_/¯
#define NUMPIXELS      44

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, STRIP_PIN, NEO_GRBW + NEO_KHZ800);
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel gameLight = Adafruit_NeoPixel(NUMPIXELS, GAME_LIGHT_PIN, NEO_GRBW + NEO_KHZ800);

// Pin constants
const int player1pin = 2;       // Player 1 input pin
const int player2pin = 4;       // Player 2 input pin

// Global variables
bool gameLightLit = false;

void setup() {
  Serial.begin(9600);                   // Set up serial connection for outputting to monitor
  pinMode(player1pin, INPUT);           // Initialize the player 1 pin as an input:
  pinMode(player2pin, INPUT);           // Initialize the player 2 pin as an input:
  strip.begin();                        // Initialize the NeoPixel library.
  gameLight.begin();                    // Initialize the NeoPixel library.
  strip.setBrightness(STRIP_BRIGHTNESS);
  gameLight.setBrightness(BRIGHTNESS);
  strip.show();                         // Clear pixels on strip
  gameLight.show();                     // Clear pixels on strip
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait, int player) {
  if(player == 1) {
    for(uint16_t i = 0; i < NUMPIXELS / 2; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
  } else if(player == 2) {
    for(uint16_t i = NUMPIXELS; i > NUMPIXELS/2; i--) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);  
     } 
  }
}

void turnOnGameLight(){
  for(uint16_t i=0; i<gameLight.numPixels(); i++) {
        gameLight.setPixelColor(i, strip.Color(255,255,255,255) );
    }
  gameLight.show();
}


void turnOffGameLight(){
  for(uint16_t i=0; i<gameLight.numPixels(); i++) {
        gameLight.setPixelColor(i, strip.Color(0,0,0) );
    }
  gameLight.show();
}

void playRound() {
  delay(random(1000,8000));
  turnOnGameLight();
  gameLightLit = true;
  while(gameLightLit) {
    int player1 = digitalRead(player1pin);
    int player2 = digitalRead(player2pin);
    if(player1) {
      turnOffGameLight();
      gameLightLit = false;
      Serial.write("1");
      colorWipe(strip.Color(255, 140, 0, 255), 40, 1);
      colorWipe(strip.Color(0, 0, 0, 0), 0, 1);
    } 
    else if(player2) {
      turnOffGameLight();
      gameLightLit = false;
      Serial.write("2");
      colorWipe(strip.Color(255, 140, 0, 255), 40, 2);
      colorWipe(strip.Color(0, 0, 0, 0), 0, 2);
    }
  }
}

char incomingByte;   // for incoming serial data
char finish_condition;

void loop() {
  if (Serial.available() > 0) {              
    incomingByte = Serial.read();
    if (incomingByte == 's') {
      Serial.write('4');  

      while(1) {
        playRound();
        finish_condition = Serial.read();
        if (finish_condition == 'f') {
          break;
        }
      }
      
    }
  }
}


