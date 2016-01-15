
/*
 * WeatherStation 1.0, Jeff Davis soupwizard@gmail.com
 *
 */

// include adafruit DHTxx library
// downloaded from https://github.com/adafruit/DHT-sensor-library
#include <DHT.h> 

// library to calculate memory free on arduino
// downloaded from http://playground.arduino.cc/Code/AvailableMemory
#include <MemoryFree.h>

// ascii table constants
#define ASCII_HEX_STX  0x02
#define ASCII_HEX_ETX  0x03 
#define ASCII_HEX_US   0x1F
#define ASCII_HEX_RS   0x1E

/************************************************************************************
 *** Pin Usage Defines                                                            ***
 ************************************************************************************/

// pin usage defs for Uno R3

#define PIN_LED_ONBOARD	         13
#define PIN_DHT_INSIDE            7
#define PIN_DHT_OUTSIDE           8  

/************************************************************************************
 *** Program Globals                                                              ***
 ************************************************************************************/

unsigned long loopCtr = 0;  // loop counter

/************************************************************************************
 *** Component Defines                                                            ***
 ************************************************************************************/

/*******************************
 *** DHTxx Temp and Humidity ***
 *******************************/
// Avail types are: DHT11, DHT21, DHT22, AM2301

#define DHT_USE_FAHRENHEIT true

#define DHT_INSIDE_NAME "Inside"
DHT dhtInside(PIN_DHT_INSIDE, DHT11);
float insideTemperature;
float insideHumidity;

#define DHT_OUTSIDE_NAME "Outside"
DHT dhtOutside(PIN_DHT_OUTSIDE, AM2301);
float outsideTemperature;
float outsideHumidity;

/************
 *** LEDs ***
 ************/
int ledOnboardLit = false;  // onboard LED (pin 13) will be toggled high or low each time loop() is called


/************************************************************************************
 ***                                                                              ***
 *** Arduino Library Required Function                                            ***
 ***                                                                              ***
 ************************************************************************************/

void setup()
{  
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
  while (!Serial) {}  // wait for Leonardo
  
  // Setup LEDs
  pinMode(PIN_LED_ONBOARD, OUTPUT);
  toggleLedOnboard();
  
  // Setup DHTxx devices
  setupDHT();
  
}

/**************************************************************/

void loop()
{
  loopCtr += 1;
  
  toggleLedOnboard();
  
  // Send sensor info to serial if asked
  String command = checkForSerialCmd();
  if (command == "GU") {
      checkSensors();
      serialSendStatus();
  }
  
  delay(100);

}

/************************************************************************************
 ***                                                                              ***
 *** Program Functions                                                            ***
 ***                                                                              ***
 ************************************************************************************/

void checkSensors() {
  
  /* DHTxx data */
  
  // inside
  insideTemperature = getDhtTemperature(dhtInside, DHT_INSIDE_NAME);
  insideHumidity    = getDhtHumidity(dhtInside, DHT_INSIDE_NAME);
  
  // outside
  outsideTemperature = getDhtTemperature(dhtOutside, DHT_OUTSIDE_NAME);
  outsideHumidity    = getDhtHumidity(dhtOutside, DHT_OUTSIDE_NAME);
}

String checkForSerialCmd() {
  
  String readString = "";
  
  while (Serial.available()) {  
    delay(2);  //delay to allow buffer to fill 
    if (Serial.available() > 0) {
      char c = Serial.read();  // gets one byte from serial buffer
      readString += c; // builds the string readString
    } 
  }

  return readString;  
}

void serialSendStatus() {
  
  Serial.write(ASCII_HEX_STX);  // start transmission block
  Serial.print('\n');
  
  Serial.print("FM");
  Serial.print('\t');
  Serial.print(freeMemory());
  Serial.print('\n');
  
  Serial.print("IT");
  Serial.print('\t');
  Serial.print(insideTemperature);
  Serial.print('\n');  
  
  Serial.print("IH");
  Serial.print('\t');
  Serial.print(insideHumidity);
  Serial.print('\n');  

  Serial.print("OT");
  Serial.print('\t');
  Serial.print(outsideTemperature);
  Serial.print('\n');  
  
  Serial.print("OH");
  Serial.print('\t');
  Serial.print(outsideHumidity);
  Serial.print('\n');  
  
  Serial.write(ASCII_HEX_ETX);  // end transmission block
  Serial.print('\n');

}

void toggleLedOnboard() {
  switch (ledOnboardLit) {
    case true: 
      ledOnboardLit = false;
      digitalWrite(PIN_LED_ONBOARD, LOW);
      break;
    case false:
      ledOnboardLit = true;
      digitalWrite(PIN_LED_ONBOARD, HIGH);
      break;
    default:
      break;
  }
}


