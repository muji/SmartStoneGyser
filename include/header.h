
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
#include "SPIFFS.h"



 



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