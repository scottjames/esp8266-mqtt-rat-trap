# esp8266-mqtt-rat-trap
IoT internet enabled rat snap-trap used to monitor mouse and rat traps remotely. using ESP8266, Arduino, MQTT / mosquitto.org

## IoT Rat Trap

* ESP8266 (ESP-12E WiFi SOC CPU)
* MQTT (mosquitto.org message queue broker software)
* MQTT Client (Raspberry Pi) --> IFTTT 'Maker' Channel

[ trap ] ---> [ ESP8266 ] ---> [ MQTT Broker ] ----> [ MQTT Client ] ---> email notification

## Trap
 - Standard snap-trap from hardware store, modified with switch sensor.
 - Use magnet + reed switch to detect position.
 - Connect to CPU GPIO pin as closed-loop alarm.

## ESP8266
 - ESP-12E WiFi System On Chip (SOC) for about $3
 - Program with Arduino IDE (version 1.6.8)
 - Read GPIO pin for switch position as "ready" or "sprung"
 - Publish MQTT messages on 'trap' topic.

## MQTT Broker
 - mosquitto.org open source software  MQTT broken, run on laptop or Raspberry Pi.
 - Listen for Topic event message, then re-transmitts message to client listeners on topic.

# Why?
 * Lazy...got tired of checking traps in attic and garage.
 * Forget...to check traps.
 * Excuse... to MAKE! Wanted to make IoT with ESP8266 and MQTT. Learn, Enjoy, Share!

#  What Next
 * Transmit battery level, easier to plan batter replacement *before* failure.
 * GPIO pin to trigger LEVEL interrupt (intead of polling) - to get immediate notification.
 * MQTT app on phone, tablet, or use AWS IoT.
