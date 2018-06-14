#include <ESP8266WiFi.h>

const char* ssid = "<Your WiFi SSID>";
const char* password = "<Your WiFi PASS>";

void setup() {
  Serial.begin(115200);  /* printf用 */
  setup_wifi();
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);  /* WiFi接続 */

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());  /* IPアドレス表示 */
}

void loop() {
}
