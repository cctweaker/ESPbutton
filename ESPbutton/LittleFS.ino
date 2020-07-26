//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void load_config()
{
    if (!skip_espnow)
        if (load_espnow())
            if (start_espnow)
                prepare_espnow();

    //////////
    //////////
    //////////

    if (load_wifi())
        cfg_wifi = true;

    if (load_device())
        cfg_dev = true;

    if (load_mqtt())
        cfg_mqtt = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool load_device()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(dev_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(2048);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("LOC"))
        strlcpy(LOC, doc["LOC"], sizeof(LOC));
    if (doc.containsKey("TIP"))
        strlcpy(TIP, doc["TIP"], sizeof(TIP));
    if (doc.containsKey("NAME"))
        strlcpy(NAME, doc["NAME"], sizeof(NAME));
    if (doc.containsKey("HOSTNAME"))
        strlcpy(HOSTNAME, doc["HOSTNAME"], sizeof(HOSTNAME));

    if (doc.containsKey("update_url"))
        strlcpy(update_url, doc["update_url"], sizeof(update_url));

    if (doc.containsKey("heartbeat"))
        heartbeat = doc["heartbeat"];
    if (doc.containsKey("heartbeat_minutes"))
        heartbeat_minutes = doc["heartbeat_minutes"];

    if (doc.containsKey("start_webserver"))
        start_webserver = doc["start_webserver"];

    if (doc.containsKey("battery_powered"))
        battery_powered = doc["battery_powered"];

    if (doc.containsKey("button_type"))
        button_type = doc["button_type"];

    doc.clear();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool load_wifi()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(wifi_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(2048);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("SSIDa"))
        strlcpy(SSIDa, doc["SSIDa"], sizeof(SSIDa));
    if (doc.containsKey("PASSa"))
        strlcpy(PASSa, doc["PASSa"], sizeof(PASSa));

    if (doc.containsKey("SSIDb"))
        strlcpy(SSIDb, doc["SSIDb"], sizeof(SSIDb));
    if (doc.containsKey("PASSb"))
        strlcpy(PASSb, doc["PASSb"], sizeof(PASSb));

    if (doc.containsKey("start_wifi"))
        start_wifi = doc["start_wifi"];

    doc.clear();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool load_mqtt()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(mqtt_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(2048);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("MQTT_HOST"))
        strlcpy(MQTT_HOST, doc["MQTT_HOST"], sizeof(MQTT_HOST));
    if (doc.containsKey("MQTT_PORT"))
        MQTT_PORT = doc["MQTT_PORT"];
    if (doc.containsKey("MQTT_USER"))
        strlcpy(MQTT_USER, doc["MQTT_USER"], sizeof(MQTT_USER));
    if (doc.containsKey("MQTT_PASS"))
        strlcpy(MQTT_PASS, doc["MQTT_PASS"], sizeof(MQTT_PASS));

    if (doc.containsKey("WILL"))
        strlcpy(WILL, doc["WILL"], sizeof(WILL));
    if (doc.containsKey("PUB"))
        strlcpy(PUB, doc["PUB"], sizeof(PUB));
    if (doc.containsKey("SUB"))
        strlcpy(SUB, doc["SUB"], sizeof(SUB));
    if (doc.containsKey("STAT"))
        strlcpy(STAT, doc["STAT"], sizeof(STAT));

    if (doc.containsKey("start_mqtt"))
        start_mqtt = doc["start_mqtt"];

    doc.clear();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool load_espnow()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(espnow_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(2048);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("gmac"))
        strncpy((char *)gmac, doc["gmac"], sizeof(gmac));
    if (doc.containsKey("umac"))
        strncpy((char *)umac, doc["umac"], sizeof(umac));

    if (doc.containsKey("kok"))
        strncpy((char *)kok, doc["kok"], sizeof(kok));
    if (doc.containsKey("key"))
        strncpy((char *)key, doc["key"], sizeof(key));

    if (doc.containsKey("WIFI_CHANNEL"))
        WIFI_CHANNEL = doc["WIFI_CHANNEL"];
    if (doc.containsKey("HIDE_AP"))
        HIDE_AP = doc["HIDE_AP"];

    if (doc.containsKey("start_espnow"))
        start_espnow = doc["start_espnow"];
    if (doc.containsKey("try_espnow"))
        try_espnow = doc["try_espnow"];

    doc.clear();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const char *save_wifi()
{
    if (!LittleFS.begin())
        return "LittleFS failure!";

    DynamicJsonDocument doc(2048);

    doc["SSIDa"] = SSIDa;
    doc["PASSa"] = PASSa;
    doc["SSIDb"] = SSIDb;
    doc["PASSb"] = PASSb;

    doc["start_wifi"] = start_wifi;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(wifi_cfg_file, "w");
    if (!configFile)
        return "File write failure!";

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return "File write success.";
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const char *save_device()
{
    if (!LittleFS.begin())
        return "LittleFS failure!";

    DynamicJsonDocument doc(2048);

    doc["LOC"] = LOC;
    doc["TIP"] = TIP;
    doc["NAME"] = NAME;
    doc["HOSTNAME"] = HOSTNAME;
    doc["update_url"] = update_url;
    doc["heartbeat"] = heartbeat;
    doc["heartbeat_minutes"] = heartbeat_minutes;
    doc["start_webserver"] = start_webserver;
    doc["battery_powered"] = battery_powered;
    doc["button_type"] = button_type;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(dev_cfg_file, "w");
    if (!configFile)
        return "File write failure!";

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return "File write success.";
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const char *save_mqtt()
{
    if (!LittleFS.begin())
        return "LittleFS failure!";

    DynamicJsonDocument doc(2048);

    doc["MQTT_HOST"] = MQTT_HOST;
    doc["MQTT_PORT"] = MQTT_PORT;
    doc["MQTT_USER"] = MQTT_USER;
    doc["MQTT_PASS"] = MQTT_PASS;
    doc["WILL"] = WILL;
    doc["PUB"] = PUB;
    doc["SUB"] = SUB;
    doc["STAT"] = STAT;
    doc["start_mqtt"] = start_mqtt;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(mqtt_cfg_file, "w");
    if (!configFile)
        return "File write failure!";

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return "File write success.";
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const char *save_espnow()
{
    if (!LittleFS.begin())
        return "LittleFS failure!";

    DynamicJsonDocument doc(2048);

    doc["gmac"] = gmac;
    doc["umac"] = umac;

    doc["kok"] = kok;
    doc["key"] = key;

    doc["WIFI_CHANNEL"] = WIFI_CHANNEL;
    doc["HIDE_AP"] = HIDE_AP;

    doc["start_espnow"] = start_espnow;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(espnow_cfg_file, "w");
    if (!configFile)
        return "File write failure!";

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return "File write success.";
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void save_config()
{

    do_save_config = false;

    char MQTT_STAT_TOPIC[64];
    sprintf(MQTT_STAT_TOPIC, "%s/%s_%06x%s", LOC, TIP, ESP.getChipId(), STAT);

    if (!LittleFS.begin())
    {
        client.publish(MQTT_STAT_TOPIC, "LittleFS fatal error!", false, 0);
        return;
    }

    DynamicJsonDocument doc(2048);

    doc["LOC"] = LOC;
    doc["TIP"] = TIP;
    doc["NAME"] = NAME;

    doc["HOSTNAME"] = HOSTNAME;
    doc["update_url"] = update_url;

    doc["heartbeat"] = heartbeat;
    doc["heartbeat_minutes"] = heartbeat_minutes;

    doc["start_webserver"] = start_webserver;
    doc["battery_powered"] = battery_powered;
    doc["button_type"] = button_type;

    // serialize & write config
    File configFile = LittleFS.open(dev_cfg_file, "w");
    if (!configFile)
    {
        client.publish(MQTT_STAT_TOPIC, "device.json save error!", false, 0);
        return;
    }

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    doc["SSIDa"] = SSIDa;
    doc["PASSa"] = PASSa;
    doc["SSIDb"] = SSIDb;
    doc["PASSb"] = PASSb;

    // serialize & write config
    configFile = LittleFS.open(wifi_cfg_file, "w");
    if (!configFile)
    {
        client.publish(MQTT_STAT_TOPIC, "wifi.json save error!", false, 0);
        return;
    }

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    doc["MQTT_HOST"] = MQTT_HOST;
    doc["MQTT_PORT"] = MQTT_PORT;
    doc["MQTT_USER"] = MQTT_USER;
    doc["MQTT_PASS"] = MQTT_PASS;

    doc["WILL"] = WILL;
    doc["PUB"] = PUB;
    doc["SUB"] = SUB;
    doc["STAT"] = STAT;

    doc["start_mqtt"] = use_mqtt;

    // serialize & write config
    configFile = LittleFS.open(mqtt_cfg_file, "w");
    if (!configFile)
    {
        client.publish(MQTT_STAT_TOPIC, "mqtt.json save error!", false, 0);
        return;
    }

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    LittleFS.end();

    client.publish(MQTT_STAT_TOPIC, "Save complete.", false, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void format_fs()
{
    do_fs_format = false;

    char MQTT_STAT_TOPIC[64];
    sprintf(MQTT_STAT_TOPIC, "%s/%s_%06x%s", LOC, TIP, ESP.getChipId(), STAT);

    if (!LittleFS.begin())
    {
        client.publish(MQTT_STAT_TOPIC, "LittleFS failure!", false, 0);
        return;
    }

    LittleFS.format();
    LittleFS.end();

    client.publish(MQTT_STAT_TOPIC, "Format complete.", false, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void web_format_fs()
{

    if (!LittleFS.begin())
        return;

    LittleFS.format();
    LittleFS.end();

    return;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
