/*
  Flow of the code

  1 - Put WiFi in STA Mode
  2 - Intialize ESPNOW
  3 - Set Role to Combo
  4 - Add peer device
  5 - Define Send Callback Function
  6 - Define Receive Callback Function
*/

#define but1 0
#define but2 2

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>
String jsondata;

StaticJsonDocument<200> doc;

// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Variable to store if sending data was successful
String success;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}


void setup()
{
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(but1, INPUT);
  pinMode(but2, INPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
}

void loop()
{
  delay(2000);
  jsondata = "";  //clearing String after data is being sent

  if (digitalRead(but1) == LOW)
  {
    doc["1"] = 1;
    Serial.println("Button 1 Pressed");
    serializeJson(doc, jsondata);  //Serilizing JSON

    esp_now_send(broadcastAddress, (uint8_t *) jsondata.c_str(), sizeof(jsondata) + 1);  //Sending "jsondata"
  }

  if (digitalRead(but2) == LOW)
  {
    doc["1"] = 0;
    Serial.println("Button 2 Pressed");
    serializeJson(doc, jsondata);  //Serilizing JSON

    esp_now_send(broadcastAddress, (uint8_t *) jsondata.c_str(), sizeof(jsondata) + 1);  //Sending "jsondata"
  }


  ESP.deepSleep(0);
}
