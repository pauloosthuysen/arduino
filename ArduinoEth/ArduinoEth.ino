//*******************************

#include <SPI.h>
#include <Ethernet.h>

/*
Simple Ethernet Test
Arduino server outputs simple text to browser
and controlling LED with simple checkbox

The circuit:
* Arduino Duemilanove
* Arduino Ethernet shield
* LED connected to GND and digital pin 4 via resistor
By Minde
http://www.sciencprog.com/

**************************************************************
*  Edited by Haavard GJ to work in version 0021              *
*  Thanks to PaulS for the big help on getting this to work  *
**************************************************************
*/


 byte    mac[] =     { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
 byte    ip[] =      { 192, 168, 0, 71 };                     // ip in lan
 byte    gateway[] = { 192, 168, 0, 1 };                       // the IP of the router or acsesspoint
 byte    subnet[] =  { 255, 255, 255, 0 };                     //subnet mask (i dont think this is neccesary
 EthernetServer  server(80);                // server port (change this if you are having a local webserver else than the arduino)
 int     ledPin = 8;                // LED pin
 int     heatpin = 9;               // Heating *relay* (change this if you want)
 int     sensorPin = A0;            // analog in 0 for testing
 int     sensorValue = 0;           // integer for the analog sensor
 String  readString = String(30);   // string for fetching data from address
 boolean LEDON = false;             // LED status flag
 boolean HEATON = false;             // Heat status flag  (add more status flags if you need more outputs)


void setup()
{    
  Serial.begin(57600); //enable serial datada print
  Ethernet.begin(mac, ip, gateway, subnet); //start Ethernet
  
  pinMode(ledPin, OUTPUT); //Set pin 4 to output
  pinMode(heatpin, OUTPUT);
  Serial.print("Starting server"); //for debugging
  
  sensorValue = analogRead(sensorPin);
  Serial.println("Analog in:"); //for debugging
  Serial.println("");
  Serial.println (sensorValue);
  Serial.println("");
  Serial.println("");

}

void loop(){

  EthernetClient client = server.available();  // Create a client connection
    if (client) {
      while (client.connected()) {
        if (client.available()) {

          char c = client.read();

        if (readString.length() < 30) { //read char by char HTTP request
          readString.concat(c); } //store characters to string
          
        Serial.print(c); //output chars to serial port for debugging
        

//indexOf("L=")

        if (c == '\n') { //if HTTP request has ended
        
          Serial.println("");
          Serial.println(readString); // print for debugging
          Serial.println("");
          int Le = readString.indexOf("L="); //here is a key component of where
          int He = readString.indexOf("H="); //the status is being read by the arduino
          Serial.print("L= position: ");
          Serial.println(Le);
          Serial.print("H= position: ");
          Serial.println(He);
          
          
          
          
          
          //lets check if LED should be lighted
          if (Le > 1){
          
          if (readString.substring(Le,(Le+3)) == "L=1") { //led has to be turned ON
              digitalWrite(ledPin, HIGH); // set the LED on
              Serial.println("ledpin paa");
              LEDON = true;
          }

          if (readString.substring(Le,(Le+3))== "L=0") {
            //led has to be turned OFF
            digitalWrite(ledPin, LOW); // set the LED OFF
            Serial.println("ledpin av");
            LEDON = false;
           }}
          
           if (He > 1){
            
          if (readString.substring(He,(He+3)) == "H=1") { //heat has to be turned ON
              digitalWrite(heatpin, HIGH); // set the heat on
              Serial.println("heatpin paa");
              HEATON = true;
          }

          if (readString.substring(He,(He+3)) == "H=0") {
            //heat has to be turned OFF
            digitalWrite(heatpin, LOW); // set the heat OFF
            Serial.println("heatpin av");
            HEATON = false;
           }}


  client.println("HTTP/1.1 200 OK"); //output HTML data starting with standart header
  client.println("Content-Type: text/html");
  client.println();
  client.print  ("<body style=background-color:white>"); //set background to white

  // HTML Code

  client.println("<font color='red'><h1>Arduino Remote control</font></h1>");//send first heading
  client.println("<hr />");
  client.println("<font color='blue' size='5'>Analog input: "); //output some sample data to browser
  
  
  sensorValue = analogRead(sensorPin);


  client.print(sensorValue);//Analog input.
  Serial.print(sensorValue);

  client.println("<br />");//some space between lines
  client.println("<br />");
  client.println("<hr />");


  //controlling led via checkbox
  client.println("<h1>LED control</h1>");

  // address will look like http://192.168.xxx.xxx/?L=1 when submited
  // this line will give the radiobuttons that give the input
  client.println("<form method=get name=LED> <input type='radio' name='L' value='1'>LED ON<br><input type='radio' name='L' value='0'>LED OFF<br><input type='radio' name='H' value='1'>HEAT ON<br><input type='radio' name='H' value='0'>HEAT OFF<br><input type=submit value=submit></form>");

  client.println("<br />");
  //printing LED status
  client.print("<font size='5'>LED status: ");
  if (LEDON == true) {
     client.println("<font color='green' size='5'>ON");
     Serial.print("led on");
   }
   else {
    client.println("<font color='grey' size='5'>OFF");
    Serial.println("Led off");
   }

 //printing LED status
  client.print("<font size='5'>Heatpin status: ");
  if (HEATON == true) {
     client.println("<font color='green' size='5'>ON");
     Serial.print("Heat on");
   }
   else {
    client.println("<font color='grey' size='5'>OFF");
    Serial.println("Heat off");
   }



  client.println("<hr />");
  client.println("</body></html>");


readString=""; //clearing string for next read

client.stop(); //stopping client

Serial.println("stopped client"); // for debugging

}}}}}
