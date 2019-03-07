#include "DHT.h"
#include <VirtualWire.h>
#include <Narcoleptic.h>
#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 


//DHT 11
#define VCC 13
#define Data 12
#define GND 10


//TRANSMITTER
const int DATA_pin = 9;
const int VCC_pin = 8;
const int GND_pin = 7;

//LDR

int LDR = A4;  //Pin for Photo resistor
int LDRValue = 0;
int Light_sensitivity = 255;


DHT dht(Data, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");


  pinMode(VCC,OUTPUT);
  pinMode(Data,OUTPUT);
  pinMode(GND,OUTPUT);


  digitalWrite(VCC,HIGH);
  digitalWrite(GND,LOW);
  
  pinMode(VCC_pin,OUTPUT);
  pinMode(DATA_pin,OUTPUT);
  pinMode(GND_pin,OUTPUT);


  digitalWrite(VCC_pin,HIGH);
  digitalWrite(GND_pin,LOW);
  
  pinMode(LDR,INPUT);

  // Initialise the IO and ISR
  vw_set_tx_pin(DATA_pin);

  vw_setup(2000);       


  dht.begin();
}

void loop()
{
   Narcoleptic.delay(600000); // During this time power consumption is minimised
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int light = analogRead(LDR);
 
  
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else {
    Serial.println("Transmitting: ");
    delay(1000);
    Serial.println("");
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    Serial.print("light intensity: "); 
    Serial.println(light);
    if(light > 200)
    {
    light = 1;Serial.println(light);
    }
    else
    {
     light = 0;Serial.println(light);
    }
  }

 const int bytes = 4 * sizeof(int); 
 int name[bytes];

  name[0]=243; 
  name[1]=h; 
  name[2]=t; 
  name[3]=light;

 send((byte*)name,bytes);
 delay(2000);
}

void send(byte *name,int length)
{
  vw_send(name,length);
  vw_wait_tx();

}
