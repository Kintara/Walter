// ESP32 DEV MODULE ESP WROOM-32 30 pin model
//This is a cutdown version - no keypad. A working version that ties into the RPi and the Beamz
//Changed MaxAir and MaxFlow to 255
//1102 works but no buttons and ony 0 to 255 ranges
//This Version
//Button rnabled
//Webpage 2 ranges enabled
//Attempt to switch ranges fromm pressing Blow button on webpage Done
int version = 1;  //From 12023 - very few errors errors; 
#include <EEPROM.h> //Copyright (c) 2014 Ivan Grokhotkov
#include <WiFi.h>        // standard library WiFi.h - esp32 Wifi support. Based on WiFi.h from Arduino WiFi shield library. Copyright (c) 2011-2014 Arduino.  All right reserved. Modified by Ivan Grokhotkov, December 2014
#include <WebServer.h>   // standard library
#include "SnowWeb200.h"  // .h file that stores your html page code High Air and flow values
#include "SnowWeb100.h"  // .h file that stores your html page code
#include <Arduino.h> 
#include <esp_dmx.h>  //v4.1.0 Mitch Weisbrod NEW from V12022

#include <Wire.h> /*
 
*/
#include <Adafruit_GFX.h> //1.1.10 OLED
#include <Adafruit_SSD1306.h> //2.5.11 OLED



//++ ++ ++ ++ ++ ++ ++ ++ ++
// here you post web pages to your homes intranet which will make page debugging easier
// as you just need to refresh the browser as opposed to reconnection to the web server
//#define USE_INTRANET

// replace this with your homes intranet connect parameters
#define LOCAL_SSID "Plume"
#define LOCAL_PASS "Allthedogs"

// once  you are ready to go live these settings are what your client will connect to
#define AP_SSID "BeamZ"
#define AP_PASS ""

// start your defines for pins for sensors, outputs etc.#define PIN_LED 2      //On board LED

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// gotta create a server
WebServer server(80);
int ButtonPad = 255;
int Page = 1;
int Air = 26;
int Flow = 0;
int IntervalStart = 20000;
int Interval = IntervalStart;
int IntervalLeft = IntervalStart;
int DurationStart = 10000;
int Duration = DurationStart;
int DurationLeft = DurationStart;
int MaxAir = 255;
int MaxFlow = 255;
//define the pins for the buttons 
#define B1 23 //Button 1 (Yellow)
#define B2 27 //Button 2 (Red / Blue)
#define B3 36 //Button  (Green)
#define B4 39 //Button 
#define B5 14 //Button 
#define B6 19 //Button 
#define B7 34 //Button 
#define B8 12 //Button 
#define B9 18 //Button 
#define B10 35 //Button 
#define B11 13 //Button 
#define B12 15 //Button 


//define the pins to send signals to the Rpi Zero2 W
#define PIN_LED 2
#define GPIO_Pin_Snow 25  //To Rpi Zero2 W to trigger Snowing sequence > CAT pin 3 Active Hi
#define GPIO_Pin_AwaitCommandFromRPi 33        //From Rpi Zero2 W to stop the blower and await Snow Command from RPi  > CAT pin 4 Active Hi
#define GPIO_Pin_ProduceSnowCommandFromRpi 32  //From Rpi Zero2 W : Start Snow! command > CAT pin 5 Active Hi
int RPiStatus = 0;
unsigned long ButtonTime = 0;
unsigned long LastButtonTime = 0;
unsigned long StartIntervalMills = 0;
unsigned long IntervalMills = 0;
unsigned long StartDurationMills = 0;
unsigned long DurationMills = 0;
unsigned long LEDupdate = 0;

String password = "";
int change = 0;
int CH_Number = 2;
bool ButtonMode = true;
bool KeypadActive = false;
bool BeamZActive = false;
int CH_Value = 0;
int timingMode = 1;  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence

String RPiMode = "";
bool OldTime = true;
int OldtimingMode = 0;  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence
int ManTime = 1;
int PushButtonMode = 0;
int PushButtonCount = 0;
/* Now we want somewhere to store our DMX data. Since a single packet of DMX
  data can be up to 513 bytes long, we want our array to be at least that long.
  This library knows that the max DMX packet size is 513, so we can fill in the
  array size with `DMX_Buffer */

//byte data[DMX_PACKET_SIZE];

// the XML array size needs to be bigger that your maximum expected size. 2048 is way too big for this example
char XML[2048];

// just some buffer holder for char operations
char buf[32];




// variable for the IP reported when you connect to your homes intranet (during debug mode)
IPAddress Actual_IP;

// definitions of your desired intranet created by the ESP32
IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;



/* DMX : First, lets define the hardware pins that we are using with our ESP32. We
  need to define which pin is transmitting data and which pin is receiving data.
  DMX circuits also often need to be told when we are transmitting and when we
  are receiving data. We can do this by defining an enable pin. */
int transmitPin = 17;
int receivePin = 16;
int enablePin = 4;
/* Make sure to double-check that these pins are compatible with your ESP32!
  Some ESP32s, such as the ESP32-WROVER series, do not allow you to read or
  write data on pins 16 or 17, so it's always good to read the manuals. */

/* Next, lets decide which DMX port to use. The ESP32 has either 2 or 3 ports.
  Port 0 is typically used to transmit serial data back to your Serial Monitor,
  so we shouldn't use that port. Lets use port 1! */
dmx_port_t dmxPort = 1;
byte data[DMX_PACKET_SIZE];
//int DMX_Buffer_Size = 10;


/* This variable will allow us to update our packet and print to the Serial
  Monitor at a regular interval. */
unsigned long lastUpdate = millis();

// RJM LOGO
#define LOGO_HEIGHT 64
#define LOGO_WIDTH 128
static const unsigned char PROGMEM logo_bmp[] = {  //RJM Logo
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xc0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfd, 0xff, 0x00, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x80, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x7f, 0xc0, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xe0, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xf0, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xf8, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x07, 0xfc, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0xfe, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0xff, 0x00, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0xff, 0x80, 0x7e, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x7f, 0xc1, 0xfe, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x3f, 0xff, 0xfe, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x1f, 0xff, 0xfe, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xff, 0xfe, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x07, 0xff, 0xfe, 0x03, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x03, 0xff, 0xfc, 0x03, 0xe0, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x01, 0xff, 0xf8, 0x01, 0xc0, 0x0c, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00
};





void setup() {
   pinMode(PIN_LED, OUTPUT);
   
  pinMode(B1, INPUT_PULLDOWN);
  pinMode(B2, INPUT_PULLDOWN);
  pinMode(B3, INPUT_PULLDOWN);
  pinMode(B4, INPUT_PULLDOWN);
  pinMode(B5, INPUT_PULLDOWN);
  pinMode(B6, INPUT_PULLDOWN);
  pinMode(B7, INPUT_PULLDOWN);
  pinMode(B8, INPUT_PULLDOWN);
  pinMode(B9, INPUT_PULLDOWN);
  pinMode(B10, INPUT_PULLDOWN);
  pinMode(B11, INPUT_PULLDOWN);
  pinMode(B12, INPUT_PULLDOWN);
  //digitalWrite(2, true);
  // put your setup code here, to run once:
  Serial.begin(115200);
//Wire.begin(4, 5); // Set the pins for SDA, SCL
  delay(200);
  
  digitalWrite(2, false);
 // delay(100);
 // digitalWrite(2, true);
  // OLED Screen test
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C is Default for 128x64; alternative adress is 0x3D
    Serial.println(F("M1 SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.setRotation(2);

  display.clearDisplay();

  //display RJM LOGO
  display.drawBitmap(0, 0, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  //delay(100);
  display.startscrollleft(0x00, 0x0f);
  //delay(200);
  display.stopscroll();
  display.clearDisplay();
  display.drawBitmap(32, 0, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();

  display.setRotation(2);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Snow");
  display.println("Blow");
  display.println("Man");
  display.print("V ");
  display.print(version);
  display.display();
  //(500);
  
 // digitalWrite(2, false);
  // Clear the buffer
  display.clearDisplay();


  pinMode(GPIO_Pin_Snow, OUTPUT); // ESP sending SNOW signal to RPi
  pinMode(GPIO_Pin_AwaitCommandFromRPi, INPUT_PULLDOWN); //From RPi
  pinMode(GPIO_Pin_ProduceSnowCommandFromRpi, INPUT_PULLDOWN); //Frpm RPi
 //blink(1);
 
  //5. To read the SSID, password, and IP address from the EEPROM, you can use the following code in the setup() function:
  EEPROM.begin(512);
  String ssid = "";
  String ip_address = "";
  Serial.println("Reading EEPROM");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Reading EEPROM");
  display.display();
  ////delay(500);
  for (int i = 0; i < EEPROM.read(0); i++) {
    ssid += char(EEPROM.read(i + 1));
  }
  //ssid = "Plume";
  for (int i = 0; i < EEPROM.read(EEPROM.read(0) + 1); i++) {
    password += char(EEPROM.read(i + EEPROM.read(0) + 2));
  }

  Serial.println("Home WiFI: " + ssid + " Password: " + password);

  if (ssid == "" || password == "") {
    // If the SSID, password, or IP address is missing, act as an access point */

    Serial.println("Nothing Read in EPROM");
    display.clearDisplay();
    display.println("Nothing Read in EPROM");
    display.println("Using SoftAP");
    display.println("BeamZ : ");
    
    ip=PageIP;
    WiFi.softAP("BeamZ", "");
    Serial.println("AP Mode BeamZ &  IP=192.168.1.1");
    delay(500);
    WiFi.softAPConfig(PageIP, gateway, subnet);
    BeamZActive= true;
  } else {
    // Connect to Wi-Fi
    Serial.println();
    Serial.println("Using EEPROM Values");
    Serial.println(ssid);
    Serial.println(password);
    WiFi.begin(ssid, password);
    display.clearDisplay();
    display.println("Use EEPROM Values ");
    display.println(ssid);
    display.println(password);
    // Wait for connection
    Serial.println("Waiting for Connection");
    int connectionTimeout = 5000;  // 5 seconds
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < connectionTimeout) {
      Serial.print(".");
      display.print(".");
      display.display();
      //delay(500);
      BeamZActive = false;
    }
    display.display();
    //delay(200);
    Serial.println("Time Up or Connected");
    if (WiFi.status() == WL_CONNECTED) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      Serial.println("Connected to Wi-Fi");
      Serial.println(ssid);
      Serial.println(password);
      display.println("Connected");
      display.println("to Wi-Fi");
      display.println(ssid);
      display.println(password);
      display.display();
      //delay(3000);
      
  blink(2);
    } else {
      Serial.println("Failed to connect. Creating an access point...");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Failed to connect");
      display.println("to WiFi");
      display.println(ssid);
      display.println(password);
      Serial.println(ssid);
      Serial.println(password);
      display.println("Creating access point...");
      // Set up your own access point here
      WiFi.softAP("BeamZ", "");
      Serial.println("AP Mode BeamZ &  IP=192.168.1.1");
      WiFi.softAPConfig(PageIP, gateway, subnet);
      ip_address = "192.168.1.1";
      ssid = "BeamZ";
      password = "";
      BeamZActive=true;
      display.display();
      //delay(2000);
      
  blink(3);
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Setting Up");
      display.println("\nBeamZ");
      display.println("\n1234");
      display.println("\n192.168.1.1");
      display.display();
      display.setTextSize(1);
      //delay(3000);
    }
  }
blink(4);
// New version of esp.dmx
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_personality_t personalities[] = {};
  int personality_count = 0;
  dmx_driver_install(dmxPort, &config, personalities, personality_count);

  /* Now set the DMX hardware pins to the pins that we want to use and setup
    will be complete! */
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);
 
 // dmx_config_t config = DMX_CONFIG_DEFAULT;
  
//blink(5);
//  dmx_driver_install(dmxPort, &config, DMX_INTR_FLAGS_DEFAULT);

blink(6);
  /* Now set the DMX hardware pins to the pins that we want to use and setup
    will be complete! */
 // dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);
  
blink(7);
 //digitalWrite(2, true);

  for (int x; 0; 255) {
    data[x] = 0;
  }
  data[1] = 0;  //Air
  data[2] = 0;  //Flow
                // if your web page or XML are large, you may not get a call back from the web page
  // and the ESP will think something has locked up and reboot the ESP
  // not sure I like this feature, actually I kinda hate it
  // disable watch dog timer 0
 // disableCore0WDT();
blink(8);
  // maybe disable watch dog timer 1 if needed
 //   disableCore1WDT();
blink(9);
  // just an update to progress
  Serial.println("starting server");

  blink(10);
  // if you have this #define USE_INTRANET,  you will connect to your home intranet, again makes debugging easier
#ifdef USE_INTRANET
  WiFi.begin(LOCAL_SSID, LOCAL_PASS);
  endif while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Actual_IP = WiFi.localIP();
#endif

  // if you don't have #define USE_INTRANET, here's where you will creat and access point
  // an intranet with no internet connection. But Clients can connect to your intranet and see
  // the web page you are about to serve up
#ifndef USE_INTRANET
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
#endif

  printWifiStatus();

  blink(11);
  
  // these calls will handle data coming back from your web page
  // this one is a page request, upon ESP getting / string the web page will be sent
  server.on("/", SendWebsite);
  server.on("/xml", SendXML);
  server.onNotFound(handle_NotFound);
  server.on("/intervalSlider", UpdateIntervalSlider);
  server.on("/durationSlider", UpdateDurationSlider);
  server.on("/airSlider", UpdateAirSlider);
  server.on("/flowSlider", UpdateFlowSlider);
  server.on("/PressStop", processStopButton);
  server.on("/PressSkip", processSkipButton);
  server.on("/PressTime", processTimeButton);
  server.on("/ProcessBlow", handleBlow);
  server.on("/ProcessRJM", handleRJM);
  server.on("/ProcessReset", handleReset);
  server.on("/PressControl", handleControl);
  server.on("/SaveIPCredentials", processIPCredentials);
  server.begin();
  CH_Number = 2;
}

void loop() {
  // put your main code here, to run repeatedly:
  //******************************************************************************************************
  /* Get the current time since boot in milliseconds so that we can find out how
    long it has been since we last updated data and printed to the Serial
    Monitor. */
    
  unsigned long now = millis();
  //indicate button has been pressed
  switch (change) {
    case 1:
      LEDupdate = now;
     // digitalWrite(2, true);
      change = 3;
      break;
    case 3:
      if (now - LEDupdate >= 500) {
        change = 0;
      }
      break;
  }


  // no matter what you must call this handleClient repeatidly- this is listening for any requests for the webpage from a browser-otherwise the web page
  // will not get instructions to do something
  server.handleClient();  //makes the webpage execute the <body style="background-color: #efefef" onload="process()"> and runs the function process()

  if (now - lastUpdate >= 1000) {
    lastUpdate = now;
    Serial.println(digitalRead(GPIO_Pin_AwaitCommandFromRPi));
//See if the Rpi wants control Pin14 = hi
    if (digitalRead(GPIO_Pin_AwaitCommandFromRPi) == 1 ) {
       RPiStatus = 1; //0=Normal; 1 = Rpi in control
      timingMode = 4;      //0=Stop 1=interval 2= Duration ; 3= Full Snow ;4=RPi Control
      RPiMode ="Primed";
      //digitalWrite(PIN_LED, true);          
          IntervalLeft = Interval;
   //further see if Rpi want snow Pin 13 = Hi and Pin 14 = hi
     if (digitalRead(GPIO_Pin_AwaitCommandFromRPi) == 1 && digitalRead(GPIO_Pin_ProduceSnowCommandFromRpi) == 1) {  
      timingMode = 5;
      RPiMode ="GO! GO!";     
    } 
    
      Serial.println(RPiMode);
    } else{//not Rpi control
       if (RPiStatus == 1) {// but if it was 
       RPiStatus =0; //reset it and enter Interval mode
         timingMode = 1;
       } //0=Normal; 1 = Rpi in control

    }
//Button detection goes here
int binaryValue = 0; 
  int button1State = digitalRead(B1);
  int button2State = digitalRead(B2);
  int button3State = digitalRead(B3);
  int button4State = digitalRead(B4);
  int button5State = digitalRead(B5);
  int button6State = digitalRead(B6);
  int button7State = digitalRead(B7);
  int button8State = digitalRead(B8);
  int button9State = digitalRead(B9);
  int button10State = digitalRead(B10);
  int button11State = digitalRead(B11);
  int button12State = digitalRead(B12);
 // int binaryValue1 = (button12State <<11) | (button11State << 10) | (button10State << 9) | (button9State << 8) | (button8State << 7) | (button7State << 6) | (button6State << 5) | (button5State << 4) | (button4State << 3) | (button3State << 2) | (button2State << 1) | (button1State ) ;
 
  int binaryValue1 = (button3State << 2) | (button2State << 1) | (button1State ) ;
  //Second read
  delay(25);
 
   button1State = digitalRead(B1);
   button2State = digitalRead(B2);
   button3State = digitalRead(B3);
   button4State = digitalRead(B4);
   button5State = digitalRead(B5);
   button6State = digitalRead(B6);
   button7State = digitalRead(B7);
   button8State = digitalRead(B8);
   button9State = digitalRead(B9);
   button10State = digitalRead(B10);
   button11State = digitalRead(B11);
   button12State = digitalRead(B12);
  //int binaryValue2 = (button12State <<11) | (button11State << 10) | (button10State << 9) | (button9State << 8) | (button8State << 7) | (button7State << 6) | (button6State << 5) | (button5State << 4) | (button4State << 3) | (button3State << 2) | (button2State << 1) | (button1State ) ;
 int binaryValue2 = (button3State << 2) | (button2State << 1) | (button1State ) ;
    //Compare reads
if(binaryValue1 == binaryValue2){
  binaryValue = binaryValue1; 
}
  Serial.print("Binary value: "); 
  Serial.print(binaryValue, BIN);  Serial.print("  :  "); // Print binary representation
  Serial.println(binaryValue,DEC); // Print binary representation
data[7]=binaryValue;
PushButtonMode=binaryValue;



switch (PushButtonMode) {
  case 0:  
PushButtonCount =0;
break;

case 1:
//SNOW
if (PushButtonCount == 0)
processTimeButton();
PushButtonCount =1;
break;

case 2:
TouchButtonFlowUp();
PushButtonCount =0;
break;

case 3:
PushButtonCount =0;
TouchButtonFlowDown();
break;

case 4:
Air = Air + 20;
TouchButtonAirUp();
PushButtonCount =0;
break;

case 5:
Air = Air -20;
PushButtonCount =0;
TouchButtonAirDown();
break;

case 6:
if (PushButtonCount == 0)
printWifiStatus();
PushButtonCount =1;
break;

case 7:
if (PushButtonCount == 0) {
if (MaxAir == 100){
MaxAir =255;
MaxFlow =255;
}else{
MaxAir =100;
MaxFlow =10;
if (Air >= MaxAir){   Air = MaxAir;   }
if (Flow >= MaxFlow){ Flow = MaxFlow;}
SendXML();
}
PushButtonCount =1;
}

break;

}
 


    switch (timingMode) {

      case 0:
        data[1] = 0;  //Airflow
        data[2] = 0;  //Fliud Flow
        break;

      case 1:
        //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration;
        IntervalLeft = IntervalLeft - 1000;
        if (IntervalLeft <= 0) {
          IntervalLeft = 0;
          timingMode = 2;  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration;
          DurationLeft = Duration;
        }
        data[1] = 0;  //Airflow
        data[2] = 0;  //Fliud Flow
        data[3] = ButtonMode;
        data[4] = timingMode;
        data[5] = CH_Number;
        data[6] = CH_Value;
        break;

      case 2:  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration;
        DurationLeft = DurationLeft - 1000;
        if (DurationLeft <= 0) {
          DurationLeft = 0;
          timingMode = 1;  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration;
          IntervalLeft = Interval;
        }
        data[1] = Flow;  //Airflow
        data[2] = Air;   //Fliud Flow
        data[3] = ButtonMode;
        data[4] = timingMode;
        data[5] = CH_Number;
        data[6] = CH_Value;
        break;
      case 3:            //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration;
        data[1] = Flow;  //Airflow
        data[2] = Air;   //Fliud Flow

        break;

      case 4:  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration; 3=Resume; 4=RPi mode
        if (digitalRead(GPIO_Pin_ProduceSnowCommandFromRpi) == 1) {
          timingMode = 5;
        } else {
          data[1] = 0;  //Airflow
          data[2] = 0;  //Fliud Flow
          IntervalLeft = IntervalStart;
        }
        break;

      case 5:  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence
     
       
          data[1] = Flow;  //Airflow
          data[2] = Air;   //Fliud Flow
        
        break;
    }

   
    dmx_write(dmxPort, data, DMX_PACKET_SIZE);
    
    /* Now we can transmit the DMX packet! */
    dmx_send(dmxPort);
    Serial.print("dmxPort.. ");
    Serial.println(dmxPort);
Serial.print(data[1]);Serial.print("..");Serial.print(data[2]);Serial.print("..");Serial.print(data[3]);Serial.print("..");Serial.print(data[4]);Serial.print("..");Serial.print(data[5]);Serial.print("..");Serial.println(data[6]);
    /* We can do some other work here if we want. */

    /* If we have no more work to do, we will wait until we are done sending our
    DMX packet. */
    dmx_wait_sent(dmxPort, DMX_TIMEOUT_TICK);

    digitalWrite(2, !digitalRead(2));

    //Send data to the OLED display
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.setCursor(0, 0);
    display.print("I");
    display.setCursor(20, 0);
    display.print(Interval / 1000);
    
    display.setCursor(90, 0);
    display.print(IntervalLeft / 1000);

    display.setCursor(0, 16);
    display.print("D");
    display.setCursor(20, 16);
    display.print(Duration / 1000);
    display.setCursor(90, 16);
    display.print(DurationLeft / 1000);

    if (CH_Number == 2) {
      display.setCursor(7, 34);
      display.setTextSize(1);
      display.print("a");
      display.setCursor(15, 34);
      display.print(Air);


      display.setCursor(65, 32);
      display.setTextSize(2);
      display.print("F");
      display.setCursor(80, 32);
      display.print(Flow);

    } else {
      display.setCursor(0, 32);
      display.setTextSize(2);
      display.print("A");
      display.setCursor(15, 32);
      display.print(Air);
      display.setCursor(72, 34);
      display.setTextSize(1);
      display.print("f");
      display.setCursor(80, 34);
      display.print(Flow);
    }


    display.setTextSize(2);

    display.setCursor(0, 48);
    if (timingMode == 0) {
      display.print("Stop! ");
      digitalWrite(GPIO_Pin_Snow, 0);//Make 0?
    }
    if (timingMode == 1) {
      display.print("Wait  ");
      digitalWrite(GPIO_Pin_Snow, 0);//Make 0?
    }
    if (timingMode == 2) {
      display.print("Snow! ");
      digitalWrite(GPIO_Pin_Snow, 1);//Make 1?
    }

    if (timingMode == 3) {
      display.print("SNOW! ");
      display.setCursor(90, 0);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      display.setCursor(90, 16);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      digitalWrite(GPIO_Pin_Snow, 1);//Make 1?
    }
    if (timingMode == 4) {
      display.print("Primed");
      display.setCursor(90, 0);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      display.setCursor(90, 16);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      digitalWrite(GPIO_Pin_Snow, 0);//Make 0?
    }
    if (timingMode == 5) {
      display.print("GO!GO!");
      display.setCursor(90, 0);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      display.setCursor(90, 16);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      digitalWrite(GPIO_Pin_Snow, 0);//Make 0?
    }


// Display additional data
//PushButtonMode
      display.setCursor(82, 48);
      display.setTextSize(1);
      display.print(PushButtonMode, HEX);

//Air and Flow values
      display.setCursor(100, 48);
      display.setTextSize(1);
      display.print(data[2], HEX);
      display.setCursor(118, 48);
      display.print(data[1], HEX);

//display version number
       display.setCursor(60, 57);
      display.setTextSize(1);
  display.print("V");
  display.print(version);
    display.display();
    display.setCursor(82, 57);
display.print(WiFi.SSID()); //SSID host name

  //If AP active  
if (BeamZActive == true){
    display.drawLine(0, 0, display.width()-1, 0, WHITE);
        }
// If Large range is active
if (MaxAir >= 101){
    display.drawLine(0, 63, display.width()-1, 63, WHITE);
}
    display.display();
    SendXML();
  }
}


void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void SendWebsite() {
  Serial.print("SendWebsite  : ");
  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  if (MaxAir>= 101){
     server.send(200, "text/html", HIGH_VALUES);
  }else{
  server.send(200, "text/html", LOW_VALUES );
  }
}



// I think I got this code from the wifi example
void printWifiStatus() {

  long rssi = WiFi.RSSI();
  if (rssi <= -1) {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    ip = WiFi.localIP();
    Serial.print("localIP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    // print where to go in a browser:
    Serial.print("Open http://");
    Serial.println(ip);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Access");
    display.setTextSize(1);
    display.println("");
    display.println(WiFi.SSID());
    display.println("");
    display.println(password);
    display.println("");
    display.println(ip);
    // print where to go in a browser:
    display.display();
  } else {

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Access");
    display.setTextSize(1);
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    display.println("\nSSID: BeamZ");
    display.println("\nP/W: ");
    display.println("\n192.168.1.1");

    display.display();
  }
  
    delay(2000);
}

void UpdateIntervalSlider() {
  // mainly, I hate strings, but wifi lib uses them...
  String t_state = server.arg("VALUE");  //The value sent to the ESP by the webpage in this case the value of the slider

  // convert the string sent from the web page to an int
  Interval = t_state.toInt();
  strcpy(buf, "");
  sprintf(buf, "%d", Interval);  //The value to send back to the Webpage
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
  Interval = Interval * 1000;
}

void UpdateDurationSlider() {
  // mainly, I hate strings, but wifi lib uses them...
  String t_state = server.arg("VALUE");  //The value sent to the ESP by the webpage in this case the value of the slider

  // convert the string sent from the web page to an int
  Duration = t_state.toInt();
  strcpy(buf, "");
  sprintf(buf, "%d", Duration);  //The value to send back to the Webpage
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
  Duration = Duration * 1000;
}

void UpdateAirSlider() {
  // mainly, I hate strings, but wifi lib uses them...
  String t_state = server.arg("VALUE");  //The value sent to the ESP by the webpage in this case the value of the slider
  // convert the string sent from the web page to an int
  Air = t_state.toInt();
  if (Air >= MaxAir) {
    Air = MaxAir;
  }  
/*if ( inRange(Air, 75, 99) )
{
 Air = 75;
}  
if ( inRange(Air, 50, 74) )
{
 Air = 50;
}  

if ( inRange(Air, 25, 49) )
{
 Air = 26;
}  

  */
  if (Air <= 24) {
    Air = 0;
  }
  CH_Number = 1;
  strcpy(buf, "");
 // sprintf(buf, "%d", Air - 1);  //The value to send back to the Webpage
  sprintf(buf, "%d", Air );  //The value to send back to the Webpage
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
}

void UpdateFlowSlider() {
  // mainly, I hate strings, but wifi lib uses them...
  String t_state = server.arg("VALUE");  //The value sent to the ESP by the webpage in this case the value of the slider
  // convert the string sent from the web page to an int
  Flow = t_state.toInt();
  
  if (Flow >= MaxFlow) {
    Flow = MaxFlow;
  }  
  
  if (Flow < 0) {
    Flow = 0;
  }  
  CH_Number = 2;
  strcpy(buf, "");
  sprintf(buf, "%d", Flow);  //The value to send back to the Webpage
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
}

void SendXML() {

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  // send  Interval Value
  sprintf(buf, "<IM>%d</IM>\n", (int)(Interval / 1000));
  strcat(XML, buf);
  // send  Interval Remain Value
  sprintf(buf, "<IR>%d</IR>\n", (int)(IntervalLeft / 1000));
  strcat(XML, buf);
  // send  Duration Value
  sprintf(buf, "<DM>%d</DM>\n", (int)(Duration / 1000));
  strcat(XML, buf);
  // send  DurationLeft Value
  sprintf(buf, "<DR>%d</DR>\n", (int)(DurationLeft / 1000));
  strcat(XML, buf);
  // send Air FLow Value
  sprintf(buf, "<AF>%d</AF>\n", Air);
  strcat(XML, buf);
  // send Fluid FLow Value
  sprintf(buf, "<FF>%d</FF>\n", Flow);
  strcat(XML, buf);
  // send timingMode  Value
  sprintf(buf, "<TM>%d</TM>\n", timingMode);  // }
  strcat(XML, buf);

  // send version  Value
  sprintf(buf, "<V1>%d</V1>\n", version);  // }
  strcat(XML, buf);
  // send ManTime  Value
  sprintf(buf, "<MT>%d</MT>\n", ManTime);  // }
  strcat(XML, buf);

  strcat(XML, "</Data>\n");
  // wanna see what the XML code looks like?
  // actually print it to the serial monitor and use some text editor to get the size
  // then pad and adjust char XML[2048]; above
  // Serial.println(XML);

  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(500, "text/xml", XML);
}
// now process button_0 press from the web site. Typical applications are the used on the web client can
// turn on / off a light, a fan, disable something etc
void processStopButton() {
  //delay(200);
  switch (timingMode) {

    case 0:
      timingMode = OldtimingMode;
      break;

    case 1:
      OldtimingMode = timingMode;
      timingMode = 0;
      break;

    case 2:
      OldtimingMode = timingMode;
      timingMode = 0;
      break;

    case 3:
      OldtimingMode = 2;
      timingMode = 0;
      break;
  }
  strcpy(buf, "");
  sprintf(buf, "%d", timingMode);
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
}


void processSkipButton() {
  switch (timingMode) {
    case 0:

      break;
    case 1:
      timingMode = 2;
      DurationLeft = Duration;
      IntervalLeft = 0;
      break;
    case 2:
      timingMode = 1;
      IntervalLeft = Interval;
      DurationLeft = 0;
      break;
    case 3:
      break;
  }
  strcpy(buf, "");
  sprintf(buf, "%d", timingMode);
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
}


void processTimeButton() {
  switch (timingMode) {
    case 0:
      timingMode = 3;
      break;
    case 1:
      timingMode = 3;
      break;
    case 2:
      timingMode = 3;
      break;
    case 3:
      IntervalLeft = 10000;
      DurationLeft = 0;
      timingMode = 1;
      break;
  }
  strcpy(buf, "");
  sprintf(buf, "%d", ManTime);
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
}


void handleBlow() {
  Serial.println("handleBlow");
 if (MaxAir == 100){
MaxAir =255;
MaxFlow =255;
}else{
MaxAir =100;
MaxFlow =10;
if (Air >= MaxAir){   Air = MaxAir;   }
if (Flow >= MaxFlow){ Flow = MaxFlow;}
if (MaxAir>= 101){
     server.send(200, "text/html", HIGH_VALUES);
  }else{
  server.send(200, "text/html", LOW_VALUES);
  }
  //SendWebsite();
  server.sendHeader("Location", "/", true);
  server.send(200, "text/plain", "");
}
}


void handleRJM() {
  Serial.println("handleRJM");
  version = 0;
  strcpy(buf, "");
  sprintf(buf, "%d", !Page);
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
}





void processIPCredentials() {
  //digitalWrite(2, !digitalRead(2));
  Serial.println("IP received");
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Received");
  display.println(ssid);
  display.println(password);
  display.println("Received");
  display.display();

  //delay(900);
  if (server.hasArg("ssid") && server.hasArg("password")) {
    // Save the data to the EEPROM here
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Saving WiFI");
    display.println(ssid);
    display.println(password);
    display.display();
    Serial.println("Saving WiFI");
    Serial.println(ssid);
    Serial.println(password);
    //delay(2000);


    EEPROM.begin(512);
    EEPROM.write(0, ssid.length());
    for (int i = 0; i < ssid.length(); i++) {
      EEPROM.write(i + 1, ssid[i]);
    }
    EEPROM.write(ssid.length() + 1, password.length());
    for (int i = 0; i < password.length(); i++) {
      EEPROM.write(i + ssid.length() + 2, password[i]);
    }
    EEPROM.commit();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("SAVED!");
    display.display();
    //delay(1100);

    TouchButtonReset();
    ;  //reboot the ESP32

  } else {
    // If the SSID, password, or IP address is missing, show the form again
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("No Data!");
    display.display();
    //delay(500);
  }
  if (MaxAir>= 101){
     server.send(200, "text/html", HIGH_VALUES);
  }else{
  server.send(200, "text/html", LOW_VALUES);
  }
  server.sendHeader("Location", "/", true);
  server.send(200, "text/plain", "");
}

void TouchButtonAirUp() {  //Air value up
  CH_Number = 1;
  CH_ValueUPISR();
}

void TouchButtonAirDown() {  //Air value Down
  CH_Number = 1;
  CH_ValueDownISR();
}
void TouchButtonFlowUp() {  //Flow value up
  CH_Number = 2;
  CH_ValueUPISR();
}

void TouchButtonFlowDown() {  //Flow value Down
  CH_Number = 2;
  CH_ValueDownISR();
}

void TouchButtonIDown() {  //Flow value Down
  IntervalDownISR();
}

void TouchButtonIUp() {  //Flow value Down
  IntervalUpISR();
}

void TouchButtonDDown() {  //Flow value Down
  DurationDownISR();
}

void TouchButtonDUp() {  //Flow value Down
  DurationUpISR();
}

void TouchButtonManual() {
  processTimeButton();
}

void TouchButtonSkip() {
  processSkipButton();
}

void TouchButtonStop() {
  processStopButton();
}

void TouchButtonReset() {
  for (uint8_t i = 3; i > 0; i--) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Reset In");
    display.print(i);
    display.print(" Seconds");
    // digitalWrite(2, !digitalRead(2));
    display.display();
    //delay(1000);
  }
  display.clearDisplay();
  display.display();
  delay(100);
  ESP.restart();
}


void CH_ValueUPISR() {  //Set Value of the channel 0-255
  ButtonTime = millis();
  if (ButtonTime - LastButtonTime >= 500) {
    switch (CH_Number) {
      case 1:

        Air = Air + 5;
 
       if (Air >= MaxAir) {
          Air = MaxAir;
        }
 
        break;
      case 2:
        Flow = Flow + 1;
        if (Flow >= MaxFlow) {
          Flow = MaxFlow;
        }
    }
    LastButtonTime = ButtonTime;
  }
  change = 1;
}

void CH_ValueDownISR() {
  ButtonTime = millis();
  if (ButtonTime - LastButtonTime >= 500) {
    switch (CH_Number) {
      case 1:
  Air = Air - 5;
       if (Air <= 4) {
          Air = 0;}

/*      
          if (Air == 25) {
          Air = 26;}*/
          break;
        

      case 2:
        Flow = Flow - 1;
        if (Flow < 0) {
          Flow = 0;
        }
        break;
    }
    LastButtonTime = ButtonTime;
  }
  change = 1;
}
void blink(uint8_t n){
  
  Serial.println("Blinking");
  Serial.println( n);
    for (uint8_t i = 0; i >=5; i++){
  //delay(500);

  //digitalWrite(2, false);
  
  //delay(500);

  //digitalWrite(2, true);
  }
}



void DurationUpISR() {  //Adjust Duration time by 10 seconds
  ButtonTime = millis();
  if (ButtonTime - LastButtonTime >= 500) {
    Duration = Duration + 10000;
    if (Duration >= 60000) {
      Duration = 60000;
    }
    DurationLeft = Duration;
    LastButtonTime = ButtonTime;
  }
  change = 1;
}
void IntervalUpISR() {
  ButtonTime = millis();
  if (ButtonTime - LastButtonTime >= 500) {
    Interval = Interval + 10000;
    if (Interval >= 180000) {
      Interval = 180000;
    }
    IntervalLeft = Interval;
    LastButtonTime = ButtonTime;
  }
  change = 1;
}
void DurationDownISR() {
  ButtonTime = millis();
  if (ButtonTime - LastButtonTime >= 500) {
    Duration = Duration - 10000;
    if (Duration <= 0) {
      Duration = 0;
    }
   
    LastButtonTime = ButtonTime;
  }
  change = 1;
}
void IntervalDownISR() {
  ButtonTime = millis();
  if (ButtonTime - LastButtonTime >= 500) {
    Interval = Interval - 10000;
    if (Interval <= 0) {
      Interval = 0;
    }
    LastButtonTime = ButtonTime;
  }
  change = 1;
}

void CH_SelectISR() {
  ButtonTime = millis();
  if (ButtonTime - LastButtonTime >= 500) {
    ButtonMode = !ButtonMode;
    if (CH_Number == 1) {
      CH_Number = 2;
    } else {
      CH_Number = 1;
    }
    LastButtonTime = ButtonTime;
  }
  change = 1;
}


 void handleControl(){
//KeypadActive = !KeypadActive;
  strcpy(buf, "");
  sprintf(buf, "%d", ip[3]);
  sprintf(buf, buf);

  Serial.println(ip[3]);
  Serial.print(ip[0]);
  Serial.print(".");
  Serial.print(ip[1]);
  Serial.print(".");
  Serial.print(ip[2]);
  Serial.print(".");
  Serial.println(ip[3]);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
 }
 
void handleReset() {
  TouchButtonReset();
}
 

