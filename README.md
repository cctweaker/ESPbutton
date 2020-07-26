# ESPbutton
Swiss army knife of buttons.
Accepts momentary (one impulse sent) and rocker (impulse on each state change) buttons, active low or high. Can be battery or DC powered and communicates through ESP-Now or MQTT (local or SSL connection).

# Features
- encrypted ESP-Now connection or
- local MQTT connection or
- secure MQTT connection, Let's Ecrypt CA cetificate already loaded

- topic structure for multiple buildings (locations) control: <code>location/type/name/+</code>
- battery or DC powered
- button inputs are active low, active high or active on both (specially to replace rocker switches)

- all the settings can be changed from the ESP's webserver page even while not connected to any WiFi
- heartbeat function to send useful info periodically

# Hardware
Roll your own or get one from <a href="https://3dstar.ro/proiecte/espbutton">3DStar shop</a> in PCB, kit (PCB + components) or fully assembled form.

The ESP-01 / ESP-01S from AI Thinker is used to control one PCA9536 4bit IO expander for 4 inputs. The 4 buttons should cover most home use cases without problems. You can assign lights, shutters/blinds, doors/gates, irrigation or other controls.
The number of boards you can use is unlimited. The trick here is that when using ESP-Now, all button boards share the same MAC address. On MQTT there is no limitation, each board sends also its unique ID so even using a single topic for all buttons is feasible.

# Build & upload
There are 2 methods:
    - download, build & upload (PlatformIO is recommended for automatic library management)
    - download bin files, upload

# Libraries
- https://github.com/256dpi/arduino-mqtt
- https://github.com/bblanchon/ArduinoJson

# PCB example
<img src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESPbutton v1.1 top example.jpg?raw=true">
<img src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESPbutton v1.1 bottom example.jpg?raw=true">