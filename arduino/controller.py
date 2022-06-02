from time import sleep
import serial

ser = serial.Serial('COM4')

packet = bytearray()
packet.append(0x00)
packet.append(0x7A)
packet.append(0xFF)
sleep(2)
ser.write(packet)
s = ser.read_all()
print(s)
ser.close()