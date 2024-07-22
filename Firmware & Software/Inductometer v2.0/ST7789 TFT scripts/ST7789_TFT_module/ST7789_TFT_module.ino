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
#include "gradients.h"

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_CS);

uint16_t img[SCR_WD+16];

static unsigned long DrawImageTest(void) {
  for(int i=0;i<SCR_WD+16;i++) img[i] = tft.rgbWheel(500L*i/SCR_WD);
  unsigned long start = millis();
  for(int i=0;i<5*4;i++) for(int y=0;y<SCR_HT;y++) tft.drawImage(0,y,SCR_WD,1,img+(((y>>2)+i)&0xf));
  return millis()-start;
}

static unsigned long DrawImageFTest(void) {
  unsigned long start = millis();
  /*for(int i=0;i<5*4;i++)*/ for(int y=0;y<SCR_HT;y++) tft.drawImageF(0,y,SCR_WD,1,gradientRainbow_ST7789/*+(((y>>2)+i)&0xf)*/);
  return millis()-start;
}

static unsigned long LinearGradientTest(uint8_t coefficientGradient) {
  unsigned long start = millis();
  uint16_t gradientInstance[SCR_WD];
  
  tft.clearScreen();
  if (coefficientGradient > SCR_WD) coefficientGradient = SCR_WD;
  for (uint16_t idx = 0; idx < SCR_WD; idx++) gradientInstance[idx] = gradientHr_ST7789[idx];
  for (uint8_t columnTFT = 0; columnTFT < 12; columnTFT++) {
    for (uint8_t slopeFactor = 0; slopeFactor < 10; slopeFactor++) {
      for (uint8_t idx = 0; idx < SCR_WD / 2; idx++) {
        gradientInstance[(SCR_WD / 2) - 1 + idx] = gradientInstance[(SCR_WD / 2) - 1 + idx + 1];
        gradientInstance[(SCR_WD / 2) - idx] = gradientInstance[(SCR_WD / 2) - idx - 1];
      }
    }
    tft.drawImageF(0, columnTFT, SCR_WD, 1, gradientInstance);
    tft.drawImageF(0, SCR_HT - columnTFT, SCR_WD, 1, gradientInstance);
  }
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

  LinearGradientTest(60);
}

void loop(void) {
  exit(0);
  
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
