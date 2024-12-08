import serial
import time

# Replace 'COMx' with your HC-05 Bluetooth COM port
bluetooth_port = 'COM12'  # yana's laptop COM 12
baud_rate = 9600

# Initialize serial connection
try:
    bt_serial = serial.Serial(bluetooth_port, baud_rate, timeout=1)
    print("Bluetooth Connected!")
    bt_serial.write(b"Bluetooth Connected!\n")  # Send welcome message
except Exception as e:
    print(f"Error connecting to Bluetooth: {e}")
    exit()

def send_temperature_data(temperature):
    """
    Sends temperature data over Bluetooth.
    """
    message = f"Temperature: {temperature:.2f} °C\n"
    bt_serial.write(message.encode('utf-8'))
    print(f"Sent: {message.strip()}")

def process_command(command):
    """
    Processes commands received over Bluetooth.
    """
    command = command.strip().upper()
    if command == "FAN ON":
        print("Fan is ON")
        bt_serial.write(b"Fan is ON\n")
    elif command == "FAN OFF":
        print("Fan is OFF")
        bt_serial.write(b"Fan is OFF\n")
    elif command >= 30:
        print("Fan is ON")
        bt_serial.write(b"Fan is ON\n")
    elif command <= 25:
        print("Fan is OFF")
        bt_serial.write(b"Fan is OFF\n")    
    else:
        print("Unknown Command")
        bt_serial.write(b"Unknown Command\n")

try:
    while True:
        # Simulate reading a temperature sensor (replace with actual sensor logic if required)
        temperature = 25.0 + (time.time() % 10)  # Simulated temperature value
        send_temperature_data(temperature)

        # Check if a command is received
        if bt_serial.in_waiting > 0:
            command = bt_serial.readline().decode('utf-8').strip()
            print(f"Received: {command}")
            process_command(command)

        time.sleep(1)  # Delay for 1 second
except KeyboardInterrupt:
    print("Exiting program.")
finally:
    bt_serial.close()
    print("Bluetooth connection closed.")
