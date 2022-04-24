/*  Codes that Receives  JSON data using ESPNOW Protocol

*   Developed by  Jay Joshi 
*   github.com/JayJoshi16
* . 
* 
*   Modified by Sachin Soni
*   
*   Developed for techiesms 
*   https://www.youtube.com/techiesms
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

#include <ArduinoJson.h>
String jsondata;
StaticJsonDocument<200> doc;

void OnDataRecv( uint8_t * mac,  uint8_t *incomingData, uint8_t len) {

  char* buff = (char*) incomingData;        //char buffer
  jsondata = String(buff);                  //converting into STRING
  Serial.print("Recieved ");
  Serial.println(jsondata);    //Complete JSON data will be printed here
  DeserializationError error = deserializeJson(doc, jsondata);
    
  if (!error) {
        int a   = doc["1"]; 
        if(a == 1)
        {
        digitalWrite(0,HIGH);
        Serial.println("Relay ON");
        }
        else
        {
        digitalWrite(0,LOW);
        Serial.println("Relay OFF");
        }
  }

  else {
        Serial.print(F("deserializeJson() failed: "));  //Just in case of an ERROR of ArduinoJSon
        Serial.println(error.f_str());
        return;
  }
}

void setup() {

  Serial.begin(115200);
  pinMode(0,OUTPUT);
  WiFi.mode(WIFI_STA);
    
if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  
 esp_now_register_recv_cb(OnDataRecv);         //Reciever CallBack function
    
}

void loop() {

}
