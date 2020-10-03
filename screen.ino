#include "bar.h"
#include "line.h"
#include "modern.h"

/////////////////////////
/******* SCREEN  *******/


// prints activity indicator
void printTick() {
  if (percent > 100) percent = 100;
  oled.setFont(runFont);
  oled.setCursor(15, 2);
  oled.print(msgCalib);
  oled.print(percent);
  oled.print("%");
  unsigned int elapsedTime = (millis() - calibStarted) / 1000;
  byte elapsedSeconds = elapsedTime % 60;
  byte elapsedMinutes = elapsedTime / 60;
  oled.setCursor (15, 5);
  oled.print("Run time: ");
  oled.print(elapsedMinutes);
  oled.print(":");
  if (elapsedSeconds < 10)oled.print("0");
  oled.print(elapsedSeconds);

}

void drawBar () {
  oled.setCursor(0, 7);
  oled.setFont(bar);
  for (byte i = 0; i < VOL_MAX_STEP; i++)
    if (i < volume)oled.print ("1");
    else oled.print("2");
}

void drawVolume (byte vol) {
  oled.setCursor(81, 4);
  oled.setFont(bigFont);
  if (vol < 10)oled.print("0");
  oled.print(vol);
  drawBar();
}

#if INPUTCOUNT > 0
void drawInput() {
  oled.setFont(runFont);
  oled.setCursor(33, 0);
  oled.print("          ");
  oled.setCursor(24, 0);
  if (state == STATE_RUN) oled.print(">");
  oled.setCursor(33, 0);
  oled.print(inputName[chan_in]);
  oled.setFont(bigFont);
  oled.setCursor(0, 0);
  oled.print(chan_in + 1);
}
# endif

#if OUTPUTCOUNT > 0
void drawOutput() {
  oled.setCursor(29, 2);
  oled.setFont(runFont);
  oled.print("         ");
  oled.setCursor(102 - (strlen(outputName[chan_out]) + 1) * 7, 2);
  oled.print(outputName[chan_out]);
  oled.setCursor(96, 2);
  if (state == STATE_RUN) oled.print (">");
  //print large output number
  oled.setFont(bigFont);
  oled.setCursor(106, 0);
  oled.print(chan_out + 1);
}
# endif

// Functions for printing two large digits. Works from 00-99
void drawRunDisplay (byte vol) {
  oled.setCursor(0, 5);
  oled.setFont(runFont);
  oled.print("VOLUME:");
  oled.setCursor(28, 2);
  oled.print("           ");
  drawVolume(vol);
#if INPUTCOUNT > 0
  drawInput();
# endif

#if OUTPUTCOUNT > 0
  drawOutput();
# endif
}

void setupMenu () {
  oled.setFont(menuFont);
  for (byte i = 0; i < 4 ; i++) {
    oled.setCursor(0, i * 2);
    if (i == selSetup)oled.print(">");
    else oled.print(" ");
  }
  oled.setCursor(10, 0);
  oled.println("Adjust BIAS");
  oled.setCursor(10, 2);
  oled.println("Measure LDRs");
  oled.setCursor(10, 4);
  oled.println("Calibrate");
  oled.setCursor(10, 6);
  oled.println("Exit");
}

byte sinMap(unsigned long x, int xmin, int xmax, int offset) {
  float pw = (sin(map(x, xmin, xmax, 4712, 6283) / 1000.0) + 1) * (LCDBRI_MAX - offset) + offset;
  return byte(pw);
}

void startLCDFadeIn() {
  if (LCDBacklightMode == LCDSTATE_MIN || LCDBacklightMode == LCDSTATE_FADEOUT) {
    LCDBacklightMode = LCDSTATE_FADEIN;
    mil_onFadeIn = millis();
    LCDinitialPW = LCDcurrentPW;
  }
}

void startLCDFadeOut() {
  if (LCDBacklightMode == LCDSTATE_MAX || LCDBacklightMode == LCDSTATE_FADEIN) {
    LCDBacklightMode = LCDSTATE_FADEOUT;
    mil_onFadeOut = millis();
    LCDinitialPW = LCDcurrentPW;
  }
}

void setLCDMinLight() {
  LCDBacklightMode = LCDSTATE_MIN;
  oled.setContrast(LCDBRI_MIN);
  LCDcurrentPW = LCDBRI_MIN;
}

void setLCDMaxLight() {
  LCDBacklightMode = LCDSTATE_MAX;
  oled.setContrast(LCDBRI_MAX);
  LCDcurrentPW = LCDBRI_MAX;
}
