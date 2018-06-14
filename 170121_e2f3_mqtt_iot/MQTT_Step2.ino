#include <ESP8266WiFi.h>
#include <PubSubClient.h>  /* Step2で追加 */

const char* ssid = "<Your WiFi SSID>";
const char* password = "<Your WiFi PASS>";

const char* mqtt_server = "iot.eclipse.org";    /* Step2で追加 */
const char* mqtt_client_id = "E2F20170121";     /* Step2で追加 */ /*★Point: ユニークな名前とすること */

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
}
