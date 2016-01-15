#!/usr/bin/env python
 
import os, sys
import time
import requests
import serial, pprint
import urllib, httplib

# global variables

serialPort = '/dev/ttyACM0'
reopenSerial = True
ser = ""  # placeholder for serial handle

weatherData = {}
weatherData['FM'] = 0      # free memory on arduino
weatherData['IT'] = 0.0    # inside temp
weatherData['IH'] = 0.0    # inside humidity
weatherData['OT'] = 0.0    # outside temp
weatherData['OH'] = 0.0    # outside humidity


def openSerial(com_port): 
  global ser
  try:
    ser = serial.Serial(com_port, baudrate=9600, timeout=5)
    return True
  except serial.SerialException as e:
    print("Error: {}".format(e))
    return False


def readFromArduino():

    global serialPort
    global reopenSerial
    global ser

    if reopenSerial:
	print "Opening serial connection on port", serialPort
        if openSerial(serialPort):
            reopenSerial = False
            time.sleep(30)

    #print "Requesting data from arduino"

    try:
        ser.write("GU") # "Get Update"
    except serial.serialutil.SerialException as e:
        print("Failed to write request to arduino: {}".format(e))
        reopenSerial = True
        return False

    time.sleep(10) # wait for arduino to read sensors
   
    while ser.inWaiting():
        line = ser.readline()
        line = line.rstrip('\r\n')

        if (line == chr(0x02)):
            # STX
            pass
        elif (line == chr(0x03)):
            # ETX
            #print "Finished getting data from arduino"
            #print "Data from arduino: ",
            #pprint.pprint(weatherData)
            break;
        elif (line == ''):
            pass
        else:
            parts = line.split('\t')
            if (parts[0] =='FM'):
                weatherData['FM'] = int(parts[1])
            elif (parts[0] =='IT'): 
                weatherData['IT'] = float(parts[1])
            elif (parts[0] =='IH'): 
                weatherData['IH'] = float(parts[1])
            elif (parts[0] =='OT'): 
                weatherData['OT'] = float(parts[1])
            elif (parts[0] =='OH'): 
                weatherData['OH'] = float(parts[1])
            else:
                print "Unrecognized data from serial:", line

    return True


# main program entry point

def run():
  print "Starting pullpush script"

  time.sleep(5)
 
  while True:
    gotUpdatedData = readFromArduino()
    sys.stdout.flush() 
 
    if gotUpdatedData:

      # ThingSpeak

      site = 'api.thingspeak.com'
      url = '/update'
      values = { 'key':'SERVER-KEY-HERE',   'field1':weatherData['FM'], \
                 'field2':weatherData['IT'], 'field3':weatherData['IH'], \
                 'field4':weatherData['OT'], 'field5':weatherData['OH'] }
      params = urllib.urlencode(values)

      try:
        conn = httplib.HTTPConnection(site);
        conn.request("POST", url, params)
        content = conn.getresponse()
        #print 'Result of ThingSpeak push:', content.reason, content.status
        conn.close()   
      except:
        e = sys.exc_info()[0]
        print("Error sending to ThingSpeak: {}".format(e))
        print sys.exc_info()[:2]
 
    sys.stdout.flush() 
    time.sleep(120)

run()
ser.close()

