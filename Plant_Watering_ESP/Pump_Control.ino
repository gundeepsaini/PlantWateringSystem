




void Pump_Control()
{	
	// Pump is not running
	if(PumpState == 0)
	{	
		if(MQTT_Pump_CMD == 1)
		{
			// check for max cycles
			if(Pump_RunCycles < Max_Pump_RunCycles)
			{	
				// check for on+off time - 1 cycle
				if( (millis()/1000 - Pump_ON_Start_time/1000) > (Max_Pump_ON_time + Max_Pump_OFF_time) )
				{
					PumpState = 1;
					digitalWrite(Water_Pump_Pin, PumpState);
					Pump_ON_Start_time = millis();

					Pump_RunCycles ++;
					
					// save time if it is first cycle
					if(Pump_RunCycles==1)
						Pump_Cycle_Period_Start_time = millis()/1000;

					Status = 0;	
				}
				//else
					// Pump off cycle ongoing
					//Status = 11;	
			}
			else
				// Max cycles reached
				Status = 102;
		}
	}
	
	// Pump is running
	if(PumpState == 1)
	{			
		// On time complete
		if(millis() - Pump_ON_Start_time > Max_Pump_ON_time * 1000)		
			Pump_OFF();
	
		// MQTT CMD - Pump OFF
		if(MQTT_Pump_CMD == 0)
			Pump_OFF();

		if(PumpState == 1)
			Pump_Running_Time_secs = (millis() - Pump_ON_Start_time) / 1000;
		else
			Pump_Running_Time_secs = 0;

		//mqtt send time left, pump status, cycle count
		if(millis() - MQTT_PumpMsg_timestamp > 500)
		{
		    MQTT_PumpMsg_timestamp = millis();		  
			MQTT_Msg();		
		}
	}
}


void Pump_OFF()
{
	PumpState = 0;
	digitalWrite(Water_Pump_Pin, PumpState);
	//Pump_ON_Start_time = 0;

	MQTT_Msg();	
}



void Pump_Cycle_Check()
{
  if(Pump_RunCycles > 0)
	{
		// reset cycles if time period has elapsed
		if(millis()/1000 - Pump_Cycle_Period_Start_time > Pump_Cycle_Period)
		{
			Status = 0;	
			Pump_RunCycles = 0;
			Pump_Cycle_Period_Start_time = 0;
		}
	}	
}