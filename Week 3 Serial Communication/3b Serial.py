import serial
import time

ser = serial.Serial('COM3', 9600)
time.sleep(2) 

try:
    while True:
        angle = input("Press Enter to read servo angle or 'q' to quit: ")
        if angle.lower() == 'q':
            ser.write(b'q') 
            break
        if ser.in_waiting > 0:
            position = ser.readline().decode('utf-8').strip()
            print(f"Current Servo Angle: {position} degrees")
except KeyboardInterrupt:
    pass
finally:
    ser.close() 
print("Serial connection closed.")
