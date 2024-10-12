#include <WiFi.h>
#include <PubSubClient.h>
#include <env.h>


WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) { // Client ID
      Serial.println("connected");
      client.publish("sensor/data", "ESP32 connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);  // Set the MQTT broker and port
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Simulated sensor data (replace with actual sensor reading)
  float sensorValue = random(20, 30);  // Simulated sensor value (temperature)
  String sensorData = String(sensorValue);
  
  // Publish sensor data to topic "sensor/data"
  client.publish("sensor/data", sensorData.c_str());
  Serial.println("Published sensor value: " + sensorData);

  delay(5000);  // Delay for 5 seconds before sending the next value
}
