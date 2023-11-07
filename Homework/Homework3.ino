const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int pinSW = 2;
const int joystickXPin = A0;
const int joystickYPin = A1;

const int segmentPins[] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
const int numSegments = 8;

int currentSegment = 7;

void(* resetFunc)(void)=0;

byte swState = LOW;
byte lastSwState = LOW;

unsigned long currentMillis;
unsigned long previousMillis = 0;
byte ledState = LOW;

long currentMillisSw;
long previousMillisSw = 0;

int keep[8];
int a = 0;

int joyX;
int joyY;

int position = 0;
int lastPosition = 0;


void setup() {
  for (int i = 0; i < numSegments; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
  digitalWrite(pinDP, HIGH);
}

void loop() {
  joyX = analogRead(joystickXPin);
  joyY = analogRead(joystickYPin);

  position = 0;

  if (joyX > 800){
    position = 1;
  }
  else if (joyX < 200){
    position = 2;
  }
  else if (joyY > 800){
    position = 3;
  }
  else if (joyY < 200){
    position = 4;
  }

  if(position != lastPosition){
    movement();
  }
  lastPosition = position;

  keepON();

  currentMillis = millis();
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(segmentPins[currentSegment], ledState);
  }

  swState = digitalRead(pinSW);
  if (swState == LOW){
    currentMillisSw = millis();
  }
  if (swState == HIGH){
    previousMillisSw = millis();
  }

  if(currentMillisSw - previousMillisSw > 1500){
    for(int i = 0; i < 8; i++){
      digitalWrite(segmentPins[i], LOW);
    }
    resetFunc();
  }
  
}

void movement(){
  if (position == 1) {
    if(currentSegment == 0){
      currentSegment = 0;
    }
    else if(currentSegment == 1){
      currentSegment = 0;
    }
    else if(currentSegment == 2){
      currentSegment = 6;
    }
    else if(currentSegment == 3){
      currentSegment = 6;
    }
    else if(currentSegment == 4){
      currentSegment = 6;
    }
    else if(currentSegment == 5){
      currentSegment = 0;
    }
    else if(currentSegment == 6){
      currentSegment = 0;
    }
  } 
  else if (position == 2) {
    if(currentSegment == 0){
      currentSegment = 6;
    }
    else if(currentSegment == 1){
      currentSegment = 6;
    }
    else if(currentSegment == 2){
      currentSegment = 3;
    }
    else if(currentSegment == 3){
      currentSegment = 3;
    }
    else if(currentSegment == 4){
      currentSegment = 3;
    }
    else if(currentSegment == 5){
      currentSegment = 6;
    }
    else if(currentSegment == 6){
      currentSegment = 3;
    }
  } 
  else if (position == 3) {
    if(currentSegment == 0){
      currentSegment = 1;
    }
    else if(currentSegment == 1){
      currentSegment = 1;
    }
    else if(currentSegment == 2){
      currentSegment = 7;
    }
    else if(currentSegment == 3){
      currentSegment = 2;
    }
    else if(currentSegment == 4){
      currentSegment = 2;
    }
    else if(currentSegment == 5){
      currentSegment = 1;
    }
    else if(currentSegment == 6){
      currentSegment = 6;
    }
  }
  else if (position == 4) {
    if(currentSegment == 0){
      currentSegment = 5;
    }
    else if(currentSegment == 1){
      currentSegment = 5;
    }
    else if(currentSegment == 2){
      currentSegment = 4;
    }
    else if(currentSegment == 3){
      currentSegment = 4;
    }
    else if(currentSegment == 4){
      currentSegment = 4;
    }
    else if(currentSegment == 5){
      currentSegment = 5;
    }
    else if(currentSegment == 6){
      currentSegment = 6;
    }
    else if(currentSegment == 7){
      currentSegment = 2;
    }
  } 
  for(int i = 0; i < 8; i++){
    digitalWrite(segmentPins[i], LOW);
  }
}

void keepON(){
  swState = digitalRead(pinSW);
  if(swState != lastSwState) {
    if(swState == LOW) {
      keep[a]=currentSegment;
      a++;
    }
  }
  lastSwState = swState;

  for(int b = 0; b < a; b++){
    if(keep[b] != currentSegment){
      digitalWrite(segmentPins[keep[b]], HIGH); 
    }
  }
}