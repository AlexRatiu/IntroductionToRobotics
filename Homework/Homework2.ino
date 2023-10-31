const int floorLEDPins[3] = {11, 12, 13};  // Pins for floor LEDs
const int stateLEDPin = 9;                // Pin for elevator state LED
const int stateLEDPin2 = 10;
const int buzzerPin = 5;                  // Pin for the buzzer
const int buttonPins[3] = {2, 3, 4};      // Pins for call buttons

const int debounceDelay = 50; // Adjust as needed for debounce

int currentFloor = 0;          // Current floor
int targetFloor = -1;          // Target floor
bool isMoving = false;         // Flag to indicate elevator movement
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
  delay(1000); // Simulate elevator movement time (adjust as needed)a
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
