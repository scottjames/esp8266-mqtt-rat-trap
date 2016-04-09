/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "RatTrapNet";
const char* password = "Mickey2016";
const char* mqtt_server = "192.168.10.200";

const char* label = "bedroom";  // TODO change label per emitter
const char* mqtt_topic = "trapStatus";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(500);
  // We start by connecting to a WiFi network
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// read trap status and report to server
void handleTrap(int pin) {
  
// read digital input on GPIO4 short to GND with resistor for LOW (return 0)
  int stat = (digitalRead(pin) == LOW) ? 0 : 1;

  // send label and status
  //snprintf (msg, 50, "[%d]:%s=%d", pin, label, stat);
  snprintf (msg, 50, "trap #%d = %d", pin, stat);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(mqtt_topic, msg); // publish to trapStatus/1

  delay(1000); // wait for msg to send

}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...      
      client.publish("trapStatus", "monitor ready");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      // rc=-2 means can not connect. Cn ubuntu, try: sudo ufw disable  (to disable firewall)
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  if (!client.connected()) {
    reconnect();
  }
    
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(4, INPUT_PULLUP);         // Initialize the GPIO 4 pin as an input
  pinMode(5, INPUT_PULLUP);         // Initialize the GPIO 5 pin as an input

  handleTrap(4);  // Read GPIO 4 pin, output trap status
  handleTrap(5);

  // Deep sleep for a while (device will reset itself to wake up)
  client.disconnect();
  delay(2000); // wait for wifi to finish before shutdown
  ESP.deepSleep(10 * 1e6, WAKE_RF_DEFAULT); // Sleep for 10 seconds, trigger RESET to wake up again (restart).
  
  // stop. no passing this point.  no loop()
}

void loop() { }



