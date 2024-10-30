import serial
ser = serial.Serial('COM4', 9600)

while True:
    data = ser.readline().decode('utf-8').strip()
    if data.startswith("Detected Gesture: "):
    gesture = data.split(": ")[1]
    if gesture == "Gesture 1":
        # Perform an action for Gesture 1
        print("Action for Gesture 1")
    elif gesture == "Gesture 2":
        # Perform an action for Gesture 2
        print("Action for Gesture 2")
# Add more gesture actions as neededimport serial
