

void WebServer_Config()
{
  
  server.onNotFound(handleNotFound);
  server.on("/",Webpage_Home1);
  server.on("/webpage1",Webpage_Home1);
  server.on("/OTA",Webpage_OTA);
  server.on("/restart",Webpage_restart);

  server.begin();  
}

void Webpage_Home1()
{
  Prep_webpage1();
  server.send ( 200, "text/html", webpage1 );
  delay(10);
  webpage1 = "";
}

void Webpage_OTA()
{
  OTA_Mode = 1;
  server.send(200,"text/plain","OTA mode active now");
}


void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found! Send one of these: /OTA, /restart, /webpage1");
}

void Webpage_restart()
{
  server.send(200,"text/plain","Restarting ESP");
  Serial.println("Restart requested from webserver");
  delay(1000);
  Restart_ESP();
}


