/*
  Welcome to the TeaTimer V1 - This is a simple yet elegant code base for creating a tea timer (or any kind of timer) 
                                that doesn't use the dreaded process blocking "delay()" function so that the user is free
                                to push that damn button anytime they please!

                                The timing and speed of everything is set up to work with the Adafruit Trinket Pro 5V
                                If you use a faster Arduino board, you may need to modify the variables
                                "ledFadeInterval" and "LEDFadeIndexStepIncrementAmount" in the MyStateMachine.cpp class

                                Ok enough chit chat, enjoy drinking your perfectly brewed tea!
*/


#include "MyStateMachine.cpp"

#define BUTTON_PIN 5
#define NUM_ROTARY_POSITIONS 6

// migrate this define and enum to the MyStateMachine class
//#define COMMON_ANODE

// Program State types supported:
//enum  states { NONE, COUNTING_DOWN, COUNTDOWN_COMPLETE};
 
int redPin = 9;
int greenPin = 10;
int bluePin = 12;

int buzzerPin = 11;

const int buttonPin = 4;     // the number of the pushbutton pin


int systemState = 0;

int rotaryKnobMinuteSelection = 0;
int rotaryKnobPinCount = 1;

static const uint8_t analog_pins[] = {A0,A1,A2,A3,A4,A5};

// no longer need this class. It was fun while it lasted, but the StateMachine is better
//MyButtonClass TheButton(buttonPin);

MyStateMachine StateMachine(redPin, greenPin, bluePin, buzzerPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  //Serial.begin();
  delay(10);
  StateMachine.setColor(0, 0, 255);  // Blue light is initial state
}


// States
// 1) Idle
// 2) Counting down
// 3) Countdown Complete


void loop() {
  // put your main code here, to run repeatedly:
  // TheButton.Update(millis());
  StateMachine.Update(millis());


  if (digitalRead(buttonPin) == LOW) // Button pressed
  {

    //Serial.println("KnobCount: ");
    
      // this sets everything in motion and makes it so that if a person presses the button more than once,
      // it will work no matter what state the StateMachine is in
      StateMachine.StartCountdown(GetNumMinutesForTimer()+1); // add 1 because the index starts at 0
      
  }


}

// a relatively efficient way to read all the pins and find the one that is low (aka the one that the rotary switch is set to)
// we'll use this to set the number of minutes to countdown
unsigned long GetNumMinutesForTimer(){

  unsigned long rotaryKnobPinCount = 0;
  
  while((digitalRead(analog_pins[rotaryKnobPinCount]) != LOW) && (rotaryKnobPinCount < NUM_ROTARY_POSITIONS)){
  
      rotaryKnobPinCount++;
  }

  return rotaryKnobPinCount;

}














