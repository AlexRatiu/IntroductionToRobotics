const int latchPin = 11; 
const int clockPin = 10;  
const int dataPin = 12;   

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

int displayDigits[] = { segD1, segD2, segD3, segD4 };
const int displayCount = 4;  

const int encodingsNumber = 10;

byte byteEncodings[encodingsNumber] = {
 
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
};

byte byteEncodings2[] = {
 
  B11111101,  // 0
  B01100001,  // 1
  B11011011,  // 2
  B11110011,  // 3
  B01100111,  // 4
  B10110111,  // 5
  B10111111,  // 6
  B11100001,  // 7
  B11111111,  // 8
  B11110111,  // 9
};

unsigned long lastIncrement = 0;
unsigned long delayCount = 100;  
unsigned long number = 0;       

int const buttonPins[] = {2, 3, 8};

byte buttonState;
byte lastButtonState = 1;
byte buttonActive = LOW;
int debounceDelay = 50;
long lastDebounceTime = 0;
int resetCount = 0;

byte buttonState2;
byte lastButtonState2 = 1;
byte buttonState3;
byte lastButtonState3 = 1;

unsigned long laps[5] = {0,0,0,0,0};
int currentLap = 0;
int lapCount = 0;


void setup() {
  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  for(int i = 0; i < 3; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  Serial.begin(9600);
}
void loop() {
  
  buttonState = digitalRead(buttonPins[0]);
  
  if((millis()- lastDebounceTime > debounceDelay) && (buttonState == LOW) && (buttonState != lastButtonState)){
    buttonActive = !buttonActive;
    lastDebounceTime = millis(); 
  }
  lastButtonState = buttonState;
  if (millis() - lastIncrement > delayCount && buttonActive == 1) {
      number++;
      number %= 10000;  
      lastIncrement = millis();
    }

  writeNumber(number);

  buttonState2 = digitalRead(buttonPins[1]);
  if((millis()- lastDebounceTime > debounceDelay) && (buttonState2 == LOW) && (buttonActive == 0) && (buttonState2 != lastButtonState2)){
    resetCount ++;
    lastDebounceTime = millis(); 
    if(resetCount == 2){
      resetCount = 0;
    }
  }
  lastButtonState2 = buttonState2;

  if(buttonState2 == 0 && buttonActive == 0 && resetCount == 1){
    number = 0;    
  }
  if(buttonState2 == 0 && buttonActive == 0 && resetCount == 0){
    number = 0;
    for(int i = 0; i<5; i++){
      laps[i]=0;
    }
    currentLap=0;    
  }

  buttonState3 = digitalRead(buttonPins[2]);

  if((millis()- lastDebounceTime > debounceDelay) && (buttonState3 == 0) && (buttonActive == 1) && (buttonState3 != lastButtonState3)){
    lastDebounceTime = millis(); 
    if(currentLap == 5){
      for(int i = 0; i < 5; i++){
        laps[i]=laps[i+1];
      }
      currentLap--;
    }
    laps[currentLap] = number;
    currentLap++;
  }
  

  if((millis()- lastDebounceTime > debounceDelay) && (buttonState3 == 0) && (buttonActive == 0) && (buttonState3 != lastButtonState3) && resetCount == 1){
    
    number = laps[lapCount];
    lapCount++;
    if(lapCount == currentLap){
      lapCount = 0;
    }
    
  }
  lastButtonState3 = buttonState3;

}
void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}
void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }

  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentNumber = number;
  int displayDigit = 3;  
  int lastDigit = 0;
  if(number == 0){
    for(int i=0 ; i<displayCount; i++ ){
      activateDisplay(i);
      if(i == 2){
        writeReg(byteEncodings2[0]);
      }
      writeReg(byteEncodings[0]);
    }

    delay(0);  
    displayDigit--;
    writeReg(B00000000);   
  }
  while (currentNumber != 0) {
    lastDigit = currentNumber % 10;
    activateDisplay(displayDigit);
    if (displayDigit == 2)
      writeReg(byteEncodings2[lastDigit]);
    else
      writeReg(byteEncodings[lastDigit]);
    delay(0);  
    displayDigit--;
    currentNumber /= 10;
    writeReg(B00000000);   
  }

  for (int i = displayDigit; i >= 0; i --)
  {
    activateDisplay(i);
    writeReg(B11111100);
    delay(0);
    writeReg(B00000000); 
  }
}