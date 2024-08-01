
int version = 23;  //22; //last good version
#include <EEPROM.h>
#include <WiFi.h>        // standard library
#include <WebServer.h>   // standard library
#include "SnowWeb200.h"  // .h file that stores your html page code

#include <Arduino.h>
#include <esp_dmx.h>  //v3.1.0

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Touch Buttons
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

bool inRange(int val, int minimum, int maximum)
{
  return ((minimum <= val) && (val <= maximum));
}


//++ ++ ++ ++ ++ ++ ++ ++ ++
// here you post web pages to your homes intranet which will make page debugging easier
// as you just need to refresh the browser as opposed to reconnection to the web server
//#define USE_INTRANET

// replace this with your homes intranet connect parameters
#define LOCAL_SSID "Plume"
#define LOCAL_PASS "Allthedogs"

// once  you are ready to go live these settings are what your client will connect to
#define AP_SSID "Walter"
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

//define the pins for the buttons
//define the pins to send signals to the Rpi Zero2 W
#define PIN_LED 2
#define GPIO_Pin_Snow 12  //To Rpi Zero2 W to trigger Snowing sequence > CAT pin 3 Active Hi
#define GPIO_Pin_AwaitCommandFromRPi 13        //From Rpi Zero2 W to stop the blower and await Snow Command from RPi  > CAT pin 4 Active Hi
#define GPIO_Pin_ProduceSnowCommandFromRpi 14  //From Rpi Zero2 W : Start Snow! command > CAT pin 5 Active Hi
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
int CH_Value = 0;
int timingMode = 1;  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence

String RPiMode = "";
bool OldTime = true;
int OldtimingMode = 0;  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence
int ManTime = 1;
/* Now we want somewhere to store our DMX data. Since a single packet of DMX
  data can be up to 513 bytes long, we want our array to be at least that long.
  This library knows that the max DMX packet size is 513, so we can fill in the
  array size with `DMX_Buffer */

byte data[DMX_PACKET_SIZE];

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
int enablePin = 23;
/* Make sure to double-check that these pins are compatible with your ESP32!
  Some ESP32s, such as the ESP32-WROVER series, do not allow you to read or
  write data on pins 16 or 17, so it's always good to read the manuals. */

/* Next, lets decide which DMX port to use. The ESP32 has either 2 or 3 ports.
  Port 0 is typically used to transmit serial data back to your Serial Monitor,
  so we shouldn't use that port. Lets use port 1! */
dmx_port_t dmxPort = 1;
int DMX_Buffer_Size = 10;


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
  digitalWrite(2, true);
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(200);
  
  digitalWrite(2, false);
  delay(100);
  digitalWrite(2, true);
  // OLED Screen test
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.setRotation(2);

  display.clearDisplay();

  //display RJM LOGO
  display.drawBitmap(0, 0, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(500);
  display.startscrollleft(0x00, 0x0f);
  delay(200);
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
  delay(500);
  
  digitalWrite(2, false);
  // Clear the buffer
  display.clearDisplay();


  pinMode(12, OUTPUT);
  pinMode(GPIO_Pin_AwaitCommandFromRPi, INPUT_PULLDOWN);
  pinMode(GPIO_Pin_ProduceSnowCommandFromRpi, INPUT_PULLDOWN);
 
  //Touch Buttons
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1)
      ;
  }
  Serial.println("MPR121 found!");
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
  delay(200);
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
    display.println("Walter : ");
    display.println("IP 192,168,1,1");
    WiFi.softAP("Walter", "");
    Serial.println("AP Mode Walter &  IP=192.168.1.1");
    delay(200);
    WiFi.softAPConfig(PageIP, gateway, subnet);
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
      delay(500);
    }
    display.display();
    delay(200);
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
      delay(500);
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
      WiFi.softAP("Walter", "");
      Serial.println("AP Mode Walter &  IP=192.168.1.1");
      WiFi.softAPConfig(PageIP, gateway, subnet);
      ip_address = "192.168.1.1";
      ssid = "Walter";
      password = "";
      display.display();
      delay(2000);
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Setting Up");
      display.println("\nWalter");
      display.println("\n1234");
      display.println("\n192.168.1.1");
      display.display();
      display.setTextSize(1);
      delay(500);
    }
  }

  digitalWrite(2, false);

  delay(1000);

  digitalWrite(2, true);
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_driver_install(dmxPort, &config, DMX_INTR_FLAGS_DEFAULT);

  /* Now set the DMX hardware pins to the pins that we want to use and setup
    will be complete! */
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);


  for (int x; 0; 255) {
    data[x] = 0;
  }
  data[1] = 0;  //Air
  data[2] = 0;  //Flow
                // if your web page or XML are large, you may not get a call back from the web page
  // and the ESP will think something has locked up and reboot the ESP
  // not sure I like this feature, actually I kinda hate it
  // disable watch dog timer 0
  disableCore0WDT();

  // maybe disable watch dog timer 1 if needed
  //  disableCore1WDT();

  // just an update to progress
  Serial.println("starting server");

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
  delay(5000);

  digitalWrite(2, false);
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
  server.on("/ProcessRJM", handleRJM);
  server.on("/ProcessReset", handleReset);
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
      digitalWrite(2, true);
      change = 3;
      break;
    case 3:
      if (now - LEDupdate >= 500) {
        change = 0;
      }
      break;
  }
  currtouched = cap.touched();
  ButtonPad = 255;
  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      ButtonPad = i;
    }
  }
  switch (ButtonPad) {
    case 6:;  //Air Up
      change = 1;
      TouchButtonAirUp();

      break;
    case 10:
      //Air Down
      change = 1;
      TouchButtonAirDown();
      break;
    case 7:
      //Flow Up
      change = 1;
      TouchButtonFlowUp();
      break;
    case 11:
      //Flow Down
      change = 1;
      TouchButtonFlowDown();
      break;
    case 8:
      //I Down
      change = 1;
      TouchButtonIDown();
      break;
    case 4:
      // I Up
      change = 1;
      TouchButtonIUp();
      break;
    case 9:
      // D Down
      change = 1;
      TouchButtonDDown();
      break;
    case 5:
      // D Up
      change = 1;
      TouchButtonDUp();
      break;
    case 0:
      // Skip
      change = 1;
      TouchButtonSkip();
      break;
    case 1:
      // Stop
      change = 1;
      TouchButtonStop();
      break;
    case 2:
      // Manual
      change = 1;
      processTimeButton();
      break;
    case 3:
      //  Re-setting
printWifiStatus();

      delay(2000);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Touch I-  To Reset");
      display.println("Any Other to Cancel");
      display.display();
      delay(500);
      currtouched = 0;
      while (currtouched == 0) {
        currtouched = cap.touched();
      }
      ButtonPad = 255;
      for (uint8_t i = 0; i < 12; i++) {
        // it if *is* touched

        if ((currtouched & _BV(i))) {
          ButtonPad = i;
        }
      }
      switch (ButtonPad) {
        case 8:

          TouchButtonReset();
          break;
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
      digitalWrite(PIN_LED, true);          
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
    dmx_send(dmxPort, DMX_PACKET_SIZE);

    /* We can do some other work here if we want. */

    /* If we have no more work to do, we will wait until we are done sending our
    DMX packet. */
    dmx_wait_sent(dmxPort, DMX_TIMEOUT_TICK);


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
      display.print("%");


      display.setCursor(65, 32);
      display.setTextSize(2);
      display.print("F");
      display.setCursor(80, 32);
      display.print(Flow);
      display.print("%");

    } else {
      display.setCursor(0, 32);
      display.setTextSize(2);
      display.print("A");
      display.setCursor(15, 32);
      display.print(Air);
      display.print("%");
      display.setCursor(72, 34);
      display.setTextSize(1);
      display.print("f");
      display.setCursor(80, 34);
      display.print(Flow);
      display.print("%");
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
      display.print("GO! GO!");
      display.setCursor(90, 0);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      display.setCursor(90, 16);
      display.setTextColor(WHITE, BLACK);
      display.print("X ");
      digitalWrite(GPIO_Pin_Snow, 0);//Make 0?
    }

    if (timingMode == 0) {  //0=Stop 1=interval 2= Duration; 3=Resume; 4=RPi mode Primed; 5=Go from Sequence2= Duration;
      display.setCursor(90, 48);
      display.setTextSize(1);
      display.print(0, HEX);
      display.setCursor(112, 48);
      display.print(0, HEX);
    } else {
      display.setCursor(90, 48);
      display.setTextSize(1);
      display.print(data[2], HEX);
      display.setCursor(112, 48);
      display.print(data[1], HEX);
    }
    display.setCursor(90, 57);
  display.print("V");
  display.print(version);
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
  server.send(200, "text/html", PAGE_MAIN);
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
    display.println("\nSSID: Walter");
    display.println("\nP/W: ");
    display.println("\n192.168.1.1");

    display.display();
    delay(3000);
  }
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
  if (Air >= 100) {
    Air = 100;
  }  
if ( inRange(Air, 75, 99) )
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
  if (Air <= 24) {
    Air = 0;
  }
  CH_Number = 1;
  strcpy(buf, "");
  sprintf(buf, "%d", Air - 1);  //The value to send back to the Webpage
  sprintf(buf, buf);
  // now send it back
  server.send(200, "text/plain", buf);  //Send web page
}

void UpdateFlowSlider() {
  // mainly, I hate strings, but wifi lib uses them...
  String t_state = server.arg("VALUE");  //The value sent to the ESP by the webpage in this case the value of the slider
  // convert the string sent from the web page to an int
  Flow = t_state.toInt();
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
  delay(200);
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

  delay(900);
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
    delay(2000);


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
    delay(1100);

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
    delay(500);
  }

  server.send(200, "text/html", PAGE_MAIN);
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
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
    delay(1000);
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

        Air = Air + 25;
        
if ( inRange(Air, 25, 49) )
{
 Air = 26;
}  
if ( inRange(Air, 50, 74) )
{
 Air = 50;
}  
if ( inRange(Air, 75, 99) )
{
 Air = 75;
} 
       if (Air >= 101) {
          Air = 100;
        }
 
        break;
      case 2:
        Flow = Flow + 1;
        if (Flow >= 11) {
          Flow = 10;
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
  Air = Air - 25;
       if (Air <= 24) {
          Air = 0;}
if ( inRange(Air, 25, 49) )
{
 Air = 26;
} 
if ( inRange(Air, 50, 74) )
{
 Air = 50;
}
 if ( inRange(Air, 75, 99) )
{
 Air = 75;
} 
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

void handleReset() {
  TouchButtonReset();
}


