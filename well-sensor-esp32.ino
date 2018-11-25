#include "Arduino.h"
#include "SPIFFS.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"

#include "definitions.h"

#include "init-led.h"
#include "init-button.h"
#include "init-qr.h"
#include "init-spiffs.h"
#include "init-server.h"
#include "init-data.h"
#include "init-wifi.h"
#include "init-mqtt.h"
#include "init-adc.h"

#include "data-median.h"
#include "data-adc.h"

#include "filter-median.h"

#include "routine-listen-ap.h"
#include "routine-data.h"

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Well Sensor...");

  initLed();
  initButton();
  initQr();

  //setWifiConfig("asus_2.4", "0965157829bi&");
  //setMqttConfig("m23.cloudmqtt.com", 12925, "tlwhlgqr", "g-VQc5c6w7eN");
 
  if(initSPIFSS()) {
    initWifi();  
    initMqtt();
  }
    
  Serial.println("Well Sensor is running");
}

void loop() {

  if(buttonClicked) {
    isAPRunning
      ? stopAP()
      : startAP();
    buttonClicked = false;    
    return;
  }

  listenServer();

  if(isAPRunning) {
    whiteLight();
    return;
  }

  if(!isWifiConfigSet) {
    redLight();    
    return;    
  }

  if(!isWifiConnected()) {
    yellowLight();
    reconnectWifi();
    return;
  }

  if(!isMqttConfigSet) {
    greenLight();
    return;
  }

  if(!mqttClient.connected()) {
    blueLight();
    reconnectMqtt();
    return;
  }

  noLight();

  if(runDataRoutine(getADC_Data)){
    dance(100);
  }
 
  mqttClient.loop();  
}
