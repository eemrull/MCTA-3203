import serial

# Initialize Serial connection
ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)  # Replace 'COM3' with your port
ser.flush()

try:
    print("Reading sensor data from Arduino. Press Ctrl+C to stop.")
    while True:
        # Check if data is available in the Serial buffer
        if ser.in_waiting > 0:
            # Read and decode the line from Serial
            line = ser.readline().decode('utf-8').strip()
            print(line)  # Print the received data to the console

except KeyboardInterrupt:
    print("\nStopping data reading...")
finally:
    ser.close()  # Ensure the Serial connection is closed properly
