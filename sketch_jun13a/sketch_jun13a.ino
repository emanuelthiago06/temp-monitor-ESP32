#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "max6675.h"


int SO = D7;

int CS = D8;

int sck = D5;

MAX6675 thermocouple(SCK, CS, SO);

void setup() {
 
  Serial.begin(9600);                 //Serial connection
  pinMode(4, INPUT); // Setup for leads off detection LO +
  pinMode(5, INPUT); // Setup for leads off detection LO -
  //WiFi.begin("brisa-2983954", "dtiu2ujf");   //WiFi connection
  WiFi.begin("GPDS", "1fp3gpds"); 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
  
  String input2;
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    float temperatura_C = thermocouple.readCelsius();
    input2 = String(temperatura_C);
    HTTPClient http;    //Declare object of class HTTPClient
    WiFiClient wifiClient;
    http.begin(wifiClient,"http://10.0.61.121:8000/snippets/");      //Specify request destination
    http.addHeader("Content-Type", "aplication/json");  //Specify content-type header
    String input = "{\"amp\":";
    input= input + input2;
    input+= "}";
    int httpCode = http.POST(input);   //Send the request
    String payload = http.getString();                  //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(input);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(2000);  //Send a request every 30 seconds
 
}
