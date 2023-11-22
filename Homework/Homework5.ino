#include <EEPROM.h>
int incomingByte = 0;
int ok = 0;
int x = 0;
int y = 0;
int z = 0;
long lastSamplingTime = 0;

const int greenLedPin = 6;
const int blueLedPin = 5;
const int redLedPin = 3;

const int trigPin = 9;
const int echoPin = 10;
long duration = 0;
int distance = 0;

int photocellPin = 0; 
int photocellValue; 


int EEPROM_DISTANCE_ADDR = 10; 
int EEPROM_PHOTOCELL_ADDR = 21;

int distanceIndex = 0;
int photocellIndex = 0;

int red;
int green;
int blue;

byte isAuto = 1;

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT); 
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  Serial.begin(9600);
  mainMenu();
}

void loop() {
  if (Serial.available()>0){
    incomingByte = Serial.read() - 48;
    switch(ok){
      case 0:{
        switch(incomingByte){
          case 1:{
            Serial.println("1.1 Sensors Sampling Interval");
            Serial.println("1.2 Ultrasonic Alert Threshold");
            Serial.println("1.3 LDR Alert Threshold");
            Serial.println("1.4 Back");
            Serial.println();
            ok = 1;
            break;
          }
          case 2:{
            Serial.println("Are you sure?");
            Serial.println("2.1 Yes");
            Serial.println("2.2 No");
            Serial.println();
            ok = 2;
            break;
          }
          case 3:{
            Serial.println("3.1 Current Sensor Readings");
            Serial.println("3.2 Current Sensor Settings");
            Serial.println("3.3 Display Logged Data");
            Serial.println("3.4 Back");
            Serial.println();
            ok = 3;
            break;
          }
          case 4:{
            Serial.println("4.1 Manual Color Control");
            Serial.println("4.2 LED: Toggle Automatic ON/OFF");
            Serial.println("4.3 Back");
            Serial.println();
            ok = 4;
            break;
          }
        }
        break;
      }

      case 1:{
        switch(incomingByte){
          case 1:{
            Serial.println("What should be the sampling interval?(between 1 and 10 seconds)");
            clearSerial();
            while (!Serial.available()) {}
            x = Serial.read() - 48;
            Serial.print("Sampling interval is: ");
            Serial.println(x);
            Serial.println();
            break;
          }
          case 2:{
            Serial.println("Ultrasonic Threshold");
            clearSerial();
            while (!Serial.available()) {}
            y = Serial.parseInt();
            Serial.print("Ultrasonic min value: ");
            Serial.println(y);
            Serial.println();
            break;
          }
          case 3:{
            Serial.println("LDR Threshold");
            clearSerial();
            while (!Serial.available()) {}
            z = Serial.parseInt();
            Serial.print("LDR min value: ");
            Serial.println(z);
            Serial.println();
            break;
          }
          case 4:{
            ok = 0;
            mainMenu();
            break;
          }
        }
        break;
      }

      case 2:{
        switch(incomingByte){
          case 1:{
            for(int i = 0; i < 10; i++){
              EEPROM.put(EEPROM_DISTANCE_ADDR + i , 0);
              EEPROM.put(EEPROM_PHOTOCELL_ADDR + i , 0);
            } 
            ok = 0;
            mainMenu();
            break;
          }
          case 2:{
            ok = 0;
            mainMenu();
            break;
          }
        }
        break;
      }

      case 3:{
        switch(incomingByte){
          case 1:{
            Serial.println("For exit press 0");
            clearSerial();
            while(Serial.available() == 0)
            if((millis() - lastSamplingTime) >= x * 1000){
              ultrasonicSensor();
              LDRsensor();
              Serial.print("Current distance: ");
              Serial.println(distance);
              Serial.print("Current lighting: ");
              Serial.println(photocellValue);
              lastSamplingTime = millis();
              Serial.println();
            }
            int exit = Serial.read() - 48;
            if(exit == 0){
              Serial.println("3.1 Current Sensor Readings");
              Serial.println("3.2 Current Sensor Settings");
              Serial.println("3.3 Display Logged Data");
              Serial.println("3.4 Back");
              Serial.println();
              ok = 3;
            }
            break;
          }
          case 2:{
            Serial.print("Samplin Interval: ");
            Serial.println(x);
            Serial.print("Ultrasonic Threshold: ");
            Serial.println(y);
            Serial.print("LDR Threshold: ");
            Serial.println(z);
            Serial.println();
            break;
          }
          case 3:{
            Serial.println("Last 10 Values:");
            Serial.println();
            for(int i = 0; i < 10; i++){
              Serial.print("Distance: ");
              Serial.println(EEPROM.read(EEPROM_DISTANCE_ADDR + i));
              Serial.print("LDR: ");
              Serial.println(EEPROM.read(EEPROM_PHOTOCELL_ADDR + i));
              Serial.println();
            }
            Serial.println();
            break;
          }
          case 4:{
            ok = 0;
            mainMenu();
            break;
          }
        }
        break;
      }

      case 4:{
        switch(incomingByte){
          case 1:{
            clearSerial();
            Serial.println("Enter the Red value (0-255)");
            while (!Serial.available()) {}
            red = Serial.parseInt();

            clearSerial();
            Serial.println("Enter the Green value (0-255)");
            while (!Serial.available()) {}
            green = Serial.parseInt();

            clearSerial();
            Serial.println("Enter the Blue value (0-255)");
            while (!Serial.available()) {}
            blue = Serial.parseInt();

            Serial.println();
            Serial.println("Selected RGB values: ");

            Serial.print("Red: ");
            Serial.println(red);

            Serial.print("Green: ");
            Serial.println(green);

            Serial.print("Blue: ");
            Serial.println(blue);

            Serial.println();

            analogWrite(redLedPin, red);
            analogWrite(greenLedPin, green);
            analogWrite(blueLedPin, blue);
            break;
          }
          case 2:{
            isAuto = !isAuto;
            if (isAuto) {
              Serial.println("Auto turned ON.");
              analogWrite(redLedPin, 0);
              analogWrite(greenLedPin, 0);
              analogWrite(blueLedPin, 0);
            }
            else{
              Serial.println("Auto turned OFF.");
              analogWrite(redLedPin, red);
              analogWrite(greenLedPin, green);
              analogWrite(blueLedPin, blue);
            } 
            Serial.println();
            break;
          }
          case 3:{
            ok = 0;
            mainMenu();
            break;
          }
        }
        break;
      }
    } 
  }

  ultrasonicSensor();
  LDRsensor();

  
  
  if(((millis() - lastSamplingTime) >= x * 1000) && isAuto == 1){
    if(distance < y){
      analogWrite(redLedPin, 123);
      analogWrite(greenLedPin, 0);
      Serial.println("Object near device!");
      Serial.println();
    }
    if(photocellValue < z){
      analogWrite(redLedPin, 255);
      analogWrite(greenLedPin, 0);
      Serial.println("It's getting dark!");
      Serial.println();
    }
    if(photocellValue > z && distance > y){
      analogWrite(redLedPin, 0);
      analogWrite(greenLedPin, 255);
    }

    EEPROM.update(EEPROM_DISTANCE_ADDR , distance);
    EEPROM_DISTANCE_ADDR = (EEPROM_DISTANCE_ADDR + 1) % 10;

    EEPROM.update(EEPROM_PHOTOCELL_ADDR , photocellValue);
    EEPROM_PHOTOCELL_ADDR = (EEPROM_PHOTOCELL_ADDR + 1) % 10;
    lastSamplingTime = millis();
  }
}

void mainMenu(){
  Serial.println("Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
  Serial.println();
}

void clearSerial(){ 
  char wrk[1];
  delay(100);
  while(Serial.available() != 0){
    Serial.readBytes(wrk,1);
  } 
  delay(1000);
} 

void ultrasonicSensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  //Serial.print("Distance: ");
  //Serial.println(distance); 
}

void LDRsensor(){
  photocellValue= analogRead(photocellPin);
  //Serial.print("Analog reading = ");
  //Serial.println(photocellValue);
}