const int dayNightSensor = A0;
const int ledPin = 9;
const int btnPin = 12;
const int onLed = 2;
const int autoLed = 3;

const int nightThreshold = 5;

int isDay = 1;
unsigned long last5000Millis = 0;
int fiveSecTimer = 0;
int lastDayNightValue = 0;
int mode = 0;
int btnLastState = 0;

void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(onLed, OUTPUT);
  pinMode(autoLed, OUTPUT);
  pinMode(btnPin, INPUT);
  last5000Millis = millis();
  lastDayNightValue = analogRead(dayNightSensor);
}

void loop(){  
  set5SecTimer();
  
  if(fiveSecTimer){
    fiveSecTimer = 0;
    do5SecTimer();
  }
  
  checkButtonPress();
  
  switchPin9();  
}

void set5SecTimer(){
  if(millis() - last5000Millis >= 5000){
    last5000Millis = millis();
    fiveSecTimer = 1;
  }
}

void do5SecTimer(){
  checkDayNight();
}

void checkDayNight(){
  int sensorValue = analogRead(dayNightSensor);
  Serial.println("DayNightValue: " + (String)sensorValue);
  if(sensorValue <= nightThreshold){
    isDay = 0;
  }else{
    isDay = 1;
  }
  lastDayNightValue = analogRead(dayNightSensor);
}

void switchPin9(){
  if(mode == 0){
    digitalWrite(ledPin, LOW);
    digitalWrite(autoLed, LOW);
    digitalWrite(onLed, LOW);
  }
  if(mode == 1){
    digitalWrite(ledPin, HIGH);
    digitalWrite(autoLed, LOW);
    digitalWrite(onLed, HIGH);
  }
  if(mode == 2){
    digitalWrite(autoLed, HIGH);
    digitalWrite(onLed, LOW);
    if(!isDay){
      digitalWrite(ledPin, HIGH);
    }else{
      digitalWrite(ledPin, LOW);
    }
  }
}

void checkButtonPress(){
  int curVal = digitalRead(btnPin);
  if(btnLastState != curVal){
    if(curVal == HIGH){
      if(mode < 2){
        mode++;
      }else{
        mode = 0;
      }
      Serial.println("Mode: " + (String)mode);
    }
    btnLastState = curVal;
  }
}
