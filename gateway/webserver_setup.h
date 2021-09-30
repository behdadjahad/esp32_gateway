#include "ESPAsyncWebServer.h"
#include "html.h"


#define TOKEN "P6RdwYe3Wa00lUbXn5Fa"
AsyncWebServer server(80);

const char* PARAM_USERNAME = "input_username";
const char* PARAM_PASSWORD = "input_password";
const char* PARAM_URL = "input_url";
const char* PARAM_DATA = "input_data";
const char* PARAM_NEW_PASSWORD = "new_password";
const char* PARAM_NEW_USERNAME = "new_username";
const char* PARAM_SSID = "ssid";
const char* PARAM_PASSWORD_WIFI = "wifi_password";

const char *ssid_ap = "gateway_esp32";
const char *password_ap = "12345678";

//defult user pass
String user = "admin";
String pass = "admin";

String inputMessage_password;
String inputParam_password;

String inputMessage_new_password;
String inputParam_new_password;

String inputMessage_username;
String inputParam_username;

String inputMessage_new_username;
String inputParam_new_username;

String inputMessage_url;
String inputParam_url;

String inputMessage_data = "-1";
String inputParam_data;

String inputMessage_ssid;
String inputParam_ssid;

String inputMessage_wifi_password;
String inputParam_wifi_password;



void setup_webserver ()
{
  WiFi.softAP(ssid_ap, password_ap);

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  digitalWrite(webserver_led, HIGH);
}
