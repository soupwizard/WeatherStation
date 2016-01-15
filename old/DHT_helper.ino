void setupDHT() {
  dhtInside.begin();
  dhtOutside.begin();
}

float getDhtTemperature(DHT dht, String dhtName) {
  
  delay(2000);  // delay before reading in case we just read sensor previously (it's a slow sensor)
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float t = dht.readTemperature(DHT_USE_FAHRENHEIT);

  // If the sensor is faulty or misconnected, read will return a value of 
  // NAN (not a number) instead of a valid value.  Can test for it using "isnan(t)"
  return t;
}

float getDhtHumidity(DHT dht, String dhtName) {
 
  delay(2000);  // delay before reading in case we just read sensor previously (it's a slow sensor)
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

  // If the sensor is faulty or misconnected, read will return a value of 
  // NAN (not a number) instead of a valid value.  Can test for it using "isnan(t)"
  return h;
}

String dhtFloatToStr(float value) {
  char tempStr[16];
  dtostrf(value, 1, 1, tempStr);
  return (String) tempStr;
}

