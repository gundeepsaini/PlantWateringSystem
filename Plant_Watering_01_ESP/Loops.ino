

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{ 
  server.handleClient(); 
  MQTT_loop();
  Pump_Control();
}


void Slow_Loop()
{  
  Handle_Sensor();
  MQTT_publish();
  Pump_Cycle_Check();
}



void Mid1_Loop()
{  


}


void Mid2_Loop()
{
  
}



void VerySlow_Loop()
{
  Time_NTP_Update();
}
