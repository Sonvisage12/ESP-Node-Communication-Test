#include <WiFi.h>
#include <esp_now.h>

// ✅ Correct: esp_wifi.h must be included properly
extern "C" {
  #include "esp_wifi.h"
}

// ✅ Corrected callback signature
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  Serial.print("📩 Received: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)incomingData[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
    Serial.print("WiFi MAC Address: ");
  Serial.println(WiFi.macAddress());

  // ✅ Correct: setting Wi-Fi parameters
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
