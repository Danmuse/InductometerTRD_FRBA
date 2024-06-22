/*
 ST7789 240x240 IPS (without CS pin) connections (only 6 wires required):

 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> D13/SCK
 #04 SDA -> D11/MOSI
 #05 RES -> D8 or any digital
 #06 DC  -> D7 or any digital
 #07 BLK -> NC
*/

#include "utils.h"

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

const uint16_t imgF[] PROGMEM = {0xF800,0xF840,0xF8A0,0xF900,0xF960,0xF9C0,0xFA20,0xFA80,0xFAE0,0xFB40,0xFBA0,0xFC00,0xFC60,0xFCC0,0xFD20,0xFD80,0xFDE0,0xFE40,0xFEA0,0xFF00,0xFF60,0xFFC0,0xFFE0,0xEFE0,0xE7E0,0xD7E0,0xCFE0,0xBFE0,0xB7E0,0xA7E0,0x9FE0,0x8FE0,0x87E0,0x77E0,0x6FE0,0x5FE0,0x57E0,0x47E0,0x3FE0,0x2FE0,0x27E0,0x17E0,0xFE0,0x7E0,0x7E1,0x7E3,0x7E4,0x7E6,0x7E7,0x7E9,0x7EA,0x7EC,0x7ED,0x7EF,0x7F0,0x7F2,0x7F3,0x7F5,0x7F6,0x7F8,0x7F9,0x7FB,0x7FC,0x7FE,0x7FF,0x79F,0x73F,0x6DF,0x67F,0x61F,0x5BF,0x55F,0x4FF,0x49F,0x43F,0x3DF,0x37F,0x31F,0x2BF,0x25F,0x1FF,0x19F,0x13F,0xDF,0x7F,0x1F,0x81F,0x101F,0x201F,0x281F,0x381F,0x401F,0x501F,0x581F,0x681F,0x701F,0x801F,0x881F,0x981F,0xA01F,0xB01F,0xB81F,0xC81F,0xD01F,0xE01F,0xE81F,0xF81F,0xF81F,0xF81D,0xF81C,0xF81A,0xF819,0xF817,0xF816,0xF814,0xF813,0xF811,0xF810,0xF80E,0xF80D,0xF80B,0xF80A,0xF808,0xF807,0xF805,0xF804,0xF802,0xF801,
                                 0xF800,0xF840,0xF8A0,0xF900,0xF960,0xF9C0,0xFA20,0xFA80,0xFAE0,0xFB40,0xFBA0,0xFC00,0xFC60,0xFCC0,0xFD20,0xFD80,0xFDE0,0xFE40,0xFEA0,0xFF00,0xFF60,0xFFC0,0xFFE0,0xEFE0,0xE7E0,0xD7E0,0xCFE0,0xBFE0,0xB7E0,0xA7E0,0x9FE0,0x8FE0,0x87E0,0x77E0,0x6FE0,0x5FE0,0x57E0,0x47E0,0x3FE0,0x2FE0,0x27E0,0x17E0,0xFE0,0x7E0,0x7E1,0x7E3,0x7E4,0x7E6,0x7E7,0x7E9,0x7EA,0x7EC,0x7ED,0x7EF,0x7F0,0x7F2,0x7F3,0x7F5,0x7F6,0x7F8,0x7F9,0x7FB,0x7FC,0x7FE,0x7FF,0x79F,0x73F,0x6DF,0x67F,0x61F,0x5BF,0x55F,0x4FF,0x49F,0x43F,0x3DF,0x37F,0x31F,0x2BF,0x25F,0x1FF,0x19F,0x13F,0xDF,0x7F,0x1F,0x81F,0x101F,0x201F,0x281F,0x381F,0x401F,0x501F,0x581F,0x681F,0x701F,0x801F,0x881F,0x981F,0xA01F,0xB01F,0xB81F,0xC81F,0xD01F,0xE01F,0xE81F,0xF81F,0xF81F,0xF81D,0xF81C,0xF81A,0xF819,0xF817,0xF816,0xF814,0xF813,0xF811,0xF810,0xF80E,0xF80D,0xF80B,0xF80A,0xF808,0xF807,0xF805,0xF804,0xF802,0xF801};

uint16_t img[SCR_WD+16];

static unsigned long DrawImageTest(void) {
  for(int i=0;i<SCR_WD+16;i++) img[i] = tft.rgbWheel(500L*i/SCR_WD);
  unsigned long start = millis();
  for(int i=0;i<5*4;i++) for(int y=0;y<SCR_HT;y++) tft.drawImage(0,y,SCR_WD,1,img+(((y>>2)+i)&0xf));
  return millis()-start;
}

static unsigned long DrawImageFTest(void) {
  unsigned long start = millis();
  /*for(int i=0;i<5*4;i++)*/ for(int y=0;y<SCR_HT;y++) tft.drawImageF(0,y,SCR_WD,1,imgF/*+(((y>>2)+i)&0xf)*/);
  return millis()-start;
}

// original Adafruit
unsigned long orig[14]={5872, 5872, 1468292, 394696, 2938532, 132952, 121996, 4068208, 925276, 1319512, 1278096, 1930524, 602080, 4649200};
// pre 18.09.2020 Fast
//unsigned long orig[14]={2645, 3577, 661616, 114064, 579036, 56216, 49500, 1830864, 222936, 259340, 175460, 903348, 138264, 1853300};

unsigned long res[14];

static void result(int i) {
  Serial.print(res[i]);
  if (res[i]<1000000) Serial.print("\t");
  Serial.print("\t\t\t");
  Serial.print(100*orig[i]/res[i]);
  Serial.println("%");
}

void setup(void) {
  //Serial.begin(9600);
  //Serial.println(F("IPS 240x240 ST7789"));
  
  //tft.reset();
  //tft.init(SCR_WD, SCR_HT);
  tft.begin();
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("IPS 240x240 ST7789");
  tft.println("Library Benchmark");
  tft.println("starts in 3s ...");
  delay(3000);
}

void loop(void) {
//  Serial.println(F("Benchmark                Time (microseconds)"));

  res[0] = FillScreenTest(tft);
//  Serial.print(F("FillScreen Mbps          "));
//  Serial.println(String(res[0])+"ms  "+String(1000*20.0/res[0])+"fps  "+String(20.0*SCR_WD*SCR_HT*16/res[0]/1000.0)+" Mbps\t"+100*orig[0]/res[0]+"%");
//  for(int i=0;i<100;i++) tft.fillRect(i,i,SCR_WD-i*2,SCR_HT-i*2,tft.rgbWheel(i*10)); delay(1000);

  res[1] = DrawImageFTest();
//  Serial.print(F("DrawImageF Mbps          "));
//  Serial.println(String(res[1])+"ms  "+String(1000*20.0/res[1])+"fps  "+String(20.0*SCR_WD*SCR_HT*16/res[1]/1000.0)+" Mbps\t"+100*orig[1]/res[1]+"%"); 
  delay(750);

  res[4] = testLines(tft, CYAN);
//  Serial.print(F("Lines                    "));
//  result(4);
  delay(500);

  res[5] = testFastLines(tft, RED, BLUE);
//  Serial.print(F("Horiz/Vert Lines         "));
//  result(5);
  delay(500);

  res[6] = testRects(tft, GREEN);
//  Serial.print(F("Rectangles (outline)     "));
//  result(6);
  delay(500);

  res[7] = testFilledRects(tft, YELLOW, MAGENTA);
//  Serial.print(F("Rectangles (filled)      "));
//  result(7);
  delay(500);

  res[8] = testFilledCircles(tft, 10, MAGENTA);
//  Serial.print(F("Circles (filled)         "));
//  result(8);

  res[9] = testCircles(tft, 10, WHITE);
//  Serial.print(F("Circles (outline)        "));
//  result(9);
  delay(500);

  res[10] = testTriangles(tft);
//  Serial.print(F("Triangles (outline)      "));
//  result(10);
  delay(500);

  res[11] = testFilledTriangles(tft);
//  Serial.print(F("Triangles (filled)       "));
//  result(11);
  delay(500);

  res[12] = testRoundRects(tft);
//  Serial.print(F("Rounded rects (outline)  "));
//  result(12);
  delay(500);

  res[13] = testFilledRoundRects(tft);
//  Serial.print(F("Rounded rects (filled)   "));
//  result(13);
  delay(500);

//  exit(0);
}
