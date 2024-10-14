#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>  // Include the ArduinoJson library
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
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
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
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Simulated sensor data (e.g., temperature and humidity)
  float distance = random(10, 50);  // Simulate distance

  // Create a JSON object
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["sensor"] = "ESP32";
  jsonDoc["distance"] = distance;

  // Serialize the JSON object to a string
  char jsonBuffer[512];
  serializeJson(jsonDoc, jsonBuffer);

  // Publish the JSON string to the MQTT topic
  client.publish("sensor/data", jsonBuffer);
  Serial.println("Published JSON: ");
  Serial.println(jsonBuffer);

  delay(5000);  // Delay before sending the next message
}