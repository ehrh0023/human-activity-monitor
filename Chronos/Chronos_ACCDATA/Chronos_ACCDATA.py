#Get acceleration data from Chronos watch.
import serial
import time
import array
import csv
 
def startAccessPoint():
    return array.array('B', [0xFF, 0x07, 0x03]).tostring()
 
def accDataRequest():
    return array.array('B', [0xFF, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00]).tostring()
 
#Open COM port 6 
#argments are 5 (COM6), 115200 (bit rate), and timeout is set so
ser = serial.Serial(3,115200,timeout=1)
 
#Start access point
ser.write(startAccessPoint())

with open('Accel.csv', 'a') as csvfile:
       fieldnames = ['x', 'y', 'z', 'Hours', 'Minutes', 'Seconds']
       writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
       writer.writeheader()

while True:
    #Send request for acceleration data
    ser.write(accDataRequest())
    accel = ser.read(7)
    
    #if ord(accel[0]) != 0 and ord(accel[1]) != 0 and ord(accel[2]) != 0:
    print("x: " + str(accel[0]) + "y: " + str(accel[1]) + "z: " + str(accel[2])) 
    #Save Accel data to CSV file if coordinate is not 0
    with open('Accel.csv', 'a') as csvfile:
       fieldnames = ['x', 'y', 'z', 'Hours', 'Minutes', 'Seconds']
       writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
       if accel[0] != 0 or accel[1] != 0 or accel[2] != 0:
        writer.writerow({'x': accel[0], 'y': accel[1], 'z': accel[2], 'Hours': time.strftime("%H"), 'Minutes': time.strftime("%M"), 'Seconds': time.strftime("%S")})
ser.close()