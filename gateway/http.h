#include <HTTPClient.h>


// send http request every 50 seconds

unsigned long lastTime = 0;
unsigned long timerDelay = 50000;

String create_url ()
{

  //creating urls from the url got in local wizard page(webserver)
  String result = "https://" + inputMessage_url + "/api/v1/" + TOKEN + inputMessage_data;
  //attributes?clientKeys=attribute1
  return result;
}

void http_request ()
{
  // send https request 
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(create_url().c_str());
    short http_response_code = http.GET();
    if (http_response_code > 0) {
      Serial.print("http response code: ");
      Serial.println(http_response_code);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("error code: ");
      Serial.println(http_response_code);
    }
    http.end();

  } else {
    digitalWrite(wifi_led, LOW);
    Serial.println("wifi not connected.");
  }
}

void http_loop ()
{
  // send https request every 50 seconds
  if ((millis() - lastTime) > timerDelay) {
    Serial.println(create_url());
    //http_request();
    lastTime = millis();
  }
}
