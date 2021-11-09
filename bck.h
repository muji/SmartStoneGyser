
#include <Arduino.h>







/*********
Allah-Kafi

RkW:a7awPaI6.7er
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "max6675.h"
#include <Ticker.h>



 



// Replace with your network credentials
const char* ssid = "Allah-Kafi";
const char* password = "RkW:a7awPaI6.7er";

//#define SCK_PIN 15//D8
//#define CS_PIN 13//D7
//
//#define SO_PIN 12//D6


int thermoCLK = 12;
int thermoCS = 14;
int thermoDO = 27;

#define READINGS_NUMBER 20

#define DELAY_TIME 20
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
float tempC;
int MaxTemp = 42;
bool gyser_State = false;

bool slnd_State = 0;
const int gas_selonoid = 13;



void SetGas(bool);
volatile int interrupts;
int totalInterrupts;
void MonitorTask( void *  );

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  body {
  height: 100vh;
  width: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
  background: #4b79a1; /* fallback for old browsers */
  background: -webkit-linear-gradient(
    to right,
    #283e51,
    #4b79a1
  ); /* Chrome 10-25, Safari 5.1-6 */
  background: linear-gradient(to right, #283e51, #4b79a1);
}

.toggle {
  position: relative;
}

.toggle input {
  position: absolute;
  opacity: 0;
  width: 0;
  height: 0;
}

.toggle .onoff {
  color: #fff;
  font-size: 6em;
  text-align: center;
  display: block;
  font-family: Arial, Helvetica, sans-serif;
}

.slider {
  position: relative;
  display: block;
  cursor: pointer;
  background-color: #333;
  transition: 0.4s;
  width: 200px;
  height: 120px;
}

.slider:before {
  content: "";
  position: absolute;
  height: 100px;
  width: 100px;
  background-color: #fff;
  transition: 0.4s;
  top: 10px;
  left: 10px;
}

input:checked + .slider {
  background-color: #fd5d00;
  box-shadow: 0 0 12px #fd5d00;
}

input:checked + .slider:before {
  transform: translateX(80px);
}

.slider.round {
  border-radius: 20px;
}
.slider.round::before {
  border-radius: 20px;
}
  </style>
<title>ESP Gyser</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  

  <div class="content">
   <label class="toggle">
  <span class="onoff">OFF</span>
  <input type="checkbox" />
  <span class="slider round"></span>
</label>
      
    </div>
    
<script>


const toggle = document.querySelector('.toggle input')

toggle.addEventListener('click', () => {
    const onOff = toggle.parentNode.querySelector('.onoff')
    onOff.textContent = toggle.checked ? 'ON' : 'OFF'
})

  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }

 
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {


  var state;
    if (event.data == "1"){
      state = "ON";
      document.getElementById('buttonIgn').value = OFF
   
    }
    if (event.data == "0"){
      document.getElementById('buttonIgn').value = ON
    }
  
  
  document.getElementById('temp').innerHTML = event.data
   
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('buttonIgn').addEventListener('click', toggle);
    document.getElementById('button1').addEventListener('click', high);
    document.getElementById('button2').addEventListener('click', meduim);
    document.getElementById('button3').addEventListener('click', low);
  }
  function toggle(){
    websocket.send('toggle');
  }
  function high(){
    websocket.send('high');
  }
  function meduim(){
    websocket.send('meduim');
  }
  function low(){
    websocket.send('low');
  }
</script>
</body>
</html>
)rawliteral";