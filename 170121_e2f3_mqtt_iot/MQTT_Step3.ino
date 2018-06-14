#include <ESP8266WiFi.h>
#include <PubSubClient.h>  /* Step2で追加 */

const char* ssid = "<Your WiFi SSID>";
const char* password = "<Your WiFi PASS>";

const char* mqtt_server = "iot.eclipse.org";    /* Step2で追加 */
const char* mqtt_client_id = "E2F20170121";     /* Step2で追加 */ /*★Point: ユニークな名前とすること */
const char* mqtt_pub_topic = "e2f_mqtt_test";   /* Step3で追加 */

WiFiClient espClient;            /* Step2で追加 */
PubSubClient client(espClient);  /* Step2で追加 */

void setup() {
  Serial.begin(115200);  /* printf用 */
  setup_wifi();

  client.setServer(mqtt_server, 1883);   /* Step2で追加 */ /* MQTTブローカーを指定 */
  if (client.connect(mqtt_client_id)) {  /* Step2で追加 */ /* MQTTブローカーに接続 */
    Serial.println("MQTT Connected");    /* Step2で追加 */
  }                                      /* Step2で追加 */
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
  /* 以下、Step3で追加 */
  static long cnt = 0;
  static long lastMsg = 0;
  long now = millis();

  if (client.connected()) {
    if (now - lastMsg > 5000) {
      lastMsg = now;
      client.publish(mqtt_pub_topic, ("hello E2F " + String(cnt)).c_str()); /* publish */
      Serial.println("Pub " + String(cnt));
      cnt++;
    }
  } else {
    Serial.println("Disconnected " + String(cnt));
    while (1);
  }
}
