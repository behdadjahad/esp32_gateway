#include "WiFi.h"

void connect_to_wifi ()
{

  char ssid_wifi_array[50];
  char password_wifi_array[50];

  //set ssid and password from web server
  inputMessage_ssid.toCharArray(ssid_wifi_array, 50);
  inputMessage_wifi_password.toCharArray(password_wifi_array, 50);
  
  const char *ssid_wifi = ssid_wifi_array;
  const char *password_wifi = password_wifi_array;

  // connecting to wifi
  digitalWrite(wifi_led, LOW);
  Serial.println("Connecting");
  WiFi.begin(ssid_wifi, password_wifi);
  
  unsigned long startMillis = millis();
  unsigned long currentMillis = millis();
  
  int timeout = 5000;

  // waiting till connecting
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print(".");
    delay(50);
    currentMillis = millis();
    
    if (currentMillis - startMillis > timeout) {
      Serial.println("wifi not connected.");
      break;
    }
    
  }

  // update wifi state led 
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(wifi_led, HIGH);
  } else {
    digitalWrite(wifi_led, LOW);
  }

}
