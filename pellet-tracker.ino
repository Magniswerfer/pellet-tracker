#include <WiFi.h>
#include <PubSubClient.h>
#include <env.h>  // Include your config header if you're using it

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
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect with username and password
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an initial message
      client.publish("pellet-tracker/sensor", "ESP32 connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Simulated sensor data (replace with actual sensor reading)
  float sensorValue = random(20, 30);  // Simulated sensor value (e.g., temperature)
  String sensorData = String(sensorValue);

  // Publish sensor data to topic "sensor/data"
  client.publish("pellet-tracker/sensor", sensorData.c_str());
  Serial.println("Published sensor value: " + sensorData);

  delay(5000);  // Delay before sending the next value
}
