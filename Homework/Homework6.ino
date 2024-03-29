#include "LedControl.h"
#include <EEPROM.h>
 
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
//controller
const int xPin = A0;
const int yPin = A1;
const int pinSW = A2;
 
 
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
int selectedMap = 0;
byte matrixBrightness = 2;
 
const int numMaps = 3;  // Number of matrix maps to do more and select random 
byte matrixMap[numMaps][8][8] = {
  {
    {1, 0, 1, 1, 0, 0, 1, 0},
    {1, 1, 0, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 0, 0},
    {1, 1, 0, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 0, 1},
    {0, 1, 0, 1, 1, 1, 0, 0},
    {1, 0, 0, 0, 1, 0, 1, 1},
    {0, 0, 0, 1, 1, 0, 0, 0}
  },
  {
    {1, 0, 1, 0, 1, 1, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1}
  },
  {
    {1, 0, 0, 1, 0, 1, 0, 1},
    {0, 1, 1, 0, 1, 0, 1, 0},
    {1, 0, 1, 0, 0, 1, 0, 1},
    {0, 1, 1, 1, 1, 0, 1, 0},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1}
  }
};
 
byte matrixSize = 8;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
 
const int minThreshold = 200;
const int maxThreshold = 600;
 
const byte moveInterval = 100;
unsigned long long lastMoved = 0;
bool matrixChanged = true;
 
int xLast;
int yLast;

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  pinMode(pinSW, INPUT_PULLUP);
  selectedMap = EEPROM.read(0);
  selectedMap = (selectedMap + 1) % numMaps;
  EEPROM.write(0, selectedMap); 
 
  Serial.println(selectedMap);
 
  matrixMap[selectedMap][xPos][yPos] = 1;
 
  updateMatrix();
}
 
  boolean buttonState = false;
  boolean exist = false;
  int xBlink = -1;
  int yBlink = -1;
  int lastPositionSetTime;

void loop() {
  boolean currentButtonState = digitalRead(pinSW);
 
  if (currentButtonState == LOW) {
    buttonState = !buttonState;
  }
 
  if (millis() - lastMoved >= moveInterval) {
    xLast = xPos;
    yLast = yPos;
    updatePositions();
 
    if ((xLast != xPos || yLast != yPos) && buttonState == true) {
      Serial.print("X: ");
      Serial.println(xLast);
      Serial.print("Y: ");
      Serial.println(yLast);
 
      matrixMap[selectedMap][xLast][yLast] = 1;
      lc.setLed(1, xLast, yLast, matrixMap[selectedMap][xLast][yLast]);
 
      lastPositionSetTime = millis();
 
      exist = true;
      xBlink = xLast;
      yBlink = yLast;
      buttonState = false;
    }
    lastMoved = millis();
  }
 
  if (exist) {
    blinkFast(xBlink, yBlink);
  }
 
if (exist && millis() - lastPositionSetTime >= 3000) {
    matrixMap[selectedMap][xBlink][yBlink] = 0;
    lc.setLed(1, xBlink, yBlink, matrixMap[selectedMap][xBlink][yBlink]);
 
    if (xBlink > 0) {
      matrixMap[selectedMap][xBlink - 1][yBlink] = 0;
      lc.setLed(1, xBlink - 1, yBlink, matrixMap[selectedMap][xBlink - 1][yBlink]);
    }
 
    if (xBlink < (8 - 1)) {
      matrixMap[selectedMap][xBlink + 1][yBlink] = 0;
      lc.setLed(1, xBlink + 1, yBlink, matrixMap[selectedMap][xBlink + 1][yBlink]);
    }
 
    if (yBlink > 0) {
      matrixMap[selectedMap][xBlink][yBlink - 1] = 0;
      lc.setLed(1, xBlink, yBlink - 1, matrixMap[selectedMap][xBlink][yBlink - 1]);
    }
 
    if (yBlink < (8 - 1)) {
      matrixMap[selectedMap][xBlink][yBlink + 1] = 0;
      lc.setLed(1, xBlink, yBlink + 1, matrixMap[selectedMap][xBlink][yBlink + 1]);
    }

     if ((xBlink > 0) && (yBlink > 0)) {
      matrixMap[selectedMap][xBlink - 1][yBlink - 1] = 0;
      lc.setLed(1, xBlink - 1, yBlink - 1, matrixMap[selectedMap][xBlink - 1][yBlink - 1]);
     }

     if ((xBlink < (8 - 1)) && (yBlink < (8 - 1))) {
      matrixMap[selectedMap][xBlink + 1][yBlink + 1] = 0;
      lc.setLed(1, xBlink + 1, yBlink + 1, matrixMap[selectedMap][xBlink + 1][yBlink + 1]);
     }

     if ((xBlink > 0) && (yBlink < (8 - 1))) {
      matrixMap[selectedMap][xBlink - 1][yBlink + 1] = 0;
      lc.setLed(1, xBlink - 1, yBlink + 1, matrixMap[selectedMap][xBlink - 1][yBlink + 1]);
     }

     if ((xBlink < (8 - 1)) && (yBlink > 0)) {
      matrixMap[selectedMap][xBlink + 1][yBlink - 1] = 0;
      lc.setLed(1, xBlink + 1, yBlink - 1, matrixMap[selectedMap][xBlink + 1][yBlink - 1]);
     }
 
    exist = false;
    updateMatrix();
}
 
 
  if (matrixChanged) {
    updateMatrix();
    matrixChanged = false;
  }
 
  blink(xPos, yPos);
}
 
void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixMap[selectedMap][row][col]);
    }
  }
}
 
void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int hasMoved = false;
  byte newXPos = xPos;
  byte newYPos = yPos;
 
  if (xValue < minThreshold && newXPos > 0 && hasMoved == false) {
    newXPos--;
    hasMoved = true;
  } else if (xValue > maxThreshold && newXPos < matrixSize - 1 && hasMoved == false) {
    newXPos++;
        hasMoved = true;
 
  }
 
  if (yValue < minThreshold && newYPos < matrixSize - 1 && hasMoved == false) {
    newYPos++;    hasMoved = true;
 
  } else if (yValue > maxThreshold && newYPos > 0 && hasMoved == false) {
    newYPos--;    hasMoved = true;
 
  }
 
  if (matrixMap[selectedMap][newXPos][newYPos] == 0) {
    matrixChanged = true;
    matrixMap[selectedMap][xPos][yPos] = 0;
    matrixMap[selectedMap][newXPos][newYPos] = 1;
    xLastPos = xPos;
    yLastPos = yPos;
    xPos = newXPos;
    yPos = newYPos;
  }
}
 
void blink(byte x, byte y ) {
  static unsigned long lastBlinkTime = 0;
  static bool isOn = true;
  static unsigned blinkInterval = 400;
  if (millis() - lastBlinkTime >= blinkInterval) {
    isOn = !isOn;
    lc.setLed(0, x, y, isOn);
 
    lastBlinkTime = millis();
  }
}
void blinkFast(byte x, byte y) {
  static unsigned long lastBlinkTime = 0;
  static bool isOn = true;
  static unsigned blinkInterval = 200;
  if (millis() - lastBlinkTime >= blinkInterval) {
    isOn = !isOn;
    lc.setLed(0, x, y, isOn);
 
    lastBlinkTime = millis();
  }
}