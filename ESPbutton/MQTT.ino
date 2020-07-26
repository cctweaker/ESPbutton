//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_mqtt()
{
    if (!start_mqtt || !use_wifi) // don't start mqtt
        return;

    char TOPIC[128];
    sprintf(TOPIC, "%s/%s/%s%s", LOC, TIP, NAME, WILL);

    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.setWill(TOPIC, "0", true, 0);
    client.onMessage(messageReceived);
    mqtt_connect();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void mqtt_connect()
{
    uint8_t i = 0;

#ifdef USE_SSL
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);
#endif

    while (!client.connect(HOSTNAME, MQTT_USER, MQTT_PASS))
    {
        delay(100);
        i++;
        if (i > 200)
        {
            client.disconnect();
            return;
            // ESP.restart();
        }
    }
    char TOPIC[128];

    sprintf(TOPIC, "%s/%s/%s%s", LOC, TIP, NAME, SUB);
    client.subscribe(TOPIC, 0);

    sprintf(TOPIC, "%s/%s/%s%s", LOC, TIP, NAME, WILL);
    client.publish(TOPIC, "1", true, 0);

    use_mqtt = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void messageReceived(String &topic, String &payload)
{
    uint8_t idx = topic.lastIndexOf('/') + 1;
    String cmnd = topic.substring(idx);
    uint8_t value = payload.toInt();

    if (cmnd == "reset")
    {
        ESP.restart();
    }

    if (cmnd == "erase")
    {
        do_fs_format = true;
        return;
    }

    if (cmnd == "webserver")
    {
        if (value == 0)
        {
            use_webserver = false;
            save_device();
        }
        else
        {
            use_webserver = true;
            save_device();
        }
        return;
    }

    if (cmnd == "update")
    {
        do_ota_update = true;
        return;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void mqtt_heartbeat()
{
    char topic[128];
    char tx[256];

    // sprintf(tx, "{\"l\":\"%s\",\"t\":\"%s\",\"n\":\"%s\",\"fw\":\"%s_v%.2f,\"ID\":\"%06x\",\"vcc\":%d,\"SSID\":\"%s\",\"RSSI\":%d,\"BSSID\":\"%s\"}", LOC, TIP, NAME, FW_NAME, VERSION, ESP.getChipId(), ESP.getVcc(), WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.BSSIDstr().c_str());

    DynamicJsonDocument doc(256);

    doc["l"] = LOC;
    doc["t"] = TIP;
    doc["n"] = NAME;
#ifndef USE_SSL
    doc["fw"] = FW_NAME;
#else
    doc["fw"] = String(FW_NAME) + "_SSL";
#endif
    doc["fv"] = VERSION;
    doc["ID"] = String(ESP.getChipId(), HEX);
    doc["vcc"] = ESP.getVcc();
    doc["SSID"] = WiFi.SSID();
    doc["RSSI"] = WiFi.RSSI();
    doc["BSSID"] = WiFi.BSSIDstr();

    serializeJson(doc, tx);
    doc.clear();
    sprintf(topic, "%s/%s/%s%s", LOC, TIP, NAME, STAT);
    client.publish(topic, tx, true, 0);

    doc["free heap"] = ESP.getFreeHeap();
    doc["heap frag"] = ESP.getHeapFragmentation();
    doc["max block"] = ESP.getMaxFreeBlockSize();
    doc["core v"] = ESP.getCoreVersion();
    doc["sdk v"] = ESP.getSdkVersion();
    doc["sketch"] = ESP.getSketchSize();
    doc["sketch free"] = ESP.getFreeSketchSpace();
    doc["MD5"] = ESP.getSketchMD5();
    doc["crc"] = ESP.checkFlashCRC();

    serializeJson(doc, tx);
    doc.clear();
    sprintf(topic, "%s/%s/%s%s/%d", LOC, TIP, NAME, STAT, 1);
    client.publish(topic, tx, true, 0);

    doc["CPU freq"] = ESP.getCpuFreqMHz();
    doc["flash freq"] = ESP.getFlashChipSpeed();
    doc["flash ID"] = String(ESP.getFlashChipId(), HEX);
    doc["flash size"] = ESP.getFlashChipSize();
    doc["real flash size"] = ESP.getFlashChipRealSize();
    doc["CPU cycles"] = ESP.getCycleCount();

    serializeJson(doc, tx);
    doc.clear();
    sprintf(topic, "%s/%s/%s%s/%d", LOC, TIP, NAME, STAT, 2);
    client.publish(topic, tx, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void mqtt_send()
{
    char topic[128];
    char tx[256];

    sprintf(topic, "%s/%s/%s%s", LOC, TIP, NAME, PUB);

    sprintf(tx, "{\"b\":[%d,%d,%d,%d],\"vcc\":%d,\"ID\":\"%06x\"}", bitRead(xb, 0), bitRead(xb, 1), bitRead(xb, 2), bitRead(xb, 3), ESP.getVcc(), ESP.getChipId());

    client.publish(topic, tx, false, 0);

    SENT = true;
    ACK = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////