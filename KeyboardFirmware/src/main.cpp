#include <Arduino.h>

//========================================================================
// Hardwiring the ATtiny2313
//========================================================================
/*
                             ____  ____
               /RESET (PA2) -|1  \/   |- (Vcc)
                 LOCK (PD0) -|2     19|- (PB7) nc
                PIN24 (PD1) -|3     18|- (PB6) nc
                   nc (PA0) -|4     17|- (PB5) CAPSLOCTOGGLE
                   nc (PA2) -|5     16|- (PB4) S4080TOGGLE
                PIN25 (PD2) -|6     15|- (PB3) SHIFTLOCK
              CAPSLED (PD3) -|7     14|- (PB2) nc
             S4080LED (PD4) -|8     13|- (PB1) nc
             SHIFTLED (PD5) -|9     12|- (PB0) nc
                      (GND) -|________|- (PD6) nc

*/

#define CAPSLOCKPIN 14
#define S4080PIN 13
#define SHIFTLOCKPIN 12

#define CAPSLOCKLED 5
#define S4080LED 6
#define SHIFTLOCKLED 7

#define LOCKOUT 0
#define PIN24 1
#define PIN25 4


static int capslock = 0;
static int s4080 = 0;
static int shiftlock = 0;

static int shiftState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
static unsigned long shiftlastDebounceTime = 0;  // the last time the output pin was toggled

static int capsState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
static unsigned long capslastDebounceTime = 0;  // the last time the output pin was toggled

static int s4080State = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
static unsigned long  s4080lastDebounceTime = 0;  // the last time the output pin was toggled

static unsigned long  debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup()
{
  pinMode(CAPSLOCKLED, OUTPUT);
  digitalWrite(CAPSLOCKLED, HIGH);
  pinMode(S4080LED, OUTPUT);
  digitalWrite(S4080LED, HIGH);
  pinMode(SHIFTLOCKLED, OUTPUT);
  digitalWrite(SHIFTLOCKLED, HIGH);

  pinMode(CAPSLOCKPIN, INPUT);
  pinMode(S4080PIN, INPUT);
  pinMode(SHIFTLOCKPIN, INPUT);

  pinMode(LOCKOUT, OUTPUT);
  pinMode(PIN24, OUTPUT);
  pinMode(PIN25, OUTPUT);
  digitalWrite(LOCKOUT, HIGH);
  digitalWrite(PIN24, HIGH);
  digitalWrite(PIN25, HIGH);
}

void loop() {
  capsState = digitalRead(CAPSLOCKPIN);
  shiftState = digitalRead(SHIFTLOCKPIN);
  s4080State = digitalRead(S4080PIN);

// CAPS LOCK PROCESSING
  if ( (millis() - capslastDebounceTime) > debounceDelay) {
    if ( (capsState == LOW) && (capslock == 0) ) {
      digitalWrite(CAPSLOCKLED, LOW); //turn LED on
      digitalWrite(PIN25, LOW);
      capslock=1;
      capslastDebounceTime = millis(); //set the current time
    }
    else if ( (capsState == LOW) && (capslock != 0) ) {
      digitalWrite(CAPSLOCKLED, HIGH); //turn LED off
      digitalWrite(PIN25, HIGH);
      capslock=0;
      capslastDebounceTime = millis(); //set the current time
    }
  }

// S4080 LOCK PROCESSING
  if ( (millis() - s4080lastDebounceTime) > debounceDelay) {
    if ( (s4080State == LOW) && (s4080 == 0) ) {
      digitalWrite(S4080LED, LOW); //turn LED on
      digitalWrite(PIN24, LOW);
      s4080=1;
      s4080lastDebounceTime = millis(); //set the current time
    }
    else if ( (s4080State == LOW) && (s4080 != 0) ) {
      digitalWrite(S4080LED, HIGH); //turn LED off
      digitalWrite(PIN24, HIGH);
      s4080=0;
      s4080lastDebounceTime = millis(); //set the current time
    }
  }

// SHIFT LOCK PROCESSING
  if ( (millis() - shiftlastDebounceTime) > debounceDelay) {
    if ( (shiftState == LOW) && (shiftlock == 0) ) {
      digitalWrite(SHIFTLOCKLED, LOW); //turn LED on
      digitalWrite(LOCKOUT, LOW);
      shiftlock=1;
      shiftlastDebounceTime = millis(); //set the current time
    }
    else if ( (shiftState == LOW) && (shiftlock != 0) ) {
      digitalWrite(SHIFTLOCKLED, HIGH); //turn LED off
      digitalWrite(LOCKOUT, HIGH);
      shiftlock=0;
      shiftlastDebounceTime = millis(); //set the current time
    }
  }



}
