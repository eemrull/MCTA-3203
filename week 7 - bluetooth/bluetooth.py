import serial 
import matplotlib.pyplot as plt

# Update this to your actual port and baud rate
port = 'COM6'  # Change to your port, e.g., 'COM6' or '/dev/ttyUSB0'
baud_rate = 9600

# Create a serial connection
try:
    ser = serial.Serial(port, baud_rate, timeout=1)  # Set timeout to avoid hanging
    print(f"Connected to {port} at {baud_rate} baud.")
except Exception as e:
    print(f"Error: {e}")
    exit()

temperatures = []

try:
    while True:
        data = ser.readline().decode('utf-8').strip()  # Read and clean the data
        if data:  # Check if data is not empty
            try:
                temperature = float(data)
                temperatures.append(temperature)
                print(f"Temperature: {temperature} °C")
            except ValueError:
                print(f"Invalid data: {data}")  # Handles any non-numeric data
        else:
            print("No data received from serial.")
        
except KeyboardInterrupt:
    print("\nKeyboardInterrupt received, stopping the script.")
finally:
    ser.close()
    if len(temperatures) > 0:
        plt.plot(temperatures, marker='o', linestyle='-')
        plt.title('Temperature Monitoring')
        plt.xlabel('Time (s)')
        plt.ylabel('Temperature (°C)')
        plt.show()
    else:
        print("No temperature data recorded to plot.")
