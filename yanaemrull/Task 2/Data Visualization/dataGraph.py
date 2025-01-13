import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib

# Use an interactive backend
matplotlib.use('TkAgg')

# Initialize Serial connection
try:
    ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)
    ser.flush()
except serial.SerialException as e:
    print(f"Error: Unable to open serial port: {e}")
    exit()

# Data storage
ultrasonic_data = []
potentiometer_data = []
time_data = []

# Live plotting function
def update(frame):
    global ultrasonic_data, potentiometer_data, time_data

    # Read Serial data
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        print(line)

        # Parse the data
        if "Ultrasonic" in line and "Potentiometer" in line:
            try:
                parts = line.split(", ")
                ultrasonic = float(parts[0].split(": ")[1].replace(" cm", ""))
                potentiometer = int(parts[1].split(": ")[1])

                # Append data for plotting
                time_data.append(len(time_data))
                ultrasonic_data.append(ultrasonic)
                potentiometer_data.append(potentiometer)

                # Limit data to last 100 points
                time_data = time_data[-100:]
                ultrasonic_data = ultrasonic_data[-100:]
                potentiometer_data = potentiometer_data[-100:]
            except (IndexError, ValueError) as e:
                print(f"Parsing error: {e}")

    # Update plot
    ax1.clear()
    ax2.clear()
    ax1.plot(time_data, ultrasonic_data, label="Ultrasonic (cm)", color='blue')
    ax2.plot(time_data, potentiometer_data, label="Potentiometer", color='orange')

    ax1.set_title("Ultrasonic Sensor")
    ax1.set_ylabel("Distance (cm)")
    ax1.legend(loc='upper right')

    ax2.set_title("Potentiometer")
    ax2.set_ylabel("Value")
    ax2.set_xlabel("Time (Frames)")
    ax2.legend(loc='upper right')

# Initialize plot
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
plt.tight_layout()

# Start animation
ani = FuncAnimation(fig, update, interval=500, cache_frame_data=False)
anim = ani  # Prevent garbage collection
plt.show()
