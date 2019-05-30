/*
  MyStateMachine.cpp - This is a simple state machine to gracefully handle setting the three states of the tea timer system, as well as 
                          the proper audio/visual feedback in the form of an RGB LED and Piezzo Speaker.
  
                          The 3 states are:
                          1) Idle/None -- this is the state it is in when first turned on. LED == blue
                          2) Counting down -- this is the state it is in when the button is pressed and it is counting down. LED == green
                          3) Countdown Complete -- this is the state it is in after the countdown is complete
                                                    immediately after countdown is complete, the buzzer will beep 3 times at 1,200hz
                                                    and the LED will start slowly pulsing RED

                        The state machine will respond to button press input no matter which state it is in
                        If the countdown is already in progress and the button is pressed, it will re-set the time interval
                        If the countdown has completed once already and the button is pressed, it will re-start the countdown
                        
                        The timing and speed of everything is set up to work with the Adafruit Trinket Pro 5V
                        If you use a faster Arduino board, you may need to modify the variables
                        "ledFadeInterval" and "LEDFadeIndexStepIncrementAmount" in the MyStateMachine.cpp class

                        Ok enough chit chat, enjoy drinking your perfectly brewed tea!
*/


#include "Arduino.h"

// if the LEDs are common annode we need to flip the RGB values
#define COMMON_ANODE

// Program State types supported:
// 1) Idle/None
// 2) Counting down
// 3) Countdown Complete
enum  states { NONE, COUNTING_DOWN, COUNTDOWN_COMPLETE};

// Patern directions supported. used to pleasantly pulse the LED:
enum  fadeDirection { FORWARD, REVERSE };

class MyStateMachine
{
  public:
  
  int redPin;
  int greenPin;
  int bluePin;

  int buzzerPin; // the piezo is connected to analog pin designated by this variable
  int frequency = 1200; // sets the frequency in Hz that the buzzer will sound at. 1200 is pretty much perfect
                        // in the BOM for this project, I specified using a 2khz active piezo which supposedly only requires 3-5v
                        // in order to get it to make a 2kHz tone. But I swear when I hooked everything up, 
                        // the piezo was able to be controlled by the "tone" function which worked as you'd expect it to
                        // and produced a nice tone at 1,200 Hz  ¯\_(ツ)_/¯
                        // if you have issues with the Piezo you're using. sorry
  
  int numberOfBeeps = 3; // the number of times we want the buzzer to beep after countdown is complete
  int buzzerCounter;

  states ActiveState;
  fadeDirection Direction;     // direction to run the pattern

  // this variable sets how many milliseconds to add for each position of the rotary switch (which looks like tick marks from the outside)
  // one minute is 60,000 millis duh! set this to 30,000 if you want each tick to be 30 second increments
  // or come up with some logic in the StartCountdown function to decide the number of seconds each tick represents if you wanna get fancy pants
  const unsigned long tickTimeMultiple = 60000;
  
  unsigned long MillisToCount;
  unsigned long CountdownStartTime;
  
  unsigned long updateInterval;   // milliseconds between updates
  unsigned long lastUpdate; // last update of position

  unsigned long buzzerInterval;   // milliseconds between updates
  unsigned long buzzerOnInterval;   // milliseconds between updates
  unsigned long buzzerOffInterval;   // milliseconds between updates
  unsigned long buzzerToneBeginTime;



  unsigned long ledFadeUpdate;   // milliseconds between updates
  unsigned long ledFadeInterval;
  int LEDFadeIndex;
  int LEDFadeIndexStepIncrementAmount;
  int TotalLEDFadeSteps;
  

  
  //unsigned long lastUpdate; // last update of position
  
  
  //void (*OnComplete)();  // Callback on completion of pattern

  
  // Constructor - 
  // initializes the member variables and state
  MyStateMachine(int theRedPin, int theGreenPin, int theBluePin, int theBuzzerPin)
  {
    redPin = theRedPin;
    greenPin = theGreenPin;
    bluePin = theBluePin;

    ActiveState = NONE;
    buzzerPin = theBuzzerPin;

    buzzerOnInterval = 100;  // the number of milliseconds each beep will be "on" for
    buzzerOffInterval = 200; // the number of milliseconds each beep will be "off" for (or pause) between beeping
    
    // in order to simplify the logic of how the buzzer turns on and off (and not use the dreaded blocking "delay" function), this additional variable is used to set the 
    // interval that the buzzer is "on" or "off" and is toggled by being set to either "buzzerOnInterval" or "buzzerOffInterval"
    // since the first time the buzzer will sound, it needs to be on, then that means we want it to be set to the "buzzerOnInterval" first
    buzzerInterval = buzzerOnInterval;
    buzzerCounter = 0; // the number of times the buzzer has turned on so far    
    buzzerToneBeginTime = 0; // used for the timer logic to know if it's been long enough since the last buzz

    Direction = FORWARD;
    ledFadeUpdate = 0;   // milliseconds between updates
    ledFadeInterval = 10; // effectively sets the speed at which the LED pulses. 10ms seems to be about right for the Trinket Pro's slow ass processor
    LEDFadeIndex = 0; // see the LEDUpdate function to understand this
    LEDFadeIndexStepIncrementAmount = 5; // a silly way to say "the amount of brightness to add to the LED everytime it updates"
    TotalLEDFadeSteps = 255; // essentially sets the max brightness that the LED can achieve
  }

  void Update(unsigned long currentTime)
  {
      if((millis() - lastUpdate) > updateInterval) // time to update
      {
          lastUpdate = millis();

          //Meat and potatoes
          // our state machine! Isn't it beautiful?
          switch(ActiveState)
          {
              case COUNTING_DOWN:
                  if((millis() - CountdownStartTime) > MillisToCount){// time to update
                    ActiveState = COUNTDOWN_COMPLETE;
                    buzzerToneBeginTime = millis();
                    buzzerCounter = 0;
                    //setColor(255, 0, 0);  // red
                  }

                  break;
              case COUNTDOWN_COMPLETE:
                  //TheaterChaseUpdate();
                  if(buzzerCounter < numberOfBeeps*2){
                    BuzzerUpdate();  
                  }
                  
                  LEDUpdate();
                                    
                  break;
              default:
                  break;
          }
      }
  }

 /* void CountdownIncrement()
  {
    
  }

*/

  // logic to manage how long the buzzer is buzzing without the dreaded blocking "delay()" function
  void BuzzerUpdate()
  {

      if((millis() - buzzerToneBeginTime) > buzzerInterval){
        if((buzzerCounter % 2 ) == 0){
          // in the BOM for this project, I specified using a 2khz active piezo which supposedly only requires 3-5v
          // in order to get it to make a 2kHz tone. But I swear when I hooked everything up, 
          // the piezo was able to be controlled by the "tone" function which worked as you'd expect it to
          // and produced a nice tone at 1,200 Hz  ¯\_(ツ)_/¯
          // if you have issues with the Piezo you're using. sorry
          tone(buzzerPin, frequency);
          
          //digitalWrite(buzzerPin, 1);
          //setColor(255, 0, 0);  // red
          buzzerInterval = buzzerOffInterval;
        }
        else{
            noTone(buzzerPin);
            
            //digitalWrite(buzzerPin, 1);
            //setColor(255, 0, 0);  // red
            buzzerInterval = buzzerOnInterval;
        }
        buzzerToneBeginTime = millis();
        buzzerCounter++;
      }
    
  }


    // Increment/Decrement the LEDFadeIndex to increase or decrease the brightness of the LED depending on Direction variable
    void LEDFadeIncrement()
    {
        if (Direction == FORWARD)
        {
           LEDFadeIndex += LEDFadeIndexStepIncrementAmount;

           // if the LED gets to 255 brightness, flip the script and start making it dimmer
           if (LEDFadeIndex >= TotalLEDFadeSteps)
            {
                Direction = REVERSE;
                
                // we don't need no stinking callbacks...but maybe I'll use them later
                /*
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
                */
            }
        }
        else // Direction == REVERSE -- in other words lower the brightness every time
        {   
            LEDFadeIndex -=  LEDFadeIndexStepIncrementAmount;

            // if the LED gets to 0 brightness, flip the script and start making it brighter
            if (LEDFadeIndex <= 0)
            {
                Direction = FORWARD;

                // we don't need no stinking callbacks...but maybe I'll use them later
                /*
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
                */
            }
        }
    }



  void LEDUpdate()
  {

      if((millis() - ledFadeUpdate) > ledFadeInterval){
        ledFadeUpdate = millis();
        
        setColor(LEDFadeIndex, 0, 0);  // red
        
        LEDFadeIncrement();

      }
    
  }



  void StartCountdown(unsigned long numMinutesToCount)
  {
      // the tickTimeMultiple variable sets how many milliseconds to add for each position of the rotary switch (which looks like tick marks from the outside)
      // one minute is 60,000 millis duh! set this to 30,000 if you want each tick to be 30 second increments
      // or come up with some logic here to decide the number of seconds each tick represents if you wanna get fancy pants
      MillisToCount = numMinutesToCount * tickTimeMultiple;
      CountdownStartTime = millis();
      
      ActiveState = COUNTING_DOWN;
      
      // Turn on Green light while the countdown is running
      setColor(0, 255, 0);  // green
          
  }



  void setColor(int red, int green, int blue)
  {
    #ifdef COMMON_ANODE
      red = 255 - red;
      green = 255 - green;
      blue = 255 - blue;
    #endif
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);  
  }
      


  // old code that I stole from somewhere and modified for my own purposes. again, I don't remember where I stole this from so unfortunately can't give credit but it's pretty basic
  /*
  // Increment the Index and reset at the end
  void Increment()
  {
      if (Direction == FORWARD)
      {
         Index++;
         if (Index >= TotalSteps)
          {
              Index = 0;
              if (OnComplete != NULL)
              {
                  OnComplete(); // call the comlpetion callback
              }
          }
      }
      else // Direction == REVERSE
      {
          --Index;
          if (Index <= 0)
          {
              Index = TotalSteps-1;
              if (OnComplete != NULL)
              {
                  OnComplete(); // call the comlpetion callback
              }
          }
      }
  }



  void Function1()
  {
     //if enough time has passed, read the button
      if( (currentTime - lastButtonReadTime) > buttonReadInterval )
      {
          // read the button and store it in the variable buttonStateReading
          int buttonStateReading = digitalRead(buttonPin); // read the button

          // if the button has changed since last reading, update the button State
          if (buttonStateReading != buttonState) {
            buttonState = buttonStateReading;

            // only do something if the buttonState is LOW
            if (buttonState == LOW) {
              
              //reset the timer and update the LED
              //LEDStrip->resetLastupdateTime();
              ledState = !ledState;
              digitalWrite(11, ledState);
              lastButtonTriggerTime = currentTime;
            }
           
            
            
          }
          
          lastButtonReadTime = currentTime;
      }

  }

*/

  


};
