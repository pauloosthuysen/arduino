#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>

byte    mac[] =     { 0x90, 0xA2, 0xDA, 0x0D, 0x23, 0x01 };
const byte    ip[] =      { 192, 168, 0, 71 };
const byte    gateway[] = { 192, 168, 0, 1 };
const byte    subnet[] =  { 255, 255, 255, 0 };
const byte    remoteServerIp[] =  { 192, 168, 0, 3 };
const String  remoteServerUri = "/test.php";
const int  remoteServerPort = 80;
const int     led1Pin = 8;
const int     led2Pin = 9;
const int     led3Pin = 7;
const int     btnPin = 6;
const int     sdPin = 4;
const int     ethCs = 10;
const String arduinoSecret = "ajh32ww";

EthernetServer  server(8088);
EthernetClient client;
String  readString = String(100);
int SDStatus = 1;
int prevBtnState = 0;


void setup()
{    
  Ethernet.begin(mac, ip, gateway, subnet);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(btnPin, INPUT);
  pinMode(ethCs, OUTPUT);
  
  if(!SD.begin(sdPin)){
    SDStatus = 0;
  }
}

void loop(){
  checkRequest();
  checkBtn();
}

void checkRequest(){
  client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (readString.length() < 100) {
          readString.concat(c);
        }

        if (c == '\n') {
          if(getGETVar("secret") == arduinoSecret){
            if (getGETVar("L") == "1") {
              digitalWrite(led1Pin, HIGH);
              clientPrint("<html><body>LED 1 ON</body></html>");
            }
            if (getGETVar("L") == "0") {
              digitalWrite(led1Pin, LOW);
              clientPrint("<html><body>LED 1 OFF</body></html>");
            }
            if (getGETVar("H") == "1") {
              digitalWrite(led2Pin, HIGH);
              clientPrint("<html><body>LED 2 ON</body></html>");
            }
            if (getGETVar("H") == "0") {
              digitalWrite(led2Pin, LOW);
              clientPrint("<html><body>LED 2 OFF</body></html>");
            }
            if (getGETVar("T") == "1") {
              digitalWrite(led3Pin, HIGH);
              clientPrint("<html><body>LED 3 ON</body></html>");
            }
            if (getGETVar("T") == "0") {
              digitalWrite(led3Pin, LOW);
              clientPrint("<html><body>LED 3 OFF</body></html>");
            }
            if(getGETVar("action") == "getStatus"){
              printXMLStatus();
            }
            if(getGETVar("action") == "showLog"){
              printLog();
            }
          }else{
            //addLogEntry("Not authenticated...");
          }
          readString="";
          client.stop();
        }
      }
    }
  }
}

void clientPrint(String s){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(s);
}

void checkBtn(){
  int btnVal1 = digitalRead(btnPin);
  delay(10);
  int btnVal2 = digitalRead(btnPin);
  if(btnVal1 == btnVal2){
    if(btnVal1 != prevBtnState){
      if(btnVal1 == HIGH){
        if(client.connect(remoteServerIp, remoteServerPort)){
          client.println("GET "+remoteServerUri+"?secret="+arduinoSecret+" HTTP/1.0");
          client.println("");
          client.stop();
        }
      }
      prevBtnState = btnVal1;
    }
  }  
}

void printXMLStatus(){
  String led1Status = digitalRead(led1Pin) == HIGH ? "1" : "0";
  String led2Status = digitalRead(led2Pin) == HIGH ? "1" : "0";
  String led3Status = digitalRead(led3Pin) == HIGH ? "1" : "0";
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();
  client.print("[");
  client.print("{\"name\":\"L\",\"value\":\"" + led1Status + "\"},");
  client.print("{\"name\":\"H\",\"value\":\"" + led2Status + "\"},");
  client.print("{\"name\":\"T\",\"value\":\"" + led3Status + "\"}");
  client.print("]");
}

void printLog(){
  File logFile = SD.open("log.txt");
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println();
  if(logFile){
    while(logFile.available()){
      client.print((char)logFile.read());
    }
  }else{
    client.print("Can't open log file...");
  }
  logFile.close();
}

void addLogEntry(String msg){
  File logFile = SD.open("log.txt", FILE_WRITE);
  if(logFile){
    logFile.println(msg);
    logFile.close();
  }
}

String getGETVar(String name){
  int idx = readString.indexOf(name + "=");
  if(idx == -1){
    return "";
  }
  int valIdx = idx + name.length() + 1;
  String lastPart = readString.substring(valIdx, readString.length());
  int endOfValIdx = lastPart.indexOf('&');
  if(endOfValIdx == -1){
    endOfValIdx = lastPart.indexOf(' ');
  }
  return lastPart.substring(0, endOfValIdx);
}
