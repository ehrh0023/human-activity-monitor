#Get acceleration data from Chronos watch.
import serial
#import time
import array
import csv
import datetime
#import time
 
def startAccessPoint():
    return array.array('B', [0xFF, 0x07, 0x03]).tostring()
 
def accDataRequest():
    return array.array('B', [0xFF, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00]).tostring()
  


#Open COM port 6 
#argments are 5 (COM6), 115200 (bit rate), and timeout is set so
ser = serial.Serial(3,115200,timeout=1)
 
#Start access point
ser.write(startAccessPoint())

with open('C:/Users/brian_000/Documents/GitHub/human-activity-monitor/Chronos/Chronos_ACCDATA/Accel.csv', 'a') as csvfile:
       fieldnames = ['x', 'y', 'z', 'Seconds', 'Microseconds']
       writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
       writer.writeheader()
       
now = datetime.datetime.now()
while True:
    #Send request for acceleration data
    ser.write(accDataRequest())
    accel = ser.read(7)
    duration = datetime.datetime.now() - now
    print("x: " + str(ord(accel[0])) + "y: " + str(ord(accel[1])) + "z: " + str(ord(accel[2]))) 
    #Save Accel data to CSV file if coordinate is not 0
    with open('C:/Users/brian_000/Documents/GitHub/human-activity-monitor/Chronos/Chronos_ACCDATA/Accel.csv', 'a') as csvfile:
       fieldnames = ['x', 'y', 'z', 'Seconds', 'Microseconds']
       writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
       writer.writerow({'x': str(ord(accel[0])), 'y': str(ord(accel[1])), 'z': str(ord(accel[2])),'Seconds': duration.seconds, 'Microseconds': duration.microseconds})
ser.close()