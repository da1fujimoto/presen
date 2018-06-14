#include <ESP8266WiFi.h>
#include <PubSubClient.h>  /* Step2で追加 */
#include <Wire.h>
#include "SparkFunHTU21D.h"

const char* ssid = "<Your WiFi SSID>";
const char* password = "<Your WiFi PASS>";

const char* mqtt_server = "iot.eclipse.org";    /* Step2で追加 */
const char* mqtt_client_id = "E2F20170121";     /* Step2で追加 */ /*★Point: ユニークな名前とすること */
const char* mqtt_pub_topic = "e2f_mqtt_test";   /* Step3で追加 */
const char* mqtt_sub_topic = mqtt_pub_topic;    /* Step4で追加 */
const char* mqtt_pub_temp = "e2f_mqtt_test/sensor/temperature";
const char* mqtt_pub_hum  = "e2f_mqtt_test/sensor/humidity";

WiFiClient espClient;            /* Step2で追加 */
PubSubClient client(espClient);  /* Step2で追加 */

//Create an instance of the object
HTU21D myHumidity;

float getHTU21DHumidity()
{
  return myHumidity.readHumidity();
}

float getHTU21DTemperature()
{
  return myHumidity.readTemperature();
}

void setup() {
  Serial.begin(115200);  /* printf用 */
  setup_wifi();

  client.setServer(mqtt_server, 1883);   /* Step2で追加 */ /* MQTTブローカーを指定 */

  myHumidity.begin();
  Wire.begin(4, 14);   /* SDA(D2), SCL(D5) */
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

void reconnect() {
  while(!client.connected()) {
    if (client.connect(mqtt_client_id)) {  /* Step2で追加 */ /* MQTTブローカーに接続 */
      Serial.println("MQTT Connected");    /* Step2で追加 */
      client.setCallback(mqtt_callback);   /* Step4で追加 */ /* MQTTメッセージハンドラ登録 */
      client.subscribe(mqtt_sub_topic);    /* Step4で追加 */ /* subscribe */
    } else {
      delay(5000);
    }
  }
}

/* Step4で追加 */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String strBuff = "";

  Serial.print("Message arrived [" + String(topic) + "] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    strBuff += String((char)payload[i]);
  }
  Serial.println();

  if( strBuff.equalsIgnoreCase("temp") ) {
    strBuff = "Temperature: " + String(getHTU21DTemperature());
    Serial.println(strBuff);
    client.publish(mqtt_pub_temp, strBuff.c_str()); /* publish */
  } else if( strBuff.equalsIgnoreCase("hum") ) {
    strBuff = "Humidity: " + String(getHTU21DHumidity());
    Serial.println(strBuff);
    client.publish(mqtt_pub_hum, strBuff.c_str()); /* publish */
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    client.loop();  /* Step4で追加 */
  }
}
