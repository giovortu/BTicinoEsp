#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define VERBOSE 1

#define CHANNEL 1

#define MAX_WAIT_RESPONSE_TIME 10000

long currentMillis = millis();

#define RETRY_INTERVAL 50

static uint8_t broadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint8_t msg[] = "{\"id\":\"bagno\",\"type\":\"button\",\"command\":\"toggle\"}";
uint8_t len = sizeof(msg);

void receiveCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) 
{
#ifdef VERBOSE 
  Serial.println( (char*)incomingData);
  Serial.println("Sleep!");
#endif
  ESP.deepSleep(0);
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

#ifdef VERBOSE  
  Serial.begin(115200);
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
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, CHANNEL, NULL, 0);

  delay(5);

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

