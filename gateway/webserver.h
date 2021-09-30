
// send notfound reponse to invalid url
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/html", notfound_html);
}

void webserver ()
{
  // init variables for webserver
  setup_webserver();

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  // default page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  // response for change user and password
  server.on("/change", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", change_html);
  });

  // response for set url
  server.on("/url", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", url_html);
  });

  // response for change wifi ap
  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", wifi_html);
  });

  // response for get request
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasParam(PARAM_URL) && request->hasParam(PARAM_USERNAME) && request->hasParam(PARAM_PASSWORD)) {
      inputMessage_username = request->getParam(PARAM_USERNAME)->value();
      inputParam_username = PARAM_USERNAME;

      inputMessage_password = request->getParam(PARAM_PASSWORD)->value();
      inputParam_password = PARAM_PASSWORD;

      if (inputMessage_username.equals(user) && inputMessage_password.equals(pass)) {
        inputMessage_url = request->getParam(PARAM_URL)->value();
        inputParam_url = PARAM_URL;
        inputMessage_url.toCharArray(ThingsboardHost, 50);
        request->send(200, "text/html", success_html);
      } else {
        request->send(200, "text/html", wrong_html);
      }
    } else if (request->hasParam(PARAM_NEW_USERNAME) && request->hasParam(PARAM_NEW_USERNAME) && request->hasParam(PARAM_USERNAME) && request->hasParam(PARAM_PASSWORD)) {
      inputMessage_username = request->getParam(PARAM_USERNAME)->value();
      inputParam_username = PARAM_USERNAME;

      inputMessage_password = request->getParam(PARAM_PASSWORD)->value();
      inputParam_password = PARAM_PASSWORD;
      if (inputMessage_username.equals(user) && inputMessage_password.equals(pass)) {
        inputMessage_new_username = request->getParam(PARAM_NEW_USERNAME)->value();
        inputParam_new_username = PARAM_NEW_USERNAME;

        inputMessage_new_password = request->getParam(PARAM_NEW_PASSWORD)->value();
        inputParam_new_password = PARAM_NEW_PASSWORD;
        user = inputMessage_new_username;
        pass = inputMessage_new_password;

        request->send(200, "text/html", success_html);
      } else {
        request->send(200, "text/html", wrong_html);
      }
    } else if (request->hasParam(PARAM_DATA)) {
      inputMessage_data = request->getParam(PARAM_DATA)->value();
      inputParam_data = PARAM_DATA;
      request->send(200);
    } else if (request->hasParam(PARAM_SSID) && request->hasParam(PARAM_PASSWORD_WIFI) && request->hasParam(PARAM_USERNAME) && request->hasParam(PARAM_PASSWORD)) {
      inputMessage_username = request->getParam(PARAM_USERNAME)->value();
      inputParam_username = PARAM_USERNAME;

      inputMessage_password = request->getParam(PARAM_PASSWORD)->value();
      inputParam_password = PARAM_PASSWORD;

      if (inputMessage_username.equals(user) && inputMessage_password.equals(pass)) {
        inputMessage_ssid = request->getParam(PARAM_SSID)->value();
        inputParam_ssid = PARAM_SSID;

        inputMessage_wifi_password = request->getParam(PARAM_PASSWORD_WIFI)->value();
        inputParam_wifi_password = PARAM_PASSWORD_WIFI;
        request->send(200, "text/html", success_html);
        connect_to_wifi();

      } else {
        request->send(200, "text/html", wrong_html);
      }
    }
  });

  Serial.println(inputMessage_url);

  server.onNotFound(notFound);
  server.begin();
}
