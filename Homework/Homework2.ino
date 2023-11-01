const int floorLEDPins[3] = {11, 12, 13};  
const int stateLEDPin = 9;                
const int stateLEDPin2 = 10;
const int buzzerPin = 5;                 
const int buttonPins[3] = {2, 3, 4};      
int currentFloor = 0;          
int targetFloor = -1;          
bool isMoving = false;         
bool buttonStates[3] = {false, false, false};

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(floorLEDPins[i], OUTPUT);
  }
  pinMode(stateLEDPin, OUTPUT);
  pinMode(stateLEDPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }  
  updateLEDs();
}

void loop() {
  checkButtons();
  if (isMoving) {
    moveElevator();
    tone(buzzerPin, 200);
  }
  else
    noTone(buzzerPin);
}

void checkButtons() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttonPins[i]) == LOW && !buttonStates[i]) {
      buttonStates[i] = true;
      if (i != currentFloor && targetFloor == -1) {
        targetFloor = i;
        if (!isMoving) {
          startElevator();
        }
      }
    } else if (digitalRead(buttonPins[i]) == HIGH) {
      buttonStates[i] = false;
    }
  }
}

void startElevator() {
  isMoving = true;
  digitalWrite(stateLEDPin2, HIGH);
  digitalWrite(stateLEDPin, LOW);
  tone(buzzerPin, 5000);
  delay(200);
  noTone(buzzerPin);
}

void moveElevator() {
  if (currentFloor < targetFloor) {
    currentFloor++;
  } else if (currentFloor > targetFloor) {
    currentFloor--;
  } else {
    stopElevator();
  }
  updateLEDs();
  delay(1000); 
}

void stopElevator() {
  isMoving = false;
  digitalWrite(stateLEDPin2, LOW);
  digitalWrite(stateLEDPin, HIGH);
  tone(buzzerPin, 2000);
  delay(200);
  noTone(buzzerPin);
  targetFloor = -1;
}

void updateLEDs() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(floorLEDPins[i], (i == currentFloor) ? HIGH : LOW);
  }
}
