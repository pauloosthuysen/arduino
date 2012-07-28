const int btnPin = 2;
const int ledPin = 3;

const int stopFor = 300000;

int prevBtnState = 0;

void setup(){
  pinMode(btnPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  int val1 = digitalRead(btnPin);
  delay(10);
  int val2 = digitalRead(btnPin);
  if(val1 == val2){
    if(val1 != prevBtnState){
      Serial.println((String)val1);
      if(val1 == HIGH){
        digitalWrite(ledPin, HIGH);
        delay(stopFor);
      }else{
        digitalWrite(ledPin, LOW);
      }
      prevBtnState = val1;
    }
  }
}
