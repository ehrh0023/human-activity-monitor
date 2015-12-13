#Get acceleration data from Chronos watch.
import serial
import array
import csv
import struct 
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
       fieldnames = ['x', 'y', 'z', 'Time']#, '3', '4', '5', '6', '7']#, 'Seconds', 'Microseconds']
       writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
       writer.writeheader()

#ser.write(accDataRequest())
#accel = ser.read(7)
#firstx = accel[0]
#firsty = accel[1]
#firstz = accel[2]
prevx = 0
prevy = 0
prevz = 0
time = 0   
while True:
    #Send request for acceleration data
    ser.write(accDataRequest())
    accel  = ser.read(7)
 
    accelx = ord(accel[0])&0xFF
    accely = ord(accel[1])&0xFF
    accelz = ord(accel[2])&0xFF
    
    signx  = ((accelx&0x80) >> 7)
    signy  = ((accely&0x80) >> 7)
    signz  = ((accelz&0x80) >> 7)
    if signx == 1:
     accelx = ((~accelx&0xFF) + 1)*(-1)
    if signy == 1:
     accely = ((~accely&0xFF) + 1)*(-1)
    if signy == 1:
     accely = ((~accely&0xFF) + 1)*(-1)
     
    accelx = ((accelx*18*0.5) + (prevx*0.5))#- firstx)
    accely = ((accely*18*0.5) + (prevy*0.5))#- firsty)
    accelz = ((accely*18*0.5) + (prevz*0.5))#- firstz)
    prevx  = accelx
    prevy  = accely
    prevz  = accelz
    time = time + 8
    print("x: " + str(accelx)+ "y: " + str(accely) + "z: " + str(accelz) + "Time in ms: " + str(time))
    #Save Accel data to CSV file if coordinate is not 0
    with open('C:/Users/brian_000/Documents/GitHub/human-activity-monitor/Chronos/Chronos_ACCDATA/Accel.csv', 'a') as csvfile:
       fieldnames = ['x', 'y', 'z', 'Time']#, '3', '4', '5', '6', '7']#, 'Seconds', 'Microseconds']
       writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
       writer.writerow({'x': str(accelx), 'y': str(accely), 'z': str(accelz), 'Time': time})#, '3': str(ord(accel[3])), '4': str(ord(accel[4])), '5': str(ord(accel[5])), '6': str(ord(accel[6])), '7': str(ord(accel[2]))})#,'Seconds': duration.seconds, 'Microseconds': duration.microseconds})
ser.close()