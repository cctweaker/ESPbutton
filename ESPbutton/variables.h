/////////////////////////////////////////////////
// configuration files
/////////////////////////////////////////////////
const char dev_cfg_file[] = "device.json";
const char wifi_cfg_file[] = "wifi.json";
const char mqtt_cfg_file[] = "mqtt.json";
const char espnow_cfg_file[] = "espnow.json";
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// PCA9536 data
/////////////////////////////////////////////////
const uint8_t PCA_ADR = 0x41; // PCA9536 I2C address
const uint8_t INP_REG = 0x00; // input port register
const uint8_t OUT_REG = 0x01; // output port register
const uint8_t POL_REG = 0x02; // polarity inversion register
const uint8_t CNF_REG = 0x03; // configuration register
const uint8_t BYTE = 0x01;    // 1 byte messages
/////////////////////////////////////////////////
uint8_t butoane = 0;
uint8_t butoane_last = 0;
uint8_t ab = 0;
uint8_t xb = 0;
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// button types
/////////////////////////////////////////////////
uint8_t button_type = 0;
// 0 is active low, momentary, mains & battery powered
// 1 is active high, momentary, mains powered
// 2 is active low/high, rocker switch, mains powered
//
// type 1 is suitable for doorbell type light switches
// type 2 is suitable for standard light switches
// ESPbutton will send a message on each switch toggle
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// flags
/////////////////////////////////////////////////
bool battery_powered = false; // device is battery powered?

bool use_espnow = false;    // ESP-Now started
bool skip_espnow = false;   // skip ESP-Now config load
bool use_softap = false;    // SoftAP for configuration started
bool use_wifi = false;      // wifi started
bool use_webserver = false; // webserver started
bool use_mqtt = false;      // mqtt started
bool use_ssl = false;       // SSL started
bool use_dns = false;       // DNS server started

bool start_espnow = false;   // start ESP-Now
bool try_espnow = false;     // start ESP-Now once
bool start_wifi = true;      // start wifi
bool start_webserver = true; // start webserver
bool start_mqtt = false;     // start MQTT client
/////////////////////////////////////////////////
bool cfg_wifi = false;   // wifi config file read?
bool cfg_dev = false;    // device config file read?
bool cfg_mqtt = false;   // mqtt config file read?
bool cfg_espnow = false; // ESP-Now config file read?
/////////////////////////////////////////////////
bool do_ota_update = false;  // OTA requested
bool do_fs_format = false;   // FS format requested
bool do_save_config = false; // CFG save requested
bool PCA = false;            // PCA chip found?
bool SENT = true;            // data sent flag
bool ACK = true;             // ESP-Now packet received by GW
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// device setup
/////////////////////////////////////////////////
const char FW_NAME[] = "ESPbutton";

char LOC[16] = "home";
char TIP[16] = "button";
char NAME[16] = "attic";

char HOSTNAME[16] = "ESPbutton";
char update_url[128] = "esp.3dstar.ro/";

bool heartbeat = false;
uint8_t heartbeat_minutes = 10;
unsigned long last_heartbeat = 0;
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// WIFI
/////////////////////////////////////////////////
char SSIDa[32] = "";
char PASSa[64] = "";

char SSIDb[32] = "";
char PASSb[64] = "";
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// Configuration AP
/////////////////////////////////////////////////
const char STA_SSID[] = "ESPbutton";
const char STA_PASS[] = "abcd1234";
IPAddress STA_IP(192, 168, 10, 1);
IPAddress STA_MASK(255, 255, 255, 0);
const byte DNS_PORT = 53;
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// MQTT
/////////////////////////////////////////////////
char MQTT_HOST[64] = "mqtt.3dstar.ro";
int MQTT_PORT = 1883;
char MQTT_USER[16] = "test";
char MQTT_PASS[32] = "12345678";

char WILL[16] = "/will";
char PUB[16] = "";
char SUB[16] = "/cmnd";
char STAT[16] = "/stat";
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// ESP-Now
/////////////////////////////////////////////////
uint8_t gmac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // gateway mac
uint8_t umac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF}; // this units mac

uint8_t kok[16] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};
uint8_t key[16] = {0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED};

char NOW_SSID[20] = "ESPbutton";
char NOW_PASS[16] = "abcd1234";
uint8_t WIFI_CHANNEL = 1;
bool HIDE_AP = false;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
void espnow_send_buffer(const char *tx);
const char *save_espnow();
/////////////////////////////////////////////////

#ifdef USE_SSL
// following cert is for Let's Encrypt CA
// it will authenticate any valid Let's Encrypt issued cert
static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow
SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT
GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF
q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8
SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0
Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA
a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj
/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T
AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG
CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv
bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k
c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw
VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC
ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz
MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu
Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF
AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo
uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/
wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu
X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG
PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6
KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==
-----END CERTIFICATE-----
)EOF";
#endif