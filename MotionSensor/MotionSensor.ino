const int sensorPin = 2;
const int ledPin = 6;
const int dayNight = A1;

unsigned long lastMillis = 0;
unsigned long minute = 1;
int dayNightThreshold = 60;
unsigned long interval = minute * 60 * 1000;

unsigned long lastOneSecMillis = 0;

int lightOn = 0;
int isDark = 0;

void setup(){
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop(){
  int sensorVal = digitalRead(sensorPin);
  if(lightOn && (millis() - lastMillis >= interval)){
    if(sensorVal == HIGH){
      digitalWrite(ledPin, LOW);
      lightOn = 0;
    }
  }
  if(sensorVal == LOW){
    if(isDark){
      digitalWrite(ledPin, HIGH);
      lightOn = 1;
    }
    lastMillis = millis();
  }
  
  if(millis() - lastOneSecMillis >= 1000){
    int dayNightVal = analogRead(dayNight);
    Serial.println("dayNight Val: "+(String)(sensorVal == HIGH ? "1":"0"));
    if(dayNightVal <= dayNightThreshold){
      isDark = 1;
    }else{
      isDark = 0;
    }
    lastOneSecMillis = millis();
  }
}
