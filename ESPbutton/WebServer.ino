//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_webserver()
{
    if (!use_softap) // if not in SoftAP config mode check if server needs to start
    {
        if (!start_webserver && start_mqtt && use_mqtt) // Webserver can be OFF if MQTT is ON
            return;
    }

    use_webserver = true;

    server.on("/", page_main);
    server.on("/generate_204", page_main);
    server.on("/fwlink", page_main);
    server.on("/wifi", cfg_page_wifi);
    server.on("/device", cfg_page_device);
    server.on("/mqtt", cfg_page_mqtt);
    server.on("/espnow", cfg_page_espnow);
    server.on("/rtcram", cfg_page_rtcram);
    server.on("/sysinfo", cfg_page_sysinfo);

    server.on("/start_espnow", []() {
        page_main();
        delay(300);
        prepare_espnow();
    });

    server.on("/try_espnow", []() {
        page_main();
        delay(300);
        try_espnow = true;
        prepare_espnow();
    });

    server.on("/update", []() {
        server.send_P(200, "text/html", update);
        delay(200);
        ota_update();
    });

    server.on("/format", []() {
        server.send_P(200, "text/html", format);
        web_format_fs();
    });

    server.on("/restart", []() {
        server.send_P(200, "text/html", restart);
        delay(200);
        ESP.restart();
    });

    server.onNotFound([]() {
        server.send_P(404, "text/html", notfound);
    });

    server.begin();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ##    ###    #### ##    ##    ########     ###     ######   ########
// ###   ###   ## ##    ##  ###   ##    ##     ##   ## ##   ##    ##  ##
// #### ####  ##   ##   ##  ####  ##    ##     ##  ##   ##  ##        ##
// ## ### ## ##     ##  ##  ## ## ##    ########  ##     ## ##   #### ######
// ##     ## #########  ##  ##  ####    ##        ######### ##    ##  ##
// ##     ## ##     ##  ##  ##   ###    ##        ##     ## ##    ##  ##
// ##     ## ##     ## #### ##    ##    ##        ##     ##  ######   ########

void page_main()
{
    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(200, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return;
    }

    server.sendContent_P(page_header);

    server.sendContent(F("<h2>ESPbutton<br>configuration</h2><br>"));

    char buffer[256];

    sprintf_P(buffer, menu_entry, "/device", "Device", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/wifi", "WiFi", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/mqtt", "MQTT", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/espnow", "ESP-Now", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/rtcram", "Show RTC ram", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/update", "Update firmware", menu_entry_check_update);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/format", "Erase settings", menu_entry_check_erase);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/restart", "Restart", menu_entry_check_restart);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/sysinfo", "System information", "");
    server.sendContent(buffer);

    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##      ## #### ######## ####
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ######    ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
//  ###  ###  #### ##       ####

void cfg_page_wifi()
{
    if (server.hasArg("SSIDa"))
        strlcpy(SSIDa, server.arg("SSIDa").c_str(), sizeof(SSIDa));

    if (server.hasArg("PASSa"))
        strlcpy(PASSa, server.arg("PASSa").c_str(), sizeof(PASSa));

    if (server.hasArg("SSIDb"))
        strlcpy(SSIDb, server.arg("SSIDb").c_str(), sizeof(SSIDb));

    if (server.hasArg("PASSb"))
        strlcpy(PASSb, server.arg("PASSb").c_str(), sizeof(PASSb));

    if (server.hasArg("start_wifi"))
        start_wifi = server.arg("start_wifi").toInt();

    if (start_mqtt) // check for mqtt start, we need wifi!
        start_wifi = true;

    ///////////
    yield();
    ///////////

    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(200, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return;
    }

    server.sendContent_P(page_header);

    int i = 0;
    int n = WiFi.scanComplete();

    if (n >= 0)
    {
        server.sendContent_P(form_start);
        for (i = 0; i < n; i++)
        {
            server.sendContent(F("<input type='submit' name='SSIDa' value='"));
            server.sendContent(String() + WiFi.SSID(i) + "'> - ch." + WiFi.channel(i) + " (" + WiFi.RSSI(i) + " dBm)<br>");
        }
        server.sendContent_P(form_end);
        server.sendContent_P(html_hr);
    }

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "SSIDa", "SSIDa", SSIDa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASSa", "PASSa", PASSa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "SSIDb", "SSIDb", SSIDb);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASSb", "PASSb", PASSb);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Start WiFi?", "start_wifi", start_wifi ? "selected" : "", start_wifi ? "" : "selected");
    server.sendContent(buffer);

    // sprintf_P(buffer, form_text_field_int_min, "Start WiFi?", "start_wifi", start_wifi);
    // server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_wifi());
    }

    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ######## ##     ## ####  ######  ########
// ##     ## ##       ##     ##  ##  ##    ## ##
// ##     ## ##       ##     ##  ##  ##       ##
// ##     ## ######   ##     ##  ##  ##       ######
// ##     ## ##        ##   ##   ##  ##       ##
// ##     ## ##         ## ##    ##  ##    ## ##
// ########  ########    ###    ####  ######  ########

void cfg_page_device()
{
    if (server.hasArg("LOC"))
        strlcpy(LOC, server.arg("LOC").c_str(), sizeof(LOC));

    if (server.hasArg("TIP"))
        strlcpy(TIP, server.arg("TIP").c_str(), sizeof(TIP));

    if (server.hasArg("NAME"))
        strlcpy(NAME, server.arg("NAME").c_str(), sizeof(NAME));

    if (server.hasArg("HOSTNAME"))
        strlcpy(HOSTNAME, server.arg("HOSTNAME").c_str(), sizeof(HOSTNAME));

    if (server.hasArg("update_url"))
        strlcpy(update_url, server.arg("update_url").c_str(), sizeof(update_url));

    if (server.hasArg("heartbeat"))
        heartbeat = server.arg("heartbeat").toInt();

    if (server.hasArg("heartbeat_minutes"))
        heartbeat_minutes = server.arg("heartbeat_minutes").toInt();

    if (server.hasArg("start_webserver"))
        start_webserver = server.arg("start_webserver").toInt();

    if (server.hasArg("battery_powered"))
        battery_powered = server.arg("battery_powered").toInt();

    if (server.hasArg("button_type"))
        button_type = server.arg("button_type").toInt();

    ///////////
    yield();
    ///////////

    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(200, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return;
    }

    server.sendContent_P(page_header);

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string_min, "LOC", "LOC", LOC);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, "TIP", "TIP", TIP);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, "NAME", "NAME", NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, "HOSTNAME (_ver_abcdef)", "HOSTNAME", HOSTNAME);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "Update URL", "update_url", update_url);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, "Heartbeat", "heartbeat", heartbeat ? "selected" : "", heartbeat ? "" : "selected");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_int_min, "Heartbeat minutes", "heartbeat_minutes", heartbeat_minutes);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, "Start webserver", "start_webserver", start_webserver ? "selected" : "", start_webserver ? "" : "selected");
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, "Battery powered", "battery_powered", battery_powered ? "selected" : "", battery_powered ? "" : "selected");
    server.sendContent(buffer);

    sprintf_P(buffer, form_select_field, "Button type", "button_type");
    server.sendContent(buffer);

    if (0 == button_type)
        sprintf_P(buffer, form_option_field_selected, 0, "active low, momentary");
    else
        sprintf_P(buffer, form_option_field, 0, "active low, momentary");
    server.sendContent(buffer);

    if (1 == button_type)
        sprintf_P(buffer, form_option_field_selected, 1, "active high, momentary (batt)");
    else
        sprintf_P(buffer, form_option_field, 1, "active high, momentary (batt)");
    server.sendContent(buffer);

    if (2 == button_type)
        sprintf_P(buffer, form_option_field_selected, 2, "active low/high, rocker switch");
    else
        sprintf_P(buffer, form_option_field, 2, "active low/high, rocker switch");
    server.sendContent(buffer);

    server.sendContent_P(form_select_field_end);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_device());
    }

    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ##  #######  ######## ########
// ###   ### ##     ##    ##       ##
// #### #### ##     ##    ##       ##
// ## ### ## ##     ##    ##       ##
// ##     ## ##  ## ##    ##       ##
// ##     ## ##    ##     ##       ##
// ##     ##  ##### ##    ##       ##

void cfg_page_mqtt()
{
    if (server.hasArg("MQTT_HOST"))
        strlcpy(MQTT_HOST, server.arg("MQTT_HOST").c_str(), sizeof(MQTT_HOST));

    if (server.hasArg("MQTT_PORT"))
        MQTT_PORT = server.arg("MQTT_PORT").toInt();

    if (server.hasArg("MQTT_USER"))
        strlcpy(MQTT_USER, server.arg("MQTT_USER").c_str(), sizeof(MQTT_USER));

    if (server.hasArg("MQTT_PASS"))
        strlcpy(MQTT_PASS, server.arg("MQTT_PASS").c_str(), sizeof(MQTT_PASS));

    if (server.hasArg("WILL"))
        strlcpy(WILL, server.arg("WILL").c_str(), sizeof(WILL));

    if (server.hasArg("PUB"))
        strlcpy(PUB, server.arg("PUB").c_str(), sizeof(PUB));

    if (server.hasArg("SUB"))
        strlcpy(SUB, server.arg("SUB").c_str(), sizeof(SUB));

    if (server.hasArg("STAT"))
        strlcpy(STAT, server.arg("STAT").c_str(), sizeof(STAT));

    if (server.hasArg("start_mqtt"))
    {
        start_mqtt = server.arg("start_mqtt").toInt();

        if (!start_wifi) // needs wifi to work
            start_mqtt = false;
    }

    ///////////
    yield();
    ///////////

    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(200, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return;
    }

    server.sendContent_P(page_header);

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "MQTT host/ip", "MQTT_HOST", MQTT_HOST);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_int, "MQTT port", "MQTT_PORT", MQTT_PORT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "MQTT user", "MQTT_USER", MQTT_USER);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "MQTT password", "MQTT_PASS", MQTT_PASS);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "WILL subtopic", "WILL", WILL);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "PUB subtopic", "PUB", PUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "SUB subtopic", "SUB", SUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "STAT subtopic", "STAT", STAT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Start MQTT<br>(needs WiFi enabled)", "start_mqtt", start_mqtt ? "selected" : "", start_mqtt ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_mqtt());
    }

    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ######  ########          ##    ##  #######  ##      ##
// ##       ##    ## ##     ##         ###   ## ##     ## ##  ##  ##
// ##       ##       ##     ##         ####  ## ##     ## ##  ##  ##
// ######    ######  ########  ####### ## ## ## ##     ## ##  ##  ##
// ##             ## ##                ##  #### ##     ## ##  ##  ##
// ##       ##    ## ##                ##   ### ##     ## ##  ##  ##
// ########  ######  ##                ##    ##  #######   ###  ###

void cfg_page_espnow()
{
    if (server.hasArg("gmac0"))
        gmac[0] = strtoul(server.arg("gmac0").c_str(), 0, 16);
    if (server.hasArg("gmac1"))
        gmac[1] = strtoul(server.arg("gmac1").c_str(), 0, 16);
    if (server.hasArg("gmac2"))
        gmac[2] = strtoul(server.arg("gmac2").c_str(), 0, 16);
    if (server.hasArg("gmac3"))
        gmac[3] = strtoul(server.arg("gmac3").c_str(), 0, 16);
    if (server.hasArg("gmac4"))
        gmac[4] = strtoul(server.arg("gmac4").c_str(), 0, 16);
    if (server.hasArg("gmac5"))
        gmac[5] = strtoul(server.arg("gmac5").c_str(), 0, 16);

    ///////////
    yield();
    ///////////

    if (server.hasArg("umac0"))
        umac[0] = strtoul(server.arg("umac0").c_str(), 0, 16);
    if (server.hasArg("umac1"))
        umac[1] = strtoul(server.arg("umac1").c_str(), 0, 16);
    if (server.hasArg("umac2"))
        umac[2] = strtoul(server.arg("umac2").c_str(), 0, 16);
    if (server.hasArg("umac3"))
        umac[3] = strtoul(server.arg("umac3").c_str(), 0, 16);
    if (server.hasArg("umac4"))
        umac[4] = strtoul(server.arg("umac4").c_str(), 0, 16);
    if (server.hasArg("umac5"))
        umac[5] = strtoul(server.arg("umac5").c_str(), 0, 16);

    ///////////
    yield();
    ///////////

    if (server.hasArg("kok0"))
        kok[0] = strtoul(server.arg("kok0").c_str(), 0, 16);
    if (server.hasArg("kok1"))
        kok[1] = strtoul(server.arg("kok1").c_str(), 0, 16);
    if (server.hasArg("kok2"))
        kok[2] = strtoul(server.arg("kok2").c_str(), 0, 16);
    if (server.hasArg("kok3"))
        kok[3] = strtoul(server.arg("kok3").c_str(), 0, 16);
    if (server.hasArg("kok4"))
        kok[4] = strtoul(server.arg("kok4").c_str(), 0, 16);
    if (server.hasArg("kok5"))
        kok[5] = strtoul(server.arg("kok5").c_str(), 0, 16);
    if (server.hasArg("kok6"))
        kok[6] = strtoul(server.arg("kok6").c_str(), 0, 16);
    if (server.hasArg("kok7"))
        kok[7] = strtoul(server.arg("kok7").c_str(), 0, 16);
    if (server.hasArg("kok8"))
        kok[8] = strtoul(server.arg("kok8").c_str(), 0, 16);
    if (server.hasArg("kok9"))
        kok[9] = strtoul(server.arg("kok9").c_str(), 0, 16);
    if (server.hasArg("kok10"))
        kok[10] = strtoul(server.arg("kok10").c_str(), 0, 16);
    if (server.hasArg("kok11"))
        kok[11] = strtoul(server.arg("kok11").c_str(), 0, 16);
    if (server.hasArg("kok12"))
        kok[12] = strtoul(server.arg("kok12").c_str(), 0, 16);
    if (server.hasArg("kok13"))
        kok[13] = strtoul(server.arg("kok13").c_str(), 0, 16);
    if (server.hasArg("kok14"))
        kok[14] = strtoul(server.arg("kok14").c_str(), 0, 16);
    if (server.hasArg("kok15"))
        kok[15] = strtoul(server.arg("kok15").c_str(), 0, 16);

    ///////////
    yield();
    ///////////

    if (server.hasArg("key0"))
        key[0] = strtoul(server.arg("key0").c_str(), 0, 16);
    if (server.hasArg("key1"))
        key[1] = strtoul(server.arg("key1").c_str(), 0, 16);
    if (server.hasArg("key2"))
        key[2] = strtoul(server.arg("key2").c_str(), 0, 16);
    if (server.hasArg("key3"))
        key[3] = strtoul(server.arg("key3").c_str(), 0, 16);
    if (server.hasArg("key4"))
        key[4] = strtoul(server.arg("key4").c_str(), 0, 16);
    if (server.hasArg("key5"))
        key[5] = strtoul(server.arg("key5").c_str(), 0, 16);
    if (server.hasArg("key6"))
        key[6] = strtoul(server.arg("key6").c_str(), 0, 16);
    if (server.hasArg("key7"))
        key[7] = strtoul(server.arg("key7").c_str(), 0, 16);
    if (server.hasArg("key8"))
        key[8] = strtoul(server.arg("key8").c_str(), 0, 16);
    if (server.hasArg("key9"))
        key[9] = strtoul(server.arg("key9").c_str(), 0, 16);
    if (server.hasArg("key10"))
        key[10] = strtoul(server.arg("key10").c_str(), 0, 16);
    if (server.hasArg("key11"))
        key[11] = strtoul(server.arg("key11").c_str(), 0, 16);
    if (server.hasArg("key12"))
        key[12] = strtoul(server.arg("key12").c_str(), 0, 16);
    if (server.hasArg("key13"))
        key[13] = strtoul(server.arg("key13").c_str(), 0, 16);
    if (server.hasArg("key14"))
        key[14] = strtoul(server.arg("key14").c_str(), 0, 16);
    if (server.hasArg("key15"))
        key[15] = strtoul(server.arg("key15").c_str(), 0, 16);

    ///////////
    yield();
    ///////////

    if (server.hasArg("WIFI_CHANNEL"))
        WIFI_CHANNEL = server.arg("WIFI_CHANNEL").toInt();

    if (server.hasArg("HIDE_AP"))
        HIDE_AP = server.arg("HIDE_AP").toInt();

    if (server.hasArg("start_espnow"))
        start_espnow = server.arg("start_espnow").toInt();

    ///////////
    yield();
    ///////////

    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(200, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return;
    }

    server.sendContent_P(page_header_mac);

    char buffer[256];
    uint8_t i;

    server.sendContent_P(form_start);

    server.sendContent(F("Gateway MAC (hex)<br>"));
    for (i = 0; i < 6; i++)
    {
        sprintf_P(buffer, form_gmac_field, i, gmac[i], (i < 5 ? ":" : "<br><br>"));
        server.sendContent(buffer);
    }

    sprintf(buffer, "this units MAC (hex)<br>last byte must be between<br><b>%02X and %02X</b><br>", gmac[5] + 1, gmac[5] + 6);
    server.sendContent(buffer);
    for (i = 0; i < 6; i++)
    {
        sprintf_P(buffer, form_umac_field, i, umac[i], (i < 5 ? ":" : "<br><br>"));
        server.sendContent(buffer);
    }

    server.sendContent(F("Primary master key (KOK)<br>"));
    for (i = 0; i < 16; i++)
    {
        sprintf_P(buffer, form_kok_field, i, kok[i]);
        server.sendContent(buffer);
    }
    server.sendContent("<br><br>");

    server.sendContent(F("Payload key (KEY)<br>"));
    for (i = 0; i < 16; i++)
    {
        sprintf_P(buffer, form_key_field, i, key[i]);
        server.sendContent(buffer);
    }
    server.sendContent("<br><br>");

    sprintf_P(buffer, form_text_field_int_min, "WiFi channel", "WIFI_CHANNEL", WIFI_CHANNEL);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Hide ESP-Now AP", "HIDE_AP", HIDE_AP ? "selected" : "", HIDE_AP ? "" : "selected");
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Start ESP-Now permanently<br>(also after power loss)", "start_espnow", start_espnow ? "selected" : "", start_espnow ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    server.sendContent_P(html_hr);

    sprintf_P(buffer, menu_entry, "/start_espnow", "Start ESP-Now (temporary)", "");
    server.sendContent(buffer);

    sprintf_P(buffer, menu_entry, "/try_espnow", "Try ESP-Now (sends 1 message)", "");
    server.sendContent(buffer);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_espnow());
    }

    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ########  ######        ########     ###    ##     ##
// ##     ##    ##    ##    ##       ##     ##   ## ##   ###   ###
// ##     ##    ##    ##             ##     ##  ##   ##  #### ####
// ########     ##    ##             ########  ##     ## ## ### ##
// ##   ##      ##    ##             ##   ##   ######### ##     ##
// ##    ##     ##    ##    ##       ##    ##  ##     ## ##     ##
// ##     ##    ##     ######        ##     ## ##     ## ##     ##

void cfg_page_rtcram()
{
    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(200, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return;
    }

    server.sendContent_P(page_header);

    char buffer[512];
    char caracter[6];

    ESP.rtcUserMemoryRead(0, (uint32_t *)buffer, sizeof(buffer));

    server.sendContent(F("<pre>"));

    for (uint8_t row = 1; row <= 16; row++)
    {
        for (uint8_t col = 0; col < 32; col++)
        {
            sprintf(caracter, "%02X ", buffer[col + row * 32 - 32]);
            server.sendContent(caracter);
            if (col == 15)
                server.sendContent(" - ");
        }
        server.sendContent("\n");
    }

    server.sendContent(F("</pre>"));

    server.sendContent(F("<pre>"));

    for (uint8_t row = 1; row <= 16; row++)
    {
        for (uint8_t col = 0; col < 32; col++)
        {
            if (buffer[col + row * 32 - 32] > 0)
            {
                sprintf(caracter, "%c", buffer[col + row * 32 - 32]);
                server.sendContent(caracter);
            }
            else
            {
                server.sendContent(" ");
            }
            if (col == 15)
                server.sendContent(" -  ");
        }
        server.sendContent("\n");
    }

    server.sendContent(F("</pre>"));

    server.sendContent(F("<br><br><form action='/'><input type='submit' value='Main menu'></form>"));
    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##    ##  ######  #### ##    ## ########  #######
// ##    ##  ##  ##  ##    ##  ##  ###   ## ##       ##     ##
// ##         ####   ##        ##  ####  ## ##       ##     ##
//  ######     ##     ######   ##  ## ## ## ######   ##     ##
//       ##    ##          ##  ##  ##  #### ##       ##     ##
// ##    ##    ##    ##    ##  ##  ##   ### ##       ##     ##
//  ######     ##     ######  #### ##    ## ##        #######

void cfg_page_sysinfo()
{
    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(200, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return;
    }

    char buffer[256] = {0};

    server.sendContent_P(page_header);
    server.sendContent_P(html_table_i);

    ////////////////////////

    sprintf_P(buffer, PSTR("<td>FW name</td><td>%s</td>"), FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW version</td><td>%.2f</td>"), VERSION);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW size</td><td>%.2f KB</td>"), ESP.getSketchSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>free flash size</td><td>%.2f KB</td>"), ESP.getFreeSketchSpace() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW MD5</td><td>%s</td>"), ESP.getSketchMD5().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW CRC</td><td>%s</td>"), ESP.checkFlashCRC() ? "OK" : "ERROR!!!");
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Core version</td><td>%s</td>"), ESP.getCoreVersion().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>SDK version</td><td>%s</td>"), ESP.getSdkVersion());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, PSTR("<td>Reset reason</td><td>%s</td>"), ESP.getResetReason().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Free Heap</td><td>%.2f KB</td>"), ESP.getFreeHeap() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Max Free Block</td><td>%.2f KB</td>"), ESP.getMaxFreeBlockSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Heap Fragmentation</td><td>%d%%</td>"), ESP.getHeapFragmentation());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, PSTR("<td>ESP Chip ID</td><td>%08X</td>"), ESP.getChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>CPU frequency</td><td>%d MHz</td>"), ESP.getCpuFreqMHz());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash Chip ID</td><td>%08X</td>"), ESP.getFlashChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash frequency</td><td>%d MHz</td>"), ESP.getFlashChipSpeed() / 1000000);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Used flash size</td><td>%d KB</td>"), ESP.getFlashChipSize()/1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash size</td><td>%d KB</td>"), ESP.getFlashChipRealSize()/1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////

    server.sendContent_P(html_table_s);

    server.sendContent(F("<br><br><form action='/'><input type='submit' value='Main menu'></form>"));
    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
