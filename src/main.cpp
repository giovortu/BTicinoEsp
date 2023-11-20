#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define VERBOSE 1

#define MAX_WAIT_RESPONSE_TIME 10000

long currentMillis = millis();

#define RETRY_INTERVAL 50

uint8_t mac[] = {0xDC, 0x4F, 0x22, 0x10, 0xCA, 0x8A};

static uint8_t broadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint8_t msg[] = "{\"id\":\"gio\",\"command\":\"toggle\"}";
uint8_t len = sizeof(msg);

void receiveCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) 
{
  Serial.println( (char*)incomingData);
  ESP.deepSleep(0);
#ifdef VERBOSE  
  Serial.println("Sleep!");
#endif
}

void sendCallBackFunction(u8 *mac_addr, u8 status) 
{
#ifdef VERBOSE  
  Serial.println("Sent, waiting response...");
#endif
}

void setup() {
  currentMillis = millis();

  WiFi.mode(WIFI_STA);

  Serial.begin(115200);
#ifdef VERBOSE  
  Serial.println();
  Serial.println("ESP-Now Sender");
  Serial.printf("Transmitter mac: %s\n", WiFi.macAddress().c_str());
#endif
  if (esp_now_init() != 0) 
  {
#ifdef VERBOSE      
    Serial.println("ESP_Now init failed...");
#endif    
    delay(RETRY_INTERVAL);
    ESP.restart();
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  esp_now_register_recv_cb(receiveCallBackFunction);
  esp_now_register_send_cb(sendCallBackFunction);
#ifdef VERBOSE  
  Serial.println("Slave ready. Waiting for messages...");
#endif
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  delay(150);

  esp_now_send(broadcastAddress, msg, len);
  

}

void loop() {

if (millis() - currentMillis >= MAX_WAIT_RESPONSE_TIME) 
  {
#ifdef VERBOSE      
    Serial.println("No response, sleep!");
#endif
    ESP.deepSleep(0);
  }

}

