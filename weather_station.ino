#include <OneWire.h>
#include <DallasTemperature.h>

#include <Arduino.h>
#include <TM1637Display.h>

#include <Wire.h>
#include "RTClib.h"

#include <SoftwareSerial.h>

//RTC
RTC_DS1307 RTC;

// parallax lcd serial
const int TxPin = 10;
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// 4digit LED Module connection pins (Digital Pins)
#define CLK 8
#define DIO 9
TM1637Display display(CLK, DIO);

// globals for 4digit led module
uint8_t data[] = { };
char buffer[4];

char buffer32[32];
 
void setup(void)
{
  // start serial port
  Serial.begin(9600);

  // start lcd serial
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
  mySerial.begin(19200);

  // Start up the dallas library
  sensors.begin();

  // start rtc
  Wire.begin();
  RTC.begin();
 
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");

  } else {
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  

  display.setBrightness(0x0f);
}
 
 
void loop(void)
{
  // get teh time
  DateTime now = RTC.now();

  String timeStr = getDateTimeStr(now);

  Serial.println("time be:" + timeStr);

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");

  Serial.print("Temperature for Device 1 is: ");
  float temp1 = sensors.getTempFByIndex(0);
  int temperature1 = int(temp1 * 100);
  Serial.println(temperature1);

  // display temp on 4digit led panel
  itoa(temperature1, buffer, 10);
  /*
  data[0] = display.encodeDigit(buffer[0]);
  data[1] = display.encodeDigit(buffer[1]);
  data[2] = display.encodeDigit(buffer[2]);
  data[3] = display.encodeDigit(buffer[3]);
  display.setSegments(data);
  */

  // display temp on lcd serial display
  mySerial.write(12);                 // Clear    
  delay(100);         
  mySerial.write(17);                 // Turn backlight on
  delay(5);                           // Required delay
  mySerial.print(timeStr);  // First line
  // don't print form feed since printing timeStr contains newline
  //mySerial.write(13);                 // Form feed
  mySerial.print(buffer);   // Second line
  delay(7000);                        // Wait 3 seconds
  mySerial.write(18);                 // Turn backlight off
  
  delay(3000);
 
}

String getDateTimeStr(DateTime dateTime) {
  
 
  String theYear  = "2001";
  String theMonth = "1";
  String theDay   = "1"; 
  String theHour   = "5";
  String theMinute = "4";
  
  //if (rtcAvail) {
    theYear  = itoa(dateTime.year(), buffer32, 10);
    theMonth = itoa(dateTime.month(), buffer32, 10);
    theDay   = itoa(dateTime.day(), buffer32, 10);
    theHour   = itoa(dateTime.hour(), buffer32, 10);
    theMinute = itoa(dateTime.minute(), buffer32, 10);
  //}
  
  if (theMonth.length() < 2) theMonth = "0" + theMonth;  
  if (theDay.length()   < 2) theDay   = "0" + theDay; 
  if (theHour.length()   < 2) theHour   = "0" + theHour;
  if (theMinute.length() < 2) theMinute = "0" + theMinute; 
  
  return theYear + "/" + theMonth + "/" + theDay + " " + theHour + ":" +  theMinute;
}



