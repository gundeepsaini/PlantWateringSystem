


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
  int Converted_Value = map(rawValue, 0, 1023, Caliberation_Value_Air, Caliberation_Value_Water);

  if(Converted_Value < 0 || Converted_Value > 100)
      Status = 101;

  return Converted_Value;
} 
