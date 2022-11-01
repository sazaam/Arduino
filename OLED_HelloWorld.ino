// https://github.com/olikraus/u8g2
// https://github.com/olikraus/u8g2/wiki/u8g2reference
// arduino library manager -> u8g2
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int xPos = 0;
int yPos = 0;

unsigned long curr_time = 0;
unsigned long prev_time = 0;
int Tms = 200;  // ms period

void setup(void) {
  u8g2.begin();  
  u8g2.setFont(u8g2_font_amstrad_cpc_extended_8r );
  Serial.begin(9600);
}

void loop(void) {
  curr_time = millis();
  if (curr_time - prev_time > Tms) {
    u8g2.firstPage();
    do {
      /* Divide 128 x 64 to 8 pixels. Thus, 16 x 8 grid cells*/
      u8g2.drawStr(xPos, yPos + 8, "IT Project2!");
      u8g2.drawStr(xPos, yPos + 16, "Let's make");
      u8g2.drawStr(xPos, yPos + 24, "something fun!");
    } while ( u8g2.nextPage() );
    
    // Tms must be larger than the display time
    Serial.println((String)"Display Time = " + (millis() - curr_time));
    if (xPos < 128) {    
      xPos += 4;
      yPos += 2;
    } else {
      xPos = 0;
      yPos = 0;
    }
    prev_time = curr_time;
  }  
}
