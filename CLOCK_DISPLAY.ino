#include <TM1637Display.h>

#define CLK 2
#define DIO 3

#define DELAY 500


const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,             // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,     // o
  SEG_C | SEG_E | SEG_G,                             // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,             // e
};

TM1637Display display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  int k;
  uint8_t data[] = {0xFF, 0xFF, 0xFF, 0xFF};
  uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};
  display.setBrightness(0x0F);

  display.setSegments(data); // Expected : 8.8.8.8
  delay(DELAY) ;

  data[0] = display.encodeDigit(0);


  display.clear();
  display.setSegments(data + 2, 2, 2); // Expected : __23
  delay(DELAY) ;

  
  display.clear();
  display.setSegments(data + 2, 2, 1); // Expected : _23_
  delay(DELAY) ;


  display.clear();
  display.setSegments(data + 1, 3, 1); // Expected : _123
  delay(DELAY) ;

  display.showNumberDec(301, false); // Expected : _301
  delay(DELAY) ;

  display.showNumberDec(301, true); // Expected : 0301
  delay(DELAY) ;

  display.showNumberDecEx(301, 0x80, true); // Expect 0.301
  delay(DELAY) ;
  
  display.showNumberDecEx(301, 0x40, true); // Expect 0.301
  delay(DELAY) ;

  display.showNumberDecEx(301, 0x20, true); // Expect 0.301
  delay(DELAY) ;

  display.showNumberDecEx(301, 0x10, true); // Expect 0.301
  delay(DELAY) ;
  display.clear();



  display.showNumberDec(14, false, 2, 1); // Expected : _14_
  delay(DELAY) ;
  display.clear() ;



  display.showNumberDec(4, true, 2, 2); // Expected : __04
  delay(DELAY) ;
  display.showNumberDec(-1, false); // Expected : __-1
  delay(DELAY) ;
  display.showNumberDec(-12); // Expected : _-12
  delay(DELAY) ;
  display.clear() ;


  display.showNumberHexEx(0xabcd); // Expected : abcd
  delay(DELAY) ;

  display.clear() ;

}







