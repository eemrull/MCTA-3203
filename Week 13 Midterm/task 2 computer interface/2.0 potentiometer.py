import serial
import matplotlib.pyplot as plt
import time

# Configure the serial port
try:
    arduino = serial.Serial('COM5', 9600, timeout=1)  # Adjust COM port if necessary
    time.sleep(2)  # Wait for the connection to establish
    print("Arduino connected successfully.")
except serial.SerialException:
    print("Could not connect to Arduino. Check the COM port and connection.")
    exit()

# Initialize the plot
plt.ion()  # Interactive mode
fig, ax = plt.subplots()
data = []
timestamps = []

# Start reading and plotting data
try:
    while True:
        try:
            # Read a line from the Arduino
            line = arduino.readline().decode('utf-8').strip()
            print(f"Received: {line}")  # Debugging line: Print received data
            
            if line.isdigit():  # Only process numeric data
                pot_value = int(line)
                print(f"Potentiometer Value: {pot_value}")  # Debugging line: Print numeric value
                
                data.append(pot_value)
                timestamps.append(time.time() - timestamps[0] if timestamps else 0)

                # Limit to the last 100 data points
                if len(data) > 100:
                    data.pop(0)
                    timestamps.pop(0)

                # Update the plot
                ax.clear()
                ax.plot(timestamps, data, label='Potentiometer Value')
                ax.set_title("Real-Time Potentiometer Data")
                ax.set_xlabel("Time (s)")
                ax.set_ylabel("Value (0-1023)")
                ax.set_ylim(0, 1023)  # Match the Arduino ADC range
                ax.legend()
                plt.pause(0.1)
        except ValueError:
            print("Invalid data received:", line)
except KeyboardInterrupt:
    print("\nExiting...")

# Cleanup
arduino.close()
plt.ioff()
plt.show()
