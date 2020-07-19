/*

    Automatic Plant Watering System

  
  Date              : 16.07.2020
  Device            : ESP8266
  

  Status Msg:
    - Status = 101; // raw sensor value out of caliberation range 
    

------------------------------------------- */





/* ------------- LIB ------------------------------ */
#include "Secrets.h"
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>


/* ------------- CONFIG VAR ------------------------------ */
unsigned long looptime_Fast = 1;    // in secs
unsigned long looptime_Mid1 = 1;    // in secs
unsigned long looptime_Mid2 = 10;   // in secs
unsigned long looptime_Slow       = 1 * (60);      // in mins
unsigned long looptime_VerySlow   = 5 * (60);      // in mins

int Soil_Sensor_Pin = A0;
int Water_Pump_Pin = D5;

// Pump Duty Cycle - for safety 
int Max_Pump_ON_time    = 10;             // On time  - max seconds the pump will run for in a single cycle
int Max_Pump_OFF_time   = 50;             // Off time - min seconds the pump will be off for in a single cycle
int Max_Pump_RunCycles  = 5;              // Max cycles the pump will run for in a fixed period (defined in Max period)
int Pump_Cycle_Period   = 3 * 60 * 60;    // Fixed period within which pump will run 

/*

To take care of flooding situation in case of bug / software failure
e.g. 
  Pump_Cycle_Period   - 3 hours
  Max_Pump_RunCycles  - 10 cycles
  Max_Pump_ON_time    - 10 sec
  Max_Pump_OFF_time   - 50 sec

  (cycle 1) pump runs for 10 sec  ---> pump off for 50 sec 
  ---> if retrigger reqd ---> (cycle 2) pump runs for 10 sec  ---> pump off for 50 sec
  on_off cycle repeats max 10 times if required
  cycle count resets after cycle period 3 hours

*/

int Sampling_Delay  = 50; // delay between sampling of analog read
int numReadings     = 10; // number of samples per reading

int Caliberation_Value_Air    = 715;  //   0% soil moisture
int Caliberation_Value_Water  = 285;  // 100% soil moisture


/* ------------- VAR ------------------------------ */
const char* ssid             = SECRET_WIFI_SSID2;
const char* pass             = SECRET_WIFI_PASS2;
const char* DeviceHostName   = SECRET_Device_Name8;
const char* OTA_Password     = SECRET_Device_OTA_PASS; 
unsigned long lastrun_fast, lastrun_Mid1;
unsigned long lastrun_Mid2, lastrun_slow, lastrun_Veryslow;
unsigned long lastrun_30mins,lastrun_OTA;
bool OTA_Mode=0;
String webpage1="";

int Soil_Moisture_Value = 0;
int Raw_Sensor_Value = 0;
int Status = 0;

int PumpState = 0;


ESP8266WebServer server(80);



void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Switch off pump if reset / on start up
  Pump_OFF();
  
  Serial.begin(115200);
  Serial.println(DeviceHostName);

  wifi_station_set_hostname(DeviceHostName);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Wifi Error! Rebooting in 30s...");
    delay(30 *1000);
    digitalWrite(LED_BUILTIN, HIGH);
    Restart_ESP();
  }
  
  Serial.print("Connected to ");
  Serial.print(WiFi.SSID());
  Serial.print(", IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.print(", MAC: ");
  Serial.println(WiFi.macAddress());
  
  OTA_Config();
  WebServer_Config();
  MQTT_Config();
  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Ready");
}

void loop() 
{
  // Always
  Always_loop();

  // Fast Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_fast > looptime_Fast) || lastrun_fast ==0))
  {
    lastrun_fast = millis()/1000;
    Fast_Loop();
  }

/*
  // Mid1 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid1 > looptime_Mid1) || lastrun_Mid1 ==0))
  {
    lastrun_Mid1 = millis()/1000;
    Mid1_Loop();
  }


  // Mid2 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid2 > looptime_Mid2) || lastrun_Mid2 ==0))
  {
    lastrun_Mid2 = millis()/1000;
    Mid2_Loop();
  }
*/
  // Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_slow > looptime_Slow) || lastrun_slow ==0))
  {
    lastrun_slow = millis()/1000;
    Slow_Loop();
  }

/*

    // Very Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Veryslow > looptime_VerySlow) || lastrun_Veryslow ==0))
  {
    lastrun_Veryslow = millis()/1000;
    VerySlow_Loop();
  }

*/
}




void Restart_ESP()
{
  Serial.println("Restarting ESP");
  ESP.restart();
  delay(1000);
  while(1);
}
