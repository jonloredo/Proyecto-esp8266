// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <Ticker.h>  //Ticker Library

/************************ Example Starts Here *******************************/

// digital pin 5
#define BOMBA 12

//Declaración de Ticker para un tiempo1 es un pseudo-temporizador se usará más adelante
Ticker tiempo1;

// set up the 'digital' feed
AdafruitIO_Feed *autom = io.feed("autom");

int automatico;

void setup() {
  
  pinMode(BOMBA, OUTPUT);
  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  autom->onMessage(handleMessage2);  // <- CONFGURAR CON EL NOMBRE DE MI 'FEED'

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  autom->get(); // <- CONFGURAR CON EL NOMBRE DE MI 'FEED'

  tiempo1.attach_ms(100,automata);

}

  

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage2(AdafruitIO_Data *data2) {

  Serial.print("received <- ");

  if(data2->toPinLevel() == HIGH){
    Serial.println("HIGH");
    automatico=1;}
  else{
    Serial.println("LOW");
    automatico=0;}


  
}

void automata(){

   if(automatico == 1)
      digitalWrite(BOMBA,HIGH);

    else
      digitalWrite(BOMBA,LOW);
  
  }
