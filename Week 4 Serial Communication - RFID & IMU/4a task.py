import matplotlib.pyplot as plt
import serial

# Initialize plot
plt.ion()  # Interactive mode on
fig, ax = plt.subplots()
x_data, y_data = [], []

ser = serial.Serial('COM3', 9600)

while True:
    data = ser.readline().decode('utf-8').strip()
    
    if data.startswith("Detected Gesture: "):
        gesture = data.split(": ")[1]
        if gesture == "Gesture 1":
            print("Action for Gesture 1")
        elif gesture == "Gesture 2":
            print("Action for Gesture 2")

    # Assuming ax and ay data are available, plot them (replace with actual values)
    ax_data, ay_data = 0, 0  # Replace with actual x, y values from mpu.getMotion6
    x_data.append(ax_data)
    y_data.append(ay_data)
    ax.plot(x_data, y_data, 'b-')
    plt.draw()
    plt.pause(0.01)
