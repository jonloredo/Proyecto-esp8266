#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//Credenciales adafruit

#define IO_USERNAME  "jonloredo"
#define IO_KEY       "41a895c49a304c1eb9786a6900b6b54e"

//Credenciales Wifi

/******************************* WIFI **************************************/

// the AdafruitIO_WiFi client will work with the following boards:
//   - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
//   - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
//   - Feather HUZZAH ESP32 -> https://www.adafruit.com/product/3405
//   - Feather M0 WiFi -> https://www.adafruit.com/products/3010
//   - Feather WICED -> https://www.adafruit.com/products/3056
//   - Adafruit PyPortal -> https://www.adafruit.com/product/4116
//   - Adafruit Metro M4 Express AirLift Lite -> https://www.adafruit.com/product/4000
//   - Adafruit AirLift Breakout -> https://www.adafruit.com/product/4201

#define WIFI_SSID   "Totalplay-549E"
#define WIFI_PASS   "549EC05675mRFp3U"

#include "AdafruitIO_WiFi.h"

#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE)
  // Configure the pins used for the ESP32 connection
  #if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
    // Don't change the names of these #define's! they match the variant ones
    #define SPIWIFI SPI
    #define SPIWIFI_SS 10  // Chip select pin
    #define SPIWIFI_ACK 9  // a.k.a BUSY or READY pin
    #define ESP32_RESETN 6 // Reset pin
    #define ESP32_GPIO0 -1 // Not connected
  #endif
  AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
#else
  AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
#endif

/**********************************************/

#define DHTTYPE DHT11

// DHT Sensor
uint8_t DHTPin = D2;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);  


// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperatura = io.feed("temperatura");
AdafruitIO_Feed *humidity = io.feed("humidity");

float Temperature;
float Humidity;

void setup() {

  Serial.begin(115200);
  delay(100);

  pinMode(DHTPin, INPUT);

  dht.begin();    

            // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {

   // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 

  Serial.print("Temperatura: ");
  Serial.println(Temperature);

  Serial.print("Humedad: ");
  Serial.println(Humidity);

  temperatura->save(Temperature);
  humidity->save(Humidity)
  
  
  delay(5000);

}



