import serial

ser = serial.Serial("COM6", 115200, timeout=1)

while True:
    print(ser.readline())