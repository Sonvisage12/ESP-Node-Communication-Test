#include <WiFi.h>
#include <esp_now.h>

// Replace with the actual MAC address of the Doctor Node (Receiver)
uint8_t doctorMAC[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Optional: Force channel 1
  #include <esp_wifi.h>
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  Serial.print("📡 Patient MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, doctorMAC, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(doctorMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("❌ Failed to add peer");
      return;
    }
  }

  esp_now_register_send_cb(onDataSent);

  Serial.println("✅ Sending test message in 3 seconds...");
  delay(3000);

  const char *msg = "Hello Doctor!";
  esp_now_send(doctorMAC, (uint8_t *)msg, strlen(msg));
}

void loop() {
  // Nothing
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("📤 Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivered 🟢" : "Failed 🔴");
}
