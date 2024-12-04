import serial
import matplotlib.pyplot as plt
ser = serial.Serial('COMx', 9600) # adjust as needed
temperatures = []
try:
    while True:
        data = ser.readline().decode('utf-8').strip()
        temperature = float(data)
        temperatures.append(temperature)
        # Display real-time temperature
        print(f"Temperature: {temperature} °C")
except KeyboardInterrupt:
 # Plot the recorded temperatures when the user interrupts the script
 plt.plot(temperatures, marker='o')
 plt.title('Temperature Monitoring')
 plt.xlabel('Time (s)')
 plt.ylabel('Temperature (°C)')
 plt.show()
finally:
 ser.close()