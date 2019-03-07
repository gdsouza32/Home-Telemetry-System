#include <Time.h>
#include <VirtualWire.h>
#include <SD.h>

#define VCC 7
#define Data 6
#define GND 4

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

int menu_item = 0;
int newNode[15];
int system_id;

String room_names[15]; 

const int chipSelect = 4;

void displaymenu()
{
  digitalClockDisplay();
  Serial.println(F("Telemetry Project"));
  Serial.println(F("Menu options"));
  Serial.println(F("-------------------------"));
  Serial.println(F("T - set time"));
  Serial.println(F("a - add node to database"));
  Serial.println(F("x - delete node from database"));
  Serial.println(F("i - set system id"));
  Serial.println(F("c - dump to CSV"));
  Serial.println(F("h - dump status to html"));
  Serial.println(F("--------------------------"));
  Serial.println(F("w - start"));
  Serial.println(F("q - quit")); 
  Serial.println(F("--------------------------")); 
  return;

} 

int addNode()
{
  Serial.println(F("in addnode "));
  delay(5000);
  boolean gotit = false;
  int option;
  while (!gotit)
  {
    if (Serial.available() > 0)
    {
      // read the incoming byte:
      option = Serial.parseInt();
      Serial.println(F(" "));
      Serial.println(F("option= "));
      Serial.println(option);
      delay(5000);
      if (option > 0 && option < 16)
      {
        gotit = true;
        newNode[option] = 1;
        Serial.println(F("*** would like to add a node ***"));
        //delay(500);

      }
      Serial.print(F("   node "));
      Serial.println(F(" added"));
      delay(500);

    }
    else
    {
      Serial.print(option);
      Serial.print(F("   ** out of range - enter a number between 1 and 15 **"));
    }
  }
  // accept name of node
  String name;
  int finished = 0;
  int node_name = 0;
  char bytein;
  while(!finished)
  {

    if (Serial.available() > 0)
    {
      bytein = Serial.read();
      name += bytein;
      Serial.println(bytein);
    }
    else
    {
      finished = 1;
    }
  }

  room_names[option] = name;
  Serial.print(option);
  Serial.print(" ");
  Serial.println(room_names[option]);

  delay(2000);
}


int deleteNode()
{

  boolean gotit = false;
  int option;
  while (!gotit)
  {
    if (Serial.available() > 0)
    {
      // read the incoming byte:
      option = Serial.parseInt();
      Serial.println(F(" "));
      if (option > 0 && option < 16)
      {
        gotit = true;
        newNode[option] = 0;
        Serial.print(F("   node "));
        Serial.println(F(" deleted"));
        //delay(1000);

      }
      else
      {
        Serial.print(option);
        Serial.print(F("   ** out of range - enter a number between 1 and 15 **"));
      }
    }
  }
  return option;
}

int setSystemID()
{
  system_id= 15;
  Serial.println(F("setSystemID"));
  Serial.println(system_id); 
  return 1; 
} 


int dumpCSV()
{
  // start sd copy
  Serial.print(F("Initializing SD card..."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:


  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    // don't do anything more:

  }
  Serial.println(F("card initialized."));
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:

  dataString += String("node");
  dataString += ","; 
  dataString += String("place");
  dataString += ","; 
  dataString += String("humidity");
  dataString += ","; 
  dataString += String("temp");
  dataString += ","; 
  dataString += String("ligtht");
  dataString += ",";
  dataString += String("time");



  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println(F("error opening datalog.txt"));
  } 

  // end sd copy

  return 1;
} 
int dumpHTML()
{
  File dataFile;
  dataFile.println(F("<html>"));
  dataFile.println(F("<head>"));
  dataFile.println(F("<title>TEAPERATURE HUMIDITY AND LIGHT SENSORS</title>"));
  dataFile.println(F("<body>"));
  dataFile.println(F("<h1>CONDITIONS AT HOME</h1>"));
  dataFile.println(F("<h2>Dated: 29th May 2014</h2>"));
  dataFile.println(F("<table border='2'>"));
  dataFile.println(F("<tr><th>SYSTEM ID<th>Node ID<th>NODE NAME<th>Humidity<th>Temperature<th>Light</tr>"));
  dataFile.println(F("<tr><td>15<td>1<td> LOUNGE <td> 32<td>15<td> Off</tr>"));
  dataFile.println(F("<tr><td>15 <td>2<td> DINNING ROOM <td> NA<td> NA <td> NA</tr>"));
  dataFile.println(F("<tr><td>15<td>3<td>KITCHEN<td>37<td>25<td>255</tr>"));
  dataFile.println(F("<tr><td>15 <td>4<td> BEDROOM <td> NA<td> NA <td> NA</tr>"));
  dataFile.println(F("<tr><td>15 <td>5<td> HALL <td> NA<td> NA <td> NA</tr>"));
  dataFile.println(F("<tr><td> NA <td><td> -- Not Registered -- <td> NA<td> NA <td> NA</tr>"));
  dataFile.println(F("<tr><td> NA<td><td> -- Not Registered -- <td> NA<td> NA <td> NA</tr>"));
  dataFile.println(F("<tr><td> NA<td> <td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("<tr><td> NA<td><td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("<tr><td>NA <td><td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("<tr><td>NA <td> <td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("<tr><td>NA <td><td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("<tr><td> NA<td><td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("<tr><td>NA <td> <td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("<tr><td> NA<td> <td> -- Not Registered -- <td> NA<td> NA<td> NA</tr>"));
  dataFile.println(F("</table>"));
  dataFile.println(F("</body>"));
  dataFile.println(F("</html>"));

  Serial.println(F("dumpHTML"));  
  return 1;
}

int settime()
{
  if (Serial.available()) 
  {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet)
  {
    digitalClockDisplay();  
  }

  delay(1000);
  return 1;
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  int daylight = 3600;

  if(Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    pctime += daylight;
    if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
    }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}


void setup()
{
  Serial.begin(9600);  // The baudrate of the serial monitor is 9600

    setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
  delay(5000);
  settime();
  delay(5000);

  pinMode(VCC,OUTPUT);
  pinMode(GND,OUTPUT);
  digitalWrite(VCC,HIGH);
  digitalWrite(GND,LOW);

  vw_setup(2000);  // Bits per sec
  vw_set_rx_pin(Data);
  vw_rx_start();
  pinMode(10, OUTPUT);

  do
  {

    displaymenu();
    menu_item = 0;
  }
  while(menu_item>1);


}

void loop()
{

  if (Serial.available() > 0)     //ie a character has been received
  {  
    char incomingByte = Serial.read();     // read the latest byte:
    switch (incomingByte) {
    case 'a':            // if user enters 'a' then add a tag
      addNode();
      break;  
    case 'x':   
      deleteNode();    // if user enters 'x' then erase database
      break;     
    case 'i':           // if user enters 'i' then delete the last tag - superfluous?
      setSystemID();
      break;      
    case'c':            // if user enters 'c' then print the EEPROM database to console
      dumpCSV(); 
      break;      
    case'h':            // if user enters 'h' then set unix time (ten decimal digits)
      dumpHTML();  
      break;   
    case'T':            // if user enters 'T' then print the unix time
      settime();   
      break;    
    case 'w':
      Serial.println(F("string scan"));
      msgscan();
      Serial.println(F("stopped *****string scan"));
      displaymenu();
      break;      
    }
  } 
}

void msgscan()
{
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  uint8_t buf[buflen];
  int exitnow = 0;

  while(!exitnow)
  {
    if (Serial.available() > 0)
    {
      if(Serial.read() == 'q')
        exitnow = 1;

    }

    if (vw_get_message(buf, &buflen)) // check to see if anything has been received
    {
      int i;
      // Message with a good checksum received. 
      for (i = 0; i < buflen; i++)
      {
        Serial.print(i);
        Serial.print(buf[i]);
        // the received data is stored in buffer
      }
      Serial.println("");
    }

    int temp = buf[0];

    int nid = temp - 240;
    int hid = temp - nid;


    int err = 0 ;
    if(hid == 240)
    {
      if( newNode[nid] == 1)
      {
        Serial.print(F("System ID: "));
        Serial.println(hid,DEC);

        Serial.print(F("NODE ID: "));
        Serial.println(nid,DEC);
        Serial.print(F("Humidity: "));
        Serial.println(buf[2],DEC);

        Serial.print(F("Temperature: "));
        Serial.println(buf[4],DEC);
        Serial.println(F(""));

        Serial.println(F("light intensity: "));
        Serial.println(buf[6],DEC);
        Serial.println(F(""));
        delay(1000);
      }
      else
      {
        err = 1;
      }

    }

    else
    {
      err = 1;
    }
    if(err)
    {
      Serial.println("not recognized System ID:  ");

    }
  }
}
