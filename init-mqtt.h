#ifndef __INIT_MQTT__
#define __INIT_MQTT__

struct MqttConfig
{
  char server[MAX_STR_LEN];
  int port;
  char user[MAX_STR_LEN];
  char pwd[MAX_STR_LEN];
  char deviceId[MAX_STR_LEN];
};

MqttConfig mqttConfig;
const char *mqttFileName = "/mqttConfig.json";
bool isMqttConfigSet = false;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqttCallback(char* topic, byte* payload, size_t len){
  
  Serial.print("Message arrived: [");
  Serial.print(topic);
  Serial.print("] =");
  for (size_t i = 0; i < len; ++i)
  {
    Serial.print(char(payload[i]));  
  }
  Serial.println();  
}
void setMqttConfig(const char* server, int port, const char* user, const char* pwd, const char* device){
  
  memset(mqttConfig.server, 0, MAX_STR_LEN);
  memcpy(mqttConfig.server, server, MAX_STR_LEN);

  mqttConfig.port = port;

  memset(mqttConfig.user, 0, MAX_STR_LEN);
  memcpy(mqttConfig.user, user, MAX_STR_LEN);

  memset(mqttConfig.pwd, 0, MAX_STR_LEN);
  memcpy(mqttConfig.pwd, pwd, MAX_STR_LEN);

  memset(mqttConfig.deviceId, 0, MAX_STR_LEN);
  memcpy(mqttConfig.deviceId, device, MAX_STR_LEN);

  isMqttConfigSet = true;
}

bool containsMqttConfig(JsonObject& json) {
  return
    json.containsKey(MQTT_CONFIG_SERVER)
    && json.containsKey(MQTT_CONFIG_PORT)
    && json.containsKey(MQTT_CONFIG_USER)
    && json.containsKey(MQTT_CONFIG_PWD)
    && json.containsKey(MQTT_CONFIG_DEVICE_ID);
    
}

bool loadMqttConfig() {
  
  bool res = false;

  if (readFile(mqttFileName)) {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(fileBuff);
      if (
        json.success()
        && containsMqttConfig(json)
      ) {
        setMqttConfig(json[MQTT_CONFIG_SERVER], json[MQTT_CONFIG_PORT], 
                      json[MQTT_CONFIG_USER], json[MQTT_CONFIG_PWD], json[MQTT_CONFIG_DEVICE_ID]);
        res = true; 
      } else {
        Serial.println("Mqtt configuration is corrupted");  
      }
  }
  
  return res;
}

bool saveMqttConfigToSPIFFS(const char* server, int port, const char* user, const char* pwd, const char* device) {
  
  bool res = false;  

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json[MQTT_CONFIG_SERVER] = server;
  json[MQTT_CONFIG_PORT] = port;
  json[MQTT_CONFIG_USER] = user;
  json[MQTT_CONFIG_PWD] = pwd;
  json[MQTT_CONFIG_DEVICE_ID] = device;

  if (saveJson(mqttFileName, json)) {
    res = true;
    Serial.println("Mqtt configs are saved.");  
  }
  return res;
}

bool connectMqtt(const char* user, const char* pwd, const char* device){
  bool res = false;
      
  mqttClient.connect(device, user, pwd);
  if (mqttClient.connected()) {    
    res = true;
    Serial.println("Connected to mqtt broker");    
  } else {
    Serial.print("failed, rc = ");
    Serial.println(mqttClient.state());
  }

  return res;
}

bool reconnectMqtt() {
  return connectMqtt(mqttConfig.user, mqttConfig.pwd, mqttConfig.deviceId);
}

void setMqttServer(const char* server, int port){
  delay(500);
  mqttClient.setServer(server, port);
  Serial.print("Mqtt server: ");  
  Serial.print(server);
  Serial.print(":");
  Serial.println(port); 
}

void initMqtt() {
  mqttClient.setCallback(mqttCallback);  
  loadMqttConfig();
  if(isMqttConfigSet){
    setMqttServer(mqttConfig.server, mqttConfig.port);
  }  
}

void publishJson(const char* topic, JsonObject& json) {
  const char* deviceId = mqttConfig.deviceId;
  
  json[PAYLOAD_DEVICE] = deviceId;
  json[PAYLOAD_TOPIC] = topic;
  
  String pailoadStr;
  json.printTo(pailoadStr);
  uint8_t pailoadSize = pailoadStr.length() + 1;
  char payload[pailoadSize];
  pailoadStr.toCharArray(payload, pailoadSize);
  
  String mqttTopicStr;
  mqttTopicStr.concat(deviceId);
  mqttTopicStr.concat(TOPIC_SEPARATOR);
  mqttTopicStr.concat(topic);
  uint8_t mqttTopicSize = mqttTopicStr.length() + 1;
  char mqttTopic[mqttTopicSize];
  mqttTopicStr.toCharArray(mqttTopic, mqttTopicSize);
  mqttClient.publish(mqttTopic, payload);
}

bool tryConnectMqtt(const char* server, int port, const char* user, const char* pwd, const char* device){
  bool res = false;
  setMqttServer(server, port);
  uint8_t attempts = 0;
  Serial.println("Connecting Mqtt");
  while (!connectMqtt(user, pwd, device) && attempts < MQTT_TIMEOUT) {
     delay(500);
     attempts++;
  }

  if (attempts < MQTT_TIMEOUT) {
    isWifiConfigSet = true;
    res = true;
    Serial.println("MQTT Connected");
  } else {
    Serial.println("Failed to connect MQTT");
  }
  return res;
}

uint8_t handleMqttJson(JsonObject& json) {
  uint8_t res = 0;
  
  if(
      json.success()
      && containsMqttConfig(json)
  ) {
        
    const char* server = json[MQTT_CONFIG_SERVER];
    int port = json[MQTT_CONFIG_PORT];
    const char* user = json[MQTT_CONFIG_USER];
    const char* pwd = json[MQTT_CONFIG_PWD];
    const char* device = json[MQTT_CONFIG_DEVICE_ID];
    
    if(tryConnectMqtt(server, port, user, pwd, device)) {
      if(saveMqttConfigToSPIFFS(server, port, user, pwd, device)) {
        setMqttConfig(server, port, user, pwd, device);
        Serial.println("MQTT is configured");
      } else {
        res = SAVE_MQTT_FAILED_RESPONSE_HEADER;
        Serial.println("Saving mqttConfig failed");                
      }
    } else {
      res = MQTT_CONNECTION_FAILED_RESPONSE_HEADER;
      Serial.println("Error. Could not connect to MQTT with mqttConfig provided");
    }
  } else {
    res = INVALID_MQTT_CONFIG_RESPONSE_HEADER;
    Serial.println("Error. Invalid mqttConfig is received");
  }
  
  return res;
}

#endif
