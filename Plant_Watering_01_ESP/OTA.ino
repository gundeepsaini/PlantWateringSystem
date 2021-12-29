

/* ---------------------------------------- */
void OTA_Config()
{
  ArduinoOTA.setPort(SECRET_Device_OTA_PORT);
  ArduinoOTA.setHostname(DeviceHostName);
  ArduinoOTA.setPassword(SECRET_Device_OTA_PASS);

  ArduinoOTA.onStart([]() 
  {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) 
      type = "sketch";
    else 
      type = "filesystem";
    
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() 
  {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) 
  {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) 
  {    
      {
        Serial.printf("Error[%u]: ", error);      
        if(error == OTA_AUTH_ERROR) 
          Serial.println("Auth Failed");
        else 
          if (error == OTA_BEGIN_ERROR) 
            Serial.println("Begin Failed");
          else 
            if (error == OTA_CONNECT_ERROR) 
              Serial.println("Connect Failed");
            else 
              if (error == OTA_RECEIVE_ERROR) 
                Serial.println("Receive Failed");
              else 
                if (error == OTA_END_ERROR)
                  Serial.println("End Failed");
      }
  });

  ArduinoOTA.begin();
}


void OTA_Check()
{
  ArduinoOTA.handle();

  // disable OTA mode after 10mins if no activity
  if(OTA_Mode)
  {
    if(lastrun_OTA == 0)
       {
        lastrun_OTA = millis()/1000;              
        Serial.println("Entering OTA Mode");
       }
       
    if(millis()/1000 - lastrun_OTA > 10 * 60)
       {
        lastrun_OTA = 0;
        OTA_Mode = 0;        
        Serial.println("Exiting OTA");
       }
  }
}
