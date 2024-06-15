/*
Potrebuje:
https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip
https://github.com/me-no-dev/AsyncTCP/archive/master.zip

dokumentace k webservu zde:
https://github.com/me-no-dev/ESPAsyncWebServer/tree/master

*/

/**
Nastaveni kompilace
*/


/**
Ovládání:

1) V mobilu vypnout data, zapnout wifi, připojit na síť
    MORNING*STAR
  s heslem
    zlutalampa

2) V prohlížeči 
  http://192.168.1.1
  a zobrazí se informace o stavu

*/


// Import required libraries

#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

DNSServer dnsServer;


/* Konfiguracni soubor na SPIFFS filesystemu. Urcite se to da resit jednoduseji, ale tohle mam hotove z jineho projeku. */
#include "ConfigProvider.h"
#include "src/ra/raConfig.h"
#include "src/ra/raLogger.h"
raLogger* logger;
raConfig config;
#define RA_CONFIG_PASSPHRASE "--------"

/*
#include <ESP32AnalogRead.h>
ESP32AnalogRead adc4;
*/

#include "Tasker.h"
Tasker tasker;


// Replace with your network credentials
const char* ssid = "MORNING*STAR";
const char* password = "zlutalampa";

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);


#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        14 

// How many NeoPixels are attached to the Arduino?

#define NUMPIXELS 6
//  The overall fire brightness
//  (this can affect both color levels and power consumption)
int brightness = 128;
bool brightnessChange = false;

int color = 0xffffff;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


/**
1 = ohen
2 = morzeovka
3 = konstantni barva
4 = blikani
*/
int aktualniRezim = 1;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



/** nacte konfiguraci ze souboru */
void loadConfigData() {
  
  // const char * p = config.getString( "limitBaterka", "12.3" ); 
  // limitBaterka = atof( p );
  // Serial.printf( "limitBaterka = %f\n", limitBaterka);

  aktualniRezim = (int) config.getLong( "rezim", 1 );
  color = (int) config.getLong( "color", 0xffffff );
  brightness = (int) config.getLong( "jas", 128 );

  if( aktualniRezim==1 ) {
    tasker.setTimeout(fireAction, 1000 );
  } else if( aktualniRezim==2 ) {
    int signalMode = (int) config.getLong( "signalMode", 1 );
    signalZacni( signalMode );
  } else if( aktualniRezim==3 ) {
    char morseText[200];
    strncpy( morseText, config.getString( "morseText", "abcd"), 199 );
    morseText[199] = 0;
    int morseSpeed =  (int) config.getLong( "morseSpeed", 100 );
    morseZacni( morseText, morseSpeed );
  }
}

/** ulozi konfiguraci do souboru */
void saveConfigData() {
  char buf[20];

  sprintf( buf, "%d", aktualniRezim );
  config.setValue( "rezim", buf );
  
  sprintf( buf, "%d", color );
  config.setValue( "color", buf );

  sprintf( buf, "%d", brightness );
  config.setValue( "jas", buf );
}





void setup(){
  Serial.begin(115200);
  delay(2000);
  Serial.println( "Startuju" );

  logger = new raLogger( RA_LOG_MODE_SERIAL );
  logger->log( "raLogger started");

  config.setInfo( (char*)"-", (char*)RA_CONFIG_PASSPHRASE );
  loadConfig( &config );
  loadConfigData();

  pixels.begin(); 
  pixels.setBrightness(brightness);
  pixels.show();

  // adc4.attach(4);

  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  WiFi.softAP(ssid, password, 1, false );
  Serial.println("Wait 200 ms for AP_START...");
  delay(500);
  WiFi.softAPsetHostname(ssid);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  WiFi.softAPsetHostname(ssid);

  webserverBegin();
  dnsServer.start(53, "*", WiFi.softAPIP());

  // tasker.setInterval(vypisStav, 5000);
}


void zhasni() {
  pixels.clear();
  pixels.show();
}

void rozsvit() {
  for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i,(color&0xff0000)>>16,(color&0xff00)>>8, color&0xff);
    }
    pixels.show();
}

bool rozsviceno = false;

void loop() {
  if( brightnessChange ) {
    logger->log( "jas %d", brightness );
    pixels.setBrightness(brightness);
    brightnessChange = false;
  }

  if( aktualniRezim==0 ) {
    if( rozsviceno ) {
      logger->log( "zhasinam" );
      zhasni();
      rozsviceno = false;
    }
  } else {
    rozsviceno = true;
  }

  if( config.isDirty() ) {
    saveConfig( &config );
  }

  dnsServer.processNextRequest();
  tasker.loop();
}

/*
Using library DNSServer at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\DNSServer 
Using library WiFi at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\WiFi 
Using library AsyncTCP at version 1.1.1 in folder: C:\Users\brouzda\Documents\Arduino\libraries\AsyncTCP 
Using library ESP Async WebServer at version 1.2.3 in folder: C:\Users\brouzda\Documents\Arduino\libraries\ESPAsyncWebServer 
Using library FS at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\FS 
Using library Tasker at version 2.0.3 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Tasker 
Using library Adafruit NeoPixel at version 1.11.0 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Adafruit_NeoPixel 
Using library SPIFFS at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SPIFFS 
*/