
// I ended up not using this button class for this project, but why not leave it in here for later just in case?
//      If not for actually using it, then at least for nostalgia sake LOL
//      Also, I have a feeling I snagged this class from somewhere on the internet but didn't comment my code until 
//      much later, so forgot who to give credit for the boilerplate of this class. sorry internet!


#include "Arduino.h"

class MyButtonClass
{
  public:

  int buttonPin; 
  int buttonState;
  
  // These maintain the current state
  unsigned long lastButtonReadTime;
  unsigned long lastButtonTriggerTime;

  unsigned long buttonReadInterval; 
  //unsigned long sensorTriggerInterval;
  int ledState;
  
  
  //void (*OnComplete)();  // Callback on completion of pattern

  
  // Constructor - 
  // initializes the member variables and state
  
  MyButtonClass(int theButtonPin)
  {

    buttonPin = theButtonPin; // the button is connected to digital pin designated by this variable
    buttonState = digitalRead(buttonPin);

    buttonReadInterval = 2;
    //buttonTriggerInterval = 70;

    lastButtonReadTime = millis();
    lastButtonTriggerTime = millis();
    ledState = HIGH;
    
  }

  void Update(unsigned long currentTime)
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


};
