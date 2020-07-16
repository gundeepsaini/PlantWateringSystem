

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{ 
  server.handleClient(); 
  MQTT_loop();
}


void Slow_Loop()
{  
  Handle_Sensor();
  MQTT_publish();
}



void Mid1_Loop()
{  


}


void Mid2_Loop()
{
  
}



void VerySlow_Loop()
{
  
}
