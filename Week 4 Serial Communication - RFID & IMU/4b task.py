import serial
import time
import json

# Set your Arduino's serial port
arduino_port = 'COM7'  # Change to COM7 for your Arduino
rfid_port = 'COM6'      # Change to COM6 for your RFID reader
baud_rate = 9600

# Load authorized card IDs and servo angles from JSON
config_data = '''{
    "authorized_cards": ["0008089233", "♥"],
    "servo_angle": 180
}'''
config = json.loads(config_data)

def main():
    # Initialize the serial connection to the Arduino
    arduino = serial.Serial(arduino_port, baud_rate, timeout=1)
    time.sleep(2)  # Wait for the connection to establish
    
    # Initialize the serial connection to the RFID reader
    rfid = serial.Serial(rfid_port, baud_rate, timeout=1)
    time.sleep(2)  # Wait for the connection to establish

    last_card_time = time.time()  # Initialize the last detected card time

    while True:
        current_time = time.time()
        
        if rfid.in_waiting > 0:  # Check if the RFID reader has data
            card_id = rfid.readline().decode('utf-8').strip()  # Read the card ID
            card_id = card_id[1:]  # Remove the first character (control character)
            print(f"Card ID: {card_id} (ASCII: {[ord(c) for c in card_id]})")  # Debugging line
            
            # Reset the last card detection time
            last_card_time = current_time  

            # Check for empty card ID
            if not card_id:
                print("No card detected. Skipping...")
                continue  # Skip to the next iteration of the loop

            if card_id in config['authorized_cards']:
                print("Access granted.")
                arduino.write(f"A{config['servo_angle']}\n".encode())  # Send angle to Arduino
            else:
                print("Access denied.")
                arduino.write(b'D')  # Send 'D' to disallow control
        else:
            # Check if 5 seconds have passed since the last card was detected
            if current_time - last_card_time > 5:
                print("No card detected for 5 seconds. Sending 'D' to Arduino.")
                arduino.write(b'D')  # Send 'D' to Arduino to disallow control
                last_card_time = current_time  # Reset the last card time to avoid repeated sends

if __name__ == "__main__":
    main()
