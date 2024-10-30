import matplotlib.pyplot as plt
import serial

# Initialize plot
plt.ion()  # Interactive mode on for live updating plot
fig, ax = plt.subplots()
x_data, y_data = [], []

# Connect to the correct serial port
ser = serial.Serial('COM4', 9600)

while True:
    data = ser.readline().decode('utf-8').strip()
    
    if data.startswith("Detected Gesture: "):
        gesture = data.split(": ")[1]
        
        if gesture == "Gesture 1":
            print("Action for Gesture 1")
            # Example: Gesture 1 - Move Right
            ax_data, ay_data = 1, 0  # Example movement, replace with actual data
        elif gesture == "Gesture 2":
            print("Action for Gesture 2")
            # Example: Gesture 2 - Move Left
            ax_data, ay_data = -1, 0  # Example movement, replace with actual data
        elif gesture == "Gesture 3":
            print("Action for Gesture 3")
            # Example: Gesture 3 - Move Up
            ax_data, ay_data = 0, 1  # Example movement, replace with actual data
        elif gesture == "Gesture 4":
            print("Action for Gesture 4")
            # Example: Gesture 4 - Move Down
            ax_data, ay_data = 0, -1  # Example movement, replace with actual data
        else:
            ax_data, ay_data = 0, 0  # No movement or gesture detected

    # Append new data to plot path
    x_data.append(ax_data)
    y_data.append(ay_data)
    ax.plot(x_data, y_data, 'b-')
    
    # Redraw plot for real-time update
    plt.draw()
    plt.pause(0.01)
