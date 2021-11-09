
#include "header.h"




void getTemp()
{

   tempC = thermocouple.readCelsius();// Celsius
   if(MaxTemp<tempC)
    SetGas(false);

  
}



// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


//window.alert(event.data);
void notifyClients() {
  getTemp();
  ws.textAll(String(tempC));

  ws.textAll(String(gyser_State));
}



void handlingIncomingData(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    String recMessage = "";
    for (int i=0; i < len; i++)
      recMessage += ((char) data[i]);

    Serial.println("Message :  " + recMessage);
    
    Serial.println("/n");
  
  }
}




void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handlingIncomingData(arg, data, len);


      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  
}

////////////////////make selonoid valve off On

void SetGas(bool _state)
{

digitalWrite(gas_selonoid, _state);
  
}





String processor(const String& var){
  
return "OFF"; 
}






////////////////////////----------------------------------------------------------/////////////////////////////////////








void setup()
{
  Serial.begin(115200); 
  
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(4000);
    Serial.println("Connecting to WiFi..");
  }

 // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  

/* we create a new task here */
xTaskCreate(
MonitorTask, /* Task function. */
"sensor Task", /* name of task. */
40000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */




}
 






/* the forever loop() function is invoked by Arduino ESP32 loopTask */
void loop()
{
Serial.println("this is Monitor Task");



   getTemp();

   if(MaxTemp>tempC)
    SetGas(true);

  

delay(1000);
}
 





 
/* this function will be invoked when additionalTask was created */
void MonitorTask( void * parameter )
{
/* loop forever */




  initWebSocket();

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/index.html", String(), false, processor);

  
  });


  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

   // Route to set GPIO to HIGH 'ON' : 'OFF'
  server.on("/ON", HTTP_GET, [](AsyncWebServerRequest *request){
    //digitalWrite(ledPin, HIGH);    
    Serial.println("ON Called");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to LOW
  server.on("/OFF", HTTP_GET, [](AsyncWebServerRequest *request){
   // digitalWrite(ledPin, LOW);  
    Serial.println("OFF Called");  
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();





for(;;)
{

// Print out raw message

delay(2000);
notifyClients();
Serial.println("notifyClients() Called");

}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}