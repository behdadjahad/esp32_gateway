#include <WiFi.h>
#include <HTTPClient.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <DHT.h>

#define relay 5
#define buzzer 2

#include "lora.h"
#include "wifi.h"

String serverName = "http://192.168.4.1/get?input_data=";

const int trigPin = 33;
const int echoPin = 32;

#define SOUND_SPEED 0.034

long duration;
int distanceCm;

void setup ()
{
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

   xTaskCreatePinnedToCore(lora_thread, "thread for lora", 10240, NULL, 1, &loraThread, 1);

  connection();
}

void loop ()
{

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  if (distanceCm > 30) {
    Serial.println("SOS");
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  delay(250);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = serverName + String(distanceCm);

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
    connection();
  }
  vTaskResume( loraThread );
}
