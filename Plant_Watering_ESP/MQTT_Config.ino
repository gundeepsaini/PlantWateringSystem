

/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024
#include <PubSubClient.h>
#include <ArduinoJson.h>


/********************* Var *************************************/
WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;

/******************** Config **************************************/
const char* mqtt_server     = SECRET_MQTT_Server;
const char* mqtt_user       = SECRET_MQTT_User;
const char* mqtt_password   = SECRET_MQTT_Pass;

// Motion Sensor
//#define MQTT_CONFIG_PIR        "homeassistant/sensor/PIR/Kitchen/config"
#define MQTT_TOPIC_STATE_PLANT  "homeassistant/device/plants/state"
#define MQTT_TOPIC_CMD_PLANT    "homeassistant/device/plants/cmd"


unsigned long MQTT_heartbeat_timestamp = 0;

/**************** External Functions ************************************/

void MQTT_Config()
{
  client.setServer(mqtt_server, SECRET_MQTT_Port);
  client.setCallback(MQTT_MessageRecd_callback);  
}


void MQTT_loop()
{
  if (!client.connected())
      MQTT_reconnect();              
  
  client.loop();
}


void MQTT_publish()
{   
    //MQTT_heartbeat();
    MQTT_Msg();
}


/**************** Internal Functions ************************************/

void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      Serial.println("MQTT reconnecting");
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password)) 
      {
        //MQTT_publish_config();  
        Serial.println("MQTT connected");
        client.subscribe(MQTT_TOPIC_CMD_PLANT);
      }
      lastReconnectAttempt = millis()/1000;
  }

}


void MQTT_MessageRecd_callback(char* p_topic, byte* p_payload, unsigned int p_length) 
{
  String payload;
  for (uint8_t i = 0; i < p_length; i++) 
    { payload.concat((char)p_payload[i]); }

  if (String(MQTT_TOPIC_CMD_PLANT).equals(p_topic)) 
  {
    Serial.print("MQTT CMD:");
    Serial.println(payload);

    // MQTT Pump start command
    if (payload.equals(String("PUMP_ON")))    
      {
        MQTT_Pump_CMD = 1;
        Serial.println("MQTT Pump start cmd");
      }    
    else
    {
      // MQTT Pump reset interlock variables
      if (payload.equals(String("PUMP_RESET")))    
      {
        MQTT_Pump_CMD = 0;
        Pump_RunCycles = 0;
        Pump_ON_Start_time = 0;
        Pump_Cycle_Period_Start_time = 0;
        Status = 0;
        Serial.println("MQTT Pump reset cycles");
      }
      else
      {
        // MQTT Pump stop command
        MQTT_Pump_CMD = 0;
        Serial.println("MQTT Pump stop cmd");
      }
    }

  } 

}


void MQTT_heartbeat()
{
  if(millis()/1000 - MQTT_heartbeat_timestamp > 300 || MQTT_heartbeat_timestamp==0)
  {
    MQTT_heartbeat_timestamp = millis()/1000;
    client.publish(MQTT_TOPIC_STATE_PLANT, "Running", true);
  }
}


void MQTT_Msg()
{
    // Use arduinojson.org/v6/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(12);
    DynamicJsonDocument doc(capacity);
      
    doc["RawValue"]     = String(Raw_Sensor_Value);
    doc["MoistValue"]   = String(Soil_Moisture_Value);
    doc["Status"]       = String(Status);

    doc["PumpCycles"]   = String(Pump_RunCycles);
    doc["PumpState"]    = String(PumpState);
    doc["PumpRunTime"]  = String(Pump_Running_Time_secs);
      
    char data[256];
    serializeJson(doc, data, sizeof(data));
    client.publish(MQTT_TOPIC_STATE_PLANT, data, true);
    Serial.println(data);
}
