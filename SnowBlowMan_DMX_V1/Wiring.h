

ESP32
3.3v >> Button Common >> OLED VCC                 VIN >> RJ45/6 >> MAX485 VCC
G  >> MAX485 GND >> OLED GND                      G  >> RJ43/7 & 8
15 << B12                                         13 << B11 
2  (INTERNAL LED) "PIN_LED"                       12 << B8
4  >> MAX485 EN  "enablePin"                      14 << B5
16 >> MAX485 RO "receivePin"                      27 << B2
17 >> MAX485 DI "transmitPin"                     26 >> X
5  >> X                                           25 << RJ45/3 "GPIO_Pin_Snow"
18 << B9                                          33 >> RJ45/4 "GPIO_Pin_AwaitCommandFromRPi"
19 << B6                                          32 >> RJ45/5 "GPIO_Pin_ProduceSnowCommandFromRpi"
21 >> OLED SDA                                    35 << B10
3  XXX                                            34 << B7
1  XXX                                            39 << B4
22 >> OLED SCL                                    36 << B1
23 << B3                                          EN XXX


MAX485
VCC << VCC
GND >> RJ45 7 & 8
MAX485 A >> RJ45/1
MAX485 B >> RJ45/2
MODE PINS ESP 4
DI << ESP32 17
RO >> ESP 16

