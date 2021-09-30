#include <RH_RF95.h>
#include <SPI.h>



// setting pins for lora
float RF95_FREQ = 433.0;
#define RFM95_CS 5
#define RFM95_RST 12
#define RFM95_INT 2
RH_RF95 rf95(RFM95_CS, RFM95_INT);


// init variables for data that recived fromm lora

char nodeID[20];
char nodeTemperature[25];
char nodeHumidity[25];
char nodeValue[25];


// init variacles for node that sends data to us via lora
int numNode = 0;
float numTemperature = 0;
float numHumidity = 0;
float numValue = 0;

void lora_thread (void *parameter)
{

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  // reseting lora 02

  digitalWrite(RFM95_RST, LOW);                                                //Manual reset.
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  //lora initialization 
  if (!rf95.init()) {
    Serial.println(">> LoRa initialization is failed!");
    digitalWrite(lora_led, LOW);
    vTaskResume(mqtt_thread_handle);
    vTaskSuspend(lora_thread_handle);
  } else {
    Serial.println(">> LoRa initialized successfully.");
    digitalWrite(lora_led, HIGH);
  }

  // set frequency for lora
  if (!rf95.setFrequency(RF95_FREQ)) {                                      //Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM.
    Serial.println(">> SetFrequency failed!");
    digitalWrite(lora_led, LOW);
    vTaskResume(mqtt_thread_handle);
    vTaskSuspend(lora_thread_handle);

  }

  Serial.print(">> Set Freq to: ");
  Serial.println(RF95_FREQ);
  rf95.setTxPower(20, false);

  Serial.println(">> Setup is done.");
  Serial.println("***********************************************");

  delay(100);

  while (1) {
    numNode = 0;


    // reciving data
    if (rf95.available())                                                        //Listen Data from LoRa Node.
    {
      digitalWrite(lora_led, HIGH);
      int i = 0, j = 0, code[5];
      int m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0;
      char message[40] = "\0";
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];                                      //Receive data buffer.
      uint8_t len = sizeof(buf);                                                 //Data buffer length.
      uint8_t nodeReply[] = "   Server ACK.";
      if (rf95.recv(buf, &len))                                                  //Check if there is incoming data.
      {
        strcpy(message, (char *)buf);
        Serial.print(">> Message is: ");
        Serial.println(message);
        while (i < 40) {
          if (message[i] == ',') {
            code[j] = i;
            j++;
          }
          i++;
        }
        for (int k = 0; k < code[0]; k++) {
          nodeID[m1] = message[k];                                               //Get nodeID.
          m1++;
        }
        for (int k = code[0] + 1; k < code[1]; k++) {
          nodeTemperature[m2] = message[k];                                      //Get Temperature.
          m2++;
        }
        for (int k = code[1] + 1; k < code[2]; k++) {
          nodeHumidity[m3] = message[k];                                         //Get Humidity.
          m3++;
        }
        for (int k = code[2] + 1; k < code[3]; k++) {
          nodeValue[m4] = message[k];                                            //Get Value.
          m4++;
        }

        numHumidity = atof(nodeHumidity);
        numTemperature = atof(nodeTemperature);
        numValue = atof(nodeValue);

        numNode = atoi(nodeID);

        if (numNode == 69) {
          Serial.println(">> Server got Node 69's message.");
          nodeReply[0] = '6';
          nodeReply[1] = '9';
          rf95.send(nodeReply, sizeof(nodeReply));
          rf95.waitPacketSent();
        }
        nodeID[0] = 0;
        nodeID[1] = 0;

      }
    }

    // resume mqtt thread and suspend lora thtead 
    vTaskResume(mqtt_thread_handle);
    vTaskSuspend(lora_thread_handle);
  }

}
