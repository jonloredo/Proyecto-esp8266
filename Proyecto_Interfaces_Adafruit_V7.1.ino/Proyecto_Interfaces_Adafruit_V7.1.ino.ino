#define D0 16
//#define D1 5 SCL para Display
//#define D2 4 SDA para Display
#define led_auto 0
#define boton_auto 2
#define D5 14
#define BOMBA 12
#define D7 13
#define D8 15

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include "customChar.h"
#include <Ticker.h>

//Declaración de Ticker para un tiempo1 es un pseudo-temporizador se usará más adelante
Ticker tiempo1;
//Ticker tiempo2;

int auto_feed=0;
int riego_feed;

//Credenciales adafruit

#define IO_USERNAME  "jonloredo"
#define IO_KEY       "41a895c49a304c1eb9786a6900b6b54e"

//Credenciales Wifi

/******************************* WIFI **************************************/

/*#define WIFI_SSID   "Totalplay-549E"
#define WIFI_PASS   "549EC05675mRFp3U"  */

#define WIFI_SSID   "jonloredoWiFi"
#define WIFI_PASS   "labrador86"

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


// DHT Sensor e inicialización

#define DHTTYPE DHT11


uint8_t DHTPin = D5;
DHT dht(DHTPin, DHTTYPE);  


// set up the 'temperature' and 'humidity' feeds
  AdafruitIO_Feed *temperatura2io = io.feed("temperatura_dht11");
  AdafruitIO_Feed *humedad2io = io.feed("humedad_dht11");

// Adafruit riego
  AdafruitIO_Feed *nriegosensor = io.feed("nivel_de_riego");

// set up the 'regar' feed para activar función en dashboard adafruit
  AdafruitIO_Feed *regar = io.feed("regar");
  AdafruitIO_Feed *autom = io.feed("autom");
  int autom_feed;

  
int temperatura_dht11;
int humedad_dht11;

//Variables riego
int value;
String n_riego;

//Constructor LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);



void setup() {

  Serial.begin(115200);
  //delay(100);

 // Definimos pin asignado a BOMBA como salida
  pinMode(BOMBA, OUTPUT);

//Setup para LCD
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, Gota);      //<- Se crean iconos especiales para mostrar en pantalla LCD 
  lcd.createChar(1, Termometro);// fuente: https://maxpromer.github.io/LCD-Character-Creator/

//Definición de Pin para DHT e inicialización
  pinMode(DHTPin, INPUT);

  dht.begin();    

    // connect to io.adafruit.com
  Serial.println();
  Serial.println("Connecting to Adafruit IO");
  io.connect();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("Adafruit IO ...");
  delay(1500);

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  // Adafruit IO Digital Output Example
  // Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
  
  regar->onMessage(handleMessage); // Configurar con el nombre de mi feed 'regar'
  autom->onMessage(handleMessage2);  // Configurar con el nombre de mi feed 'autom'
  
  // wait for a connection
  int i=0;
  lcd.clear();
  while(io.status() < AIO_CONNECTED) {
  Serial.print(".");
  lcd.setCursor(i, 1);
  lcd.print(".");
  i++;
  if(i==15){
  lcd.clear();
  }
  
  delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  regar->get(); // Configurar con el nombre de mi feed 'regar'
  autom->get(); // Configurar con el nombre de mi feed 'autom'
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adafruit IO");
  lcd.setCursor(0, 1);
  lcd.print("connected!!");
  delay(1500);
  lcd.clear();  

  /*Se utiliza Ticker como pseudo-temporizador cada 30 segundos llamando la 
  función nivel_riego() con operador attach para poder seguir haciendo llamadas a otras funciones en
  el programa ej enlace https://circuits4you.com/2018/01/02/esp8266-timer-ticker-example/ */
  tiempo1.attach(30,nivel_riego);
  //tiempo2.attach(2,automata);

//Configuración botones de riego y automatización 
  pinMode(boton_auto,INPUT);
  pinMode(led_auto,OUTPUT);

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

//Lectura y muestra de datos de DHT11 en pantalla LCD

temperatura_dht11 = int(dht.readTemperature()); // Gets the values of the temperature only integer values not float with int()
humedad_dht11 = int(dht.readHumidity()); // Gets the values of the humidity 

// REVISAR QUE LOS RESULTADOS SEAN VALORES NUMERICOS VALIDOS, INDICANDO QUE LA COMUNICACION ES CORRECTA
  if (isnan(humedad_dht11) || isnan(temperatura_dht11)) {
    Serial.println("Falla al leer el sensor DHT11!");
    return;
  }

  //Mostrar valores de Temperatura y humedad en monitor serial y pantalla LCD
  Serial.print("Temperatura: ");
  Serial.println(temperatura_dht11);

  Serial.print("Humedad: ");
  Serial.println(humedad_dht11);
    
  lcd.setCursor(0, 0);
  //lcd.print("T=");
  lcd.write(1);
  lcd.setCursor(2, 0);
  lcd.print(temperatura_dht11);
  lcd.setCursor(4,0);
  lcd.print((char)223); //<- Necesario para desplegar caracter especial ªC en LCD
  lcd.setCursor(5,0);
  lcd.print("C  ");
  
  lcd.setCursor(7, 0);
 // lcd.print("H=");
  lcd.write(0);
  lcd.setCursor(9, 0);
  lcd.print(humedad_dht11);
  lcd.setCursor(11,0);
  lcd.print("%  ");
 
  /*Mostrará niveles de riego en cinco niveles diferentes dependiendo el voltaje 
  de entrada a pin A0 y mostrará salida ocupando toda la segunda fila del LCD*/
  value= analogRead(A0);
  Serial.println(value);

    if(value>432){
    n_riego = String("Muy Alto");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);
     
    lcd.setCursor(0, 1);
    lcd.print("Riego: Muy Alto");    
   
 
   }
   
    if(value>324 && value<=432){
    n_riego = String("Alto");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);

    lcd.setCursor(0, 1);
    lcd.print("Riego: Alto     ");
    

   }
   
    if(value>216 && value<=324){
    n_riego = String("Bueno");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);
    
    lcd.setCursor(0, 1);
    lcd.print("Riego: Bueno    ");
    

   }
   
    if(value>108 && value<=216){
    n_riego = String("Bajo");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);

    lcd.setCursor(0, 1);
    lcd.print("Riego: Bajo     ");
   

   }
   
    if(value<=108){
    n_riego = String("Muy Bajo");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);

    lcd.setCursor(0, 1);
    lcd.print("Riego: Muy Bajo ");
    delay(350);
    lcd.setCursor(0, 1);
    lcd.print("Riego:          ");
    delay(350);
    
   }
   

//Comparación de variable de control para automatizar si auto_feed=1 auto esta ON
//si auto_feed=0 auto esta OFF

  if(auto_feed == 1 && value < 108)
    digitalWrite(BOMBA,HIGH);
  if(auto_feed == 1 && value >= 324)
    digitalWrite(BOMBA,LOW);
  if(auto_feed == 0)
    digitalWrite(BOMBA,LOW);

//Activación de botón para automatización

int botonverde=!digitalRead(boton_auto);
delay(10);

if(botonverde == HIGH){
  auto_feed = !auto_feed;
  autom->save(auto_feed);
  //delay(240);
  }

if(auto_feed==1)
  digitalWrite(led_auto,HIGH);
else
  digitalWrite(led_auto,LOW);
    
}

//Función para envíar valores de nivel de riego, temperatura y humedad a Adafruit

void nivel_riego(){
  
  temperatura2io->save(temperatura_dht11);
  humedad2io->save(humedad_dht11); 
  nriegosensor->save(n_riego); 
  
  }

/*void automata(){

   if(autom_feed == 1 && value<216)
      digitalWrite(BOMBA,HIGH);
    
   
    else
      digitalWrite(BOMBA,LOW);
  
  }*/

// this function is called whenever an 'regar' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  if(value<432)
  digitalWrite(BOMBA, data->toPinLevel());  //Activa riego de la bomba al recibir 1


  
}


// this function is called whenever an 'autom' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage2(AdafruitIO_Data *data2) {

 
  if(data2->toPinLevel() == HIGH){
    auto_feed=1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modo");
  lcd.setCursor(0, 1);
  lcd.print("Automático");

  if(auto_feed == 1){ //Se activa dos veces buzzer para avisar está encenido modo auto
  analogWrite(15, 8); 
  delay(180);
  analogWrite(15, 0); 
  delay(180);
  analogWrite(15, 8); 
  delay(180);
  analogWrite(15, 0);
  }
  delay(800);
  
  }
  else{
    auto_feed=0;
    
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modo");
  lcd.setCursor(0, 1);
  lcd.print("Manual");
  delay(1300);
  }
  lcd.clear();
}
