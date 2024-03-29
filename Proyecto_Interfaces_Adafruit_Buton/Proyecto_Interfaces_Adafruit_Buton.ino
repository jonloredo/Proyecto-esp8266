/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  0;      // the number of the LED pin
  // Pin 13: Arduino has an LED connected on pin 13
  // Pin 11: Teensy 2.0 has the LED on pin 11
  // Pin  6: Teensy++ 2.0 has the LED on pin 6
  // Pin 13: Teensy 3.0 has the LED on pin 13

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {

  /*Serial.begin(115200);
  delay(100);*/
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);    

  pinMode(15,OUTPUT); // pin D8 salida
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = !digitalRead(buttonPin);  //Se encuentra invertido por resistencia pull-up en pin GPIO2
  //Serial.println(!digitalRead(buttonPin));

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
    analogWrite(15, 8); 
    delay(180);
    analogWrite(15, 0); 
    delay(180);
    analogWrite(15, 8); 
    delay(180);
    analogWrite(15, 0);
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
    digitalWrite(15, LOW); 
  }
}
