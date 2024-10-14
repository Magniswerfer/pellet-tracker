# pellet-tracker
A repo for my pellet tracker.

## setup
Install pubsub client library for arduino.

Make a `env.h` and put:

`// Update these with your network and MQTT server information
const char* ssid = "your-ssid";        // Your Wi-Fi network name
const char* password = "your-wifi-password"; // Your Wi-Fi network password
const char* mqtt_server = "your-local-machine-ip";     // IP address of your MQTT broker
`

## local testing
I have installed the mosquitto package, e.g.,
`sudo apt install mosquitto mosquitto-clients`
