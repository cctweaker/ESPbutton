//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_espnow()
{
    if (!start_espnow)
        return;

    skip_espnow = true;

    if (esp_now_init() != 0)
    {
        char rtc[512] = "ESP-Now init fail!";
        ESP.rtcUserMemoryWrite(0, (uint32_t *)rtc, sizeof(rtc));
        return;
    }

    use_espnow = true;

    //////////

    esp_now_set_kok(kok, 16);

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    esp_now_add_peer(gmac, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, key, 16);
    esp_now_set_peer_key(gmac, key, 16);

    esp_now_register_send_cb(espnow_tx_cb);
    esp_now_register_recv_cb(espnow_rx_cb);

    //////////

    if (try_espnow)
    {
        espnow_heartbeat();
        char rtc[512] = "ESP-Now test message sent.";
        ESP.rtcUserMemoryWrite(0, (uint32_t *)rtc, sizeof(rtc));
        delay(1000);
        ESP.restart();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void espnow_tx_cb(uint8_t *mac, uint8_t sendStatus)
{
    if (sendStatus == 0)
        ACK = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void espnow_rx_cb(uint8_t *senderMac, uint8_t *incomingData, uint8_t len)
{
    if (strncmp((char *)incomingData, "stop", 4) == 0)
    {
        start_espnow = false;
        espnow_send_buffer(save_espnow());
        ESP.restart();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void espnow_send()
{
    char tx[250];
    sprintf(tx, "{\"t\":\"%s\",\"n\":\"%s\",\"ID\":\"%06x\",\"b\":[%d,%d,%d,%d],\"vcc\":%d}%c", TIP, NAME, ESP.getChipId(), bitRead(xb, 0), bitRead(xb, 1), bitRead(xb, 2), bitRead(xb, 3), ESP.getVcc(), 0);
    espnow_send_buffer(tx);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void espnow_heartbeat()
{
    char tx[250];
    sprintf(tx, "{\"t\":\"%s\",\"n\":\"%s\",\"fw\":\"%s_v%.2f,\"ID\":\"%06x\",\"vcc\":%d}%c", TIP, NAME, FW_NAME, VERSION, ESP.getChipId(), ESP.getVcc(), 0);
    espnow_send_buffer(tx);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void espnow_send_buffer(const char *tx)
{
    esp_now_send(gmac, (byte *)tx, strlen(tx));

    SENT = true;
    ACK = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void prepare_espnow()
{

    char rtc[512] = "ESPnow      ";
    uint8_t i;

    for (i = 0; i < sizeof(umac); i++)
        rtc[32 + i] = umac[i];

    for (i = 0; i < sizeof(NOW_SSID); i++)
        rtc[64 + i] = NOW_SSID[i];

    for (i = 0; i < sizeof(NOW_PASS); i++)
        rtc[96 + i] = NOW_PASS[i];

    rtc[128] = WIFI_CHANNEL;
    rtc[129] = HIDE_AP;
    rtc[130] = try_espnow;

    for (i = 0; i < sizeof(gmac); i++)
        rtc[160 + i] = gmac[i];

    for (i = 0; i < sizeof(kok); i++)
        rtc[192 + i] = kok[i];

    for (i = 0; i < sizeof(key); i++)
        rtc[224 + i] = key[i];

    ESP.rtcUserMemoryWrite(0, (uint32_t *)&rtc, sizeof(rtc));

    ESP.restart();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
