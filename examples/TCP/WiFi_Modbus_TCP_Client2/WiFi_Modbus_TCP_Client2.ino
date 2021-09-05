/*
  WiFi Modbus TCP Client Toggle

  This sketch toggles the coil of a Modbus TCP server connected
  on and off every second.

  Circuit:
   - MKR1000 or MKR WiFi 1010 board

  created 16 July 2018
  by Sandeep Mistry
*/

#include <SPI.h>
#include <WiFiNINA.h> // for MKR WiFi 1010
// #include <WiFi101.h> // for MKR1000

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
//char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
const int led4Pin =  4;
const int led5Pin =  5;
const int led6Pin =  6;
const int led7Pin =  7;
int coilState = LOW;
int led4State = LOW;
int led5State = LOW;
int led6State = LOW;
int led7State = LOW;

//millis items
unsigned long previousMillis = 500;
const long interval = 500;
unsigned long previousMillis1 = 2067;
const long interval1 = 6813;
unsigned long previousMillis2 = 1325;
const long interval2 = 3785;
unsigned long previousMillis3 = 0;
int interval3 = 1357;


WiFiClient wifiClient;
ModbusTCPClient modbusTCPClient(wifiClient);

IPAddress server(192, 168, 8, 1); // update with the IP Address of your Modbus server

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  Serial.println("Modbus TCP Client Toggle");

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();

  // configure the LED
  pinMode(A0,INPUT); 
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(0,INPUT_PULLUP);
  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(led4Pin, OUTPUT);
  pinMode(led5Pin, OUTPUT);
  pinMode(led6Pin, OUTPUT);
  pinMode(led7Pin, OUTPUT);
  
 

  
}

void loop() {

  unsigned long currentMillis = millis();
  
  if (!modbusTCPClient.connected()) {
    // client not connected, start the Modbus TCP client
    Serial.println("Attempting to connect to Modbus TCP server");
    
    if (!modbusTCPClient.begin(server)) {
      Serial.println("Modbus TCP Client failed to connect!");
      
    } else {
      Serial.println("Modbus TCP Client connected");
      
    }
  } else {
    // client connected
         if (currentMillis - previousMillis1 >= interval1){
          previousMillis1 = currentMillis;
          inputCoils();
         }
               
         if (currentMillis - previousMillis3 >= interval3){
          previousMillis3 = currentMillis;
          inputRegisters();
         } 
         outputCoils();  
           
  }

}//end of loop





  void inputRegisters(){
    modbusTCPClient.beginTransmission(1, 2, 0, 7);
    modbusTCPClient.write(analogRead(A0));
    modbusTCPClient.write(analogRead(A1));
    modbusTCPClient.write(analogRead(A2));
    modbusTCPClient.write(analogRead(A3));
    modbusTCPClient.write(analogRead(A4));
    modbusTCPClient.write(analogRead(A5));
    modbusTCPClient.write(analogRead(A6));
    modbusTCPClient.endTransmission();
    delay(500);
 

    
  }




  void outputCoils(){
   unsigned long currentMillis2 = millis();
    
     if (currentMillis2 - previousMillis2 >= interval2) {
    // save the last time you blinked the LED
    previousMillis2 = currentMillis2;
      if(modbusTCPClient.coilRead(1, 4) == 1){
        led4State = HIGH;      
      }else{
        led4State = LOW;       
      }
      if(modbusTCPClient.coilRead(1, 5) == 1){
        led5State = HIGH;      
      }else{
        led5State = LOW;       
      }
      if(modbusTCPClient.coilRead(1, 6) == 1){
        led6State = HIGH;      
      }else{
        led6State = LOW;       
      }
      if(modbusTCPClient.coilRead(1, 7) == 1){
        led7State = HIGH;      
      }else{
        led7State = LOW;       
      }
       
     }    
  digitalWrite(led4Pin, led4State);
  digitalWrite(led5Pin, led5State);
  digitalWrite(led6Pin, led6State);
  digitalWrite(led7Pin, led7State);
  }




  void inputCoils() {
    modbusTCPClient.beginTransmission(1, 0, 0, 4);
    modbusTCPClient.write(digitalRead(0));
    modbusTCPClient.write(digitalRead(1));
    modbusTCPClient.write(digitalRead(2));
    modbusTCPClient.write(digitalRead(3));
    modbusTCPClient.endTransmission();
    delay(500);
  }




  void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
