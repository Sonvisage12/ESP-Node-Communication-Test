#include <WiFi.h>
#include <esp_now.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Optional: Force channel 1
  #include <esp_wifi.h>
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
  Serial.println("✅ Doctor Node ready and waiting...");
}

void loop() {
  // Nothing here
}

void onDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.print("📩 Received: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)incomingData[i]);
  }
  Serial.println();
}
