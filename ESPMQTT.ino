#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15

#define Vol_UP      D6
#define Vol_DOWN    D1
#define Canal_UP    D4
#define Canal_DOWN  D3

#define MQTT_ID  "CarlosLopes_RafaelPaulosi"
#define SUBSCRIBER_TOPIC "Sistemas.Embarcados.Topico.SANSUNG"

const char* SSID = "ASUS_Z00VD";
const char* PASSWORD = "6ebf8d86f103";

const char* MQTT_BROKER = "broker.mqtt-dashboard.com";
int BROKER_PORT = 1883;

WiFiClient WIFI;
PubSubClient MQTT(WIFI);

void setup() {
  pinMode(Vol_UP, INPUT);
  pinMode(Vol_DOWN, INPUT);
  pinMode(Canal_UP, INPUT);
  pinMode(Canal_DOWN, INPUT);

  Serial.begin(115200);
  
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("Conectando");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)  {
    Serial.print(".");
    delay(50);
  }

  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  MQTT.setServer(MQTT_BROKER, BROKER_PORT);
  MQTT.setCallback(callback);
}

void loop() {
  while (!MQTT.connected()) {
    Serial.print("Trying to connect MQTT: ");
    Serial.println(MQTT_BROKER);
    if (MQTT.connect(MQTT_ID)) {
      Serial.println("MQTT Connected!");
      MQTT.subscribe(SUBSCRIBER_TOPIC);
    } else {
      Serial.println("Connection Error.");
      Serial.println(MQTT.state());
      delay(1000);
    }
  }

  MQTT.loop();

  if (digitalRead(Vol_UP) == LOW) {
    MQTT.publish(SUBSCRIBER_TOPIC, "Vol_UP");
  }

  if (digitalRead(Vol_DOWN) == LOW) {
    MQTT.publish(SUBSCRIBER_TOPIC, "Vol_DOWN");
  }

  if (digitalRead(Canal_UP) == LOW) {
    MQTT.publish(SUBSCRIBER_TOPIC, "Canal_UP");
  }

  if (digitalRead(Canal_DOWN) == LOW) {
    MQTT.publish(SUBSCRIBER_TOPIC, "Canal_DOWN");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message :");
  Serial.println(message);
}
