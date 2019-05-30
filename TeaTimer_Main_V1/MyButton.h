/*
  MyButton.h - I ended up not using this button class for this project, but why not leave it in here for later just in case?
                If not for actually using it, then at least for nostalgia sake LOL
                Also, I have a feeling I snagged this class from somewhere on the internet but didn't comment my code until 
                much later, so forgot who to give credit for the boilerplate of this class
*/
#ifndef MyButton_h
#define MyButton_h

#include "Arduino.h"

class MyButton
{
  public:
    MyButton(int buttonPin);
    //void dot();
    //void dash();
    
  private:
      int buttonPin; 
      int buttonState;
  
      // These maintain the current state
      unsigned long lastButtonReadTime;
      unsigned long lastButtonTriggerTime;

      unsigned long buttonReadInterval; 
};

#endif
