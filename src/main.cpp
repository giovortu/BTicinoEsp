#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define MAX_WAIT_RESPONSE_TIME 10000

long currentMillis = millis();

#define RETRY_INTERVAL 5000

uint8_t mac[] = {0xDC, 0x4F, 0x22, 0x10, 0xCA, 0x8A};

static uint8_t broadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint8_t msg[] = "{\"id\":\"bagno\",\"command\":\"toggle\"}";
uint8_t len = sizeof(msg);

void receiveCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) 
{
  Serial.println("Received!");
  ESP.deepSleep(0);
  Serial.println("Sleep!");
}

void sendCallBackFunction(u8 *mac_addr, u8 status) 
{
  Serial.println("Sent!");
  ESP.deepSleep(0);
  Serial.println("Sleep!");
}

void setup() {
  currentMillis = millis();

  WiFi.mode(WIFI_STA);

  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP-Now Sender");
  Serial.printf("Transmitter mac: %s\n", WiFi.macAddress().c_str());

  if (esp_now_init() != 0) 
  {
    Serial.println("ESP_Now init failed...");
    delay(RETRY_INTERVAL);
    ESP.restart();
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  esp_now_register_recv_cb(receiveCallBackFunction);
  esp_now_register_send_cb(sendCallBackFunction);

  Serial.println("Slave ready. Waiting for messages...");

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  delay(100);

  esp_now_send(broadcastAddress, msg, len);
  

}

void loop() {

if (millis() - currentMillis >= MAX_WAIT_RESPONSE_TIME) 
  {
    Serial.println("No response, sleep!");

    ESP.deepSleep(0);

  }

}

