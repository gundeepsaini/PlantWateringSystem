


void Handle_Sensor()
{
  Raw_Sensor_Value = get_Raw_SensorValue();
  Soil_Moisture_Value = get_Converted_SensorValue(Raw_Sensor_Value);
  
  Serial.print("Raw_Sensor_Value :");
  Serial.print(Raw_Sensor_Value);
  Serial.print("  Soil_Moisture_Value :");
  Serial.println(Soil_Moisture_Value);
}



int get_Raw_SensorValue()
{  
  int Sum = 0;

  for(int i=0; i<numReadings; i++)    
    {
      Sum = Sum + analogRead(Soil_Sensor_Pin);
      delay(Sampling_Delay);
    }

  int average = Sum / numReadings;
  return average;
}




int get_Converted_SensorValue(int rawValue)
{  
  int Converted_Value = map(rawValue, Caliberation_Value_Air, Caliberation_Value_Water, 0, 100);

  if(Converted_Value < 0 || Converted_Value > 100)
      {
        Status = Status | 0b0000000000000010;        // Bit 1
        Sensor_Status = -10;  // indicates out of range
      }
  else
    {
      if(Converted_Value < 10 )     // Maybe positioning error or caliberation error
      {
        Status = Status | 0b0000000000000010;        // Bit 1
        Sensor_Status = 10;  /// Maybe positioning error or caliberation error
      }
      else
      {
        Status = Status & 0b1111111111111101;        // Bit 1
        Sensor_Status = 1;
      }
    }      


  return Converted_Value;
} 
