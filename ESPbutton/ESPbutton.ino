ADC_MODE(ADC_VCC);
#include <ESP8266WiFi.h>
extern "C"
{
#include <espnow.h>
#include <user_interface.h>
}
#include <Wire.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <LittleFS.h>
#include <ESP8266WebServer.h>
#include <MQTTClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <time.h>

///////////////////////////////////////////
// for Arduino IDE only
// turn on/off SSL for MQTT and updates
// PlatformIO builds both versions
///////////////////////////////////////////
// #define USE_SSL

#include "variables.h"
#include "webpages.h"

//////////
//////////
//////////

#ifndef USE_SSL
WiFiClient net;
#else
BearSSL::WiFiClientSecure net;
time_t now;
#endif
MQTTClient client(256);
ESP8266WebServer server(80);
DNSServer dnsServer;

//////////
//////////
//////////

void initVariant() // needed for ESP-Now init
{
  WiFi.persistent(false);

  uint8_t rtc[512];
  uint8_t i;
  ESP.rtcUserMemoryRead(0, (uint32_t *)rtc, sizeof(rtc));

  if (strncmp((char *)rtc, "ESPnow  ", 6) == 0)
  {
    start_espnow = true;

    for (i = 0; i < sizeof(umac); i++)
      umac[i] = rtc[32 + i];

    for (i = 0; i < sizeof(NOW_SSID); i++)
      NOW_SSID[i] = rtc[64 + i];

    for (i = 0; i < sizeof(NOW_PASS); i++)
      NOW_PASS[i] = rtc[96 + i];

    WIFI_CHANNEL = rtc[128];
    HIDE_AP = rtc[129];
    try_espnow = rtc[130];

    for (i = 0; i < sizeof(gmac); i++)
      gmac[i] = rtc[160 + i];

    for (i = 0; i < sizeof(kok); i++)
      kok[i] = rtc[192 + i];

    for (i = 0; i < sizeof(key); i++)
      key[i] = rtc[224 + i];

    WiFi.mode(WIFI_AP);
    wifi_set_macaddr(SOFTAP_IF, umac);

    char NOWSSID[33];
    sprintf(NOWSSID, "%s_Now_%06x", NOW_SSID, ESP.getChipId());

    WiFi.softAP(NOWSSID, NOW_PASS, WIFI_CHANNEL, HIDE_AP);
  }
}

//////////
//////////
//////////

void setup()
{
  init_espnow(); // setup ESP-Now
  init_io();     // initialize PCA9536

  if (!use_espnow)
  {
    load_config();    // load config data
    init_wifi();      // initialize wifi
    init_softap();    // initialize softap for configuration
    init_dns();       // initialize dns for softap
    init_mqtt();      // setup & connect mqtt
    init_webserver(); // initialize webserver
  }
}

void loop()
{
  if (PCA && SENT) // if PCA detected and last message is sent then check button states
    check_buttons();

  if (use_espnow)
  {
    // if (!SENT && ACK) // maybe overkill
    if (!SENT)
      espnow_send();
  }
  else
  {
    if (use_dns) // if dns server is started
      dnsServer.processNextRequest();

    if (use_webserver) // if webserver is used
      server.handleClient();

    if (use_mqtt)
    {
      if (!client.connected()) // restart if mqtt not connected
        ESP.restart();

      client.loop();

      if (!SENT) // if button change detected then send mqtt message
        mqtt_send();
    }
  }

  if (heartbeat)
  {
    unsigned long heartbeat_period = heartbeat_minutes * 60 * 1000;

    if ((unsigned long)(millis() - last_heartbeat) > heartbeat_period)
    {
      last_heartbeat = millis();
      if (use_mqtt)
        mqtt_heartbeat();
      if (use_espnow)
        espnow_heartbeat();
    }
  }

  delay(10);
}
