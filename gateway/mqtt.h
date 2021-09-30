#include <PubSubClient.h>


// token of thingsboard
#define TOKEN "P6RdwYe3Wa00lUbXn5Fa" //Access token of device Display
// ip of things board server host
char ThingsboardHost[50] = "217.219.215.226";
// client id 
String clientID = "group_gateway";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;

void getAndSendData()
{

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"Humidity\":";
  payload += String(numHumidity, 2);
  payload += ",";
  payload += "\"Temperature\":";
  payload += String(numTemperature, 2);
  payload += ",";
  payload += "\"Distance\":";
  payload += String(inputMessage_data);
  payload += "}";

  char attributes[1000];
  payload.toCharArray( attributes, 1000 );
  client.publish( "v1/devices/me/telemetry", attributes);
  Serial.println( attributes );

}


void reconnect ()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if (status != WL_CONNECTED) {
      connect_to_wifi();
    }
    Serial.println(ThingsboardHost);
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect(clientID.c_str(), TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.println( " : retrying in 5 seconds]" );
      delay(100);
      vTaskResume(lora_thread_handle);
      vTaskSuspend(mqtt_thread_handle);

    }
  }
}

void mqtt_thread(void *parameter)
{
  Serial.println(ThingsboardHost);
  client.setServer( ThingsboardHost, 1883 );
  while (1) {
    if ( !client.connected() )
    {
      reconnect();
    }
    getAndSendData();
    delay(250);

    vTaskResume(lora_thread_handle);
    vTaskSuspend(mqtt_thread_handle);
  }

}
