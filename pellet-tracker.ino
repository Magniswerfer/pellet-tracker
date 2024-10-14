#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>  // Include the ArduinoJson library
#include <env.h>  // Include your config header if you're using it

WiFiClient espClient;
PubSubClient client(espClient);

#define TRIG_PIN 12  // GPIO pin for the TRIG pin of the HC-SR04
#define ECHO_PIN 14  // GPIO pin for the ECHO pin of the HC-SR04 (with voltage divider)

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
  pinMode(TRIG_PIN, OUTPUT);  // Set TRIG_PIN as OUTPUT
  pinMode(ECHO_PIN, INPUT);   // Set ECHO_PIN as INPUT
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

float measure_distance(){
    // Send a 10us HIGH pulse to the TRIG_PIN to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the pulse duration on the ECHO_PIN
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance based on the duration
  // Speed of sound = 34300 cm/s, so distance (cm) = duration * 0.0343 / 2
  float distance = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Simulated sensor data (e.g., temperature and humidity)
  float distance = measure_distance();

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

  delay(1000);  // Delay before sending the next message
}