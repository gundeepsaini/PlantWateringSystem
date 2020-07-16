

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{ 
  server.handleClient(); 
  //MQTT_loop();
}


void Slow_Loop()
{  
  //MQTT_publish();
  Raw_Sensor_Value = get_Raw_SensorValue();
  Soil_Moisture_Value = get_Converted_SensorValue(Raw_Sensor_Value);
  
  Serial.print("Raw_Sensor_Value :");
  Serial.print(Raw_Sensor_Value);
  Serial.print("  Soil_Moisture_Value :");
  Serial.println(Soil_Moisture_Value);
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
