import serial
import matplotlib.pyplot as plt
import numpy as np
import time

ser = serial.Serial('COM3', 9600)

xpoints = []
ypoints = []

try:
    while True:
        pot_value = ser.readline().decode().strip()
        
        try:
            pot_value = int(pot_value) #conver to integer value
            print("POT Value:", pot_value)

            xpoints.append(time.time()) 
            ypoints.append(pot_value)

            if len(xpoints) > 100:  
                xpoints.pop(0)
                ypoints.pop(0)

            # plotting
            plt.clf()
            plt.plot(xpoints, ypoints)
            plt.xlabel('Time (s)')
            plt.ylabel('Potentiometer Value')
            plt.title('Real-time Potentiometer Value')
            plt.pause(0.1) 
        except ValueError:
            print('Invalid pot value received')

except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed.")
except UnicodeDecodeError:
    print('Serial Error\n')
finally:
    ser.close()
    print("Serial connection closed.")
