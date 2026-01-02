import serial
import time

PORT_SERIE = '/dev/ttyACM0' 
VITESSE = 9600

ser = serial.Serial(PORT_SERIE, VITESSE, timeout=1)
ser.flush() 
print("Wait for data")


while True: 
    if ser.in_waiting > 0:
        ligne = ser.readline().decode('utf-8', errors='ignore').strip()
        
        data = ligne.split(" | ")
        
        accx = data[0] 
        accy = data[1] 
        accz = data[2]
        gyx = data[3]
        gyy = data[4]
        gyz = data[5]

        Accx = float(accx) / 16384.0
        Accy = float(accy) / 16384.0
        Accz = float(accz) / 16384.0
        
        
        GyX = float(gyx) / 131.0
        GyY = float(gyy) / 131.0
        GyZ = float(gyz) / 131.0


        print(f"X={GyX}°/s | Y={GyY}°/s | Z={GyZ}°/s ")


