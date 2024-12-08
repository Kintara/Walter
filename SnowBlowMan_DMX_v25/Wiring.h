Maglightbox
ESP32
VIN >> MAX485 VCC
GND >> MAX485 GND
D7 (TX2) >>   MAX485 DI
3.3v >> MRP121 VCC >>  OLED VCC
GND  >> MRP121 GND >> OLED GND
GPIO 21 (SDA)>> MPR121 SDA >> OLED SDA
GPIO 22 (SCL)>> MPR121 SLC >> OLED SCK
 MAX485 GND >> RJ45 7 & 8
 MAX485 A >> RJ45 1
 MAX485 B >> RJ45 2


Development
ESP  Ribbon cable

//D25  R0    Blue    Flow Up      
//D26  G0    Green   Flow Down
3.3  B0    Yellow  VCC 3.3v
GND  GND   Orange
D21  R1    Red     SDA
D22  G1    Brown   SCK
//D23  B1    Black   N/C  IRQ
//GND  GND   White
D12  A     Grey    To Raspberry Pi Zero 2 W GPIO to start snow message. Active Low
//D6   B     Violet  RX2 N/C
D7   C     Blue    TX2   MAX485 DI
//D18  D     Green   Interval Up
//D15  CLK   Yellow  Interval Down
//D32  LAT   Orange  Duration Up
//D33  OE    Red     Duration Down
//GND  GND   Brown 