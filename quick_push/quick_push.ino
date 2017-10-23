#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pin on which the Arduino is connected to the NeoPixels.
#define PIN            6

// Number of NeoPixels attached to the Arduino.
// The strip has 60 NeoPixels but theres some magic going on.
// Lighting the 44th light lights the last one on strip ¯\_(ツ)_/¯
#define NUMPIXELS      44

// Delay time for pixel animation
#define DELAYVAL       100

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

// Pin constants
const int player1pin = 2;       // Player 1 input pin
const int player2pin = 4;       // Player 2 input pin
const int gameLightPin =  12;   // Game light input pin

void setup() {
  Serial.begin(9600);               // Set up serial connection for outputting to monitor
  pinMode(gameLightPin, OUTPUT);    // Initialize the game light LED pin as an output:
  pinMode(player1pin, INPUT);       // Initialize the player 1 pin as an input:
  pinMode(player2pin, INPUT);       // Initialize the player 2 pin as an input:
  strip.begin();                   // Initialize the NeoPixel library.
  strip.show();                    // Clear pixels on strip
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

void playRound() {
  delay(5000);
  digitalWrite(gameLightPin, HIGH);
  while(digitalRead(gameLightPin) == HIGH) {
    int player1 = digitalRead(player1pin);
    int player2 = digitalRead(player2pin);
    if(player1) {
      digitalWrite(gameLightPin, LOW);
      colorWipe(strip.Color(0, 0, 0, 255), 40, 1);
      colorWipe(strip.Color(0, 0, 0, 0), 0, 1);
    } 
    else if(player2) {
      digitalWrite(gameLightPin, LOW);
      colorWipe(strip.Color(0, 0, 0, 255), 40, 2);
      colorWipe(strip.Color(0, 0, 0, 0), 0, 2);
    }
  }
}

void loop() {
  playRound();
}


