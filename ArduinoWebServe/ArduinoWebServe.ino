#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>

#include <SD.h>
#include <SPI.h>

const int     sdPin = 4;
const int     ethCs = 10;
int SDStatus = 0;
EthernetServer  server(80);
EthernetClient client;

byte    mac[] =     { 0x90, 0xA2, 0xDA, 0x0D, 0x23, 0x01 };
const byte    ip[] =      { 192, 168, 0, 71 };
const byte    gateway[] = { 192, 168, 0, 1 };
const byte    subnet[] =  { 255, 255, 255, 0 };

void setup(){
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
  if(SD.begin(sdPin) && SDStatus != 0){
    SDStatus = 0;
  }
}

void loop(){
  //getFileContents("test.txt");
  checkRequest();
}

void checkRequest(){
  client = server.available();
  if(client){
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        if (c == '\n') {
          clientPrint(getFileContents("index.htm"));
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
  client.print(s);
}

String getFileContents(char* filename){
  String ret = "";
  File file = SD.open(filename);
  if(file){
    while(file.available()){
      ret += (char)file.read();
    }
  }
  file.close();
  return ret;
}
