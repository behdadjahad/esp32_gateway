
// handles for threads
TaskHandle_t lora_thread_handle;
TaskHandle_t mqtt_thread_handle;

// state leds on box
const int webserver_led = 26;
const int wifi_led = 27;
const int lora_led = 32;

//include headers 
#include "webserver_setup.h"
#include "http.h"
#include "wifi_connection.h"
#include "lora.h"
#include "mqtt.h"
#include "webserver.h"


void setup() {
  // set pins for state leds
  pinMode(webserver_led, OUTPUT);
  pinMode(wifi_led, OUTPUT);
  pinMode(lora_led, OUTPUT);

  delay(1000);

  // on and off leds

  digitalWrite(webserver_led, HIGH);
  digitalWrite(wifi_led, HIGH);
  digitalWrite(lora_led, HIGH);

  delay(2000);

  digitalWrite(webserver_led, LOW);
  digitalWrite(wifi_led, LOW);
  digitalWrite(lora_led, LOW);

  // start serial
  Serial.begin(115200);
  // start webserver
  setup_webserver();
  webserver();

  // start mqtt thread
  xTaskCreatePinnedToCore(mqtt_thread, "mqtt function", 10240 , NULL, 1, &mqtt_thread_handle, 1);
  // start lora thread
  xTaskCreatePinnedToCore(lora_thread, "lora function", 10240 , NULL, 1, &lora_thread_handle, 1);

}

void loop() {
  // afther reconnecting to wifi the threads will got suspended and we need to resume mqtt
  vTaskResume(mqtt_thread_handle);
}
