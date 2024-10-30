import usb.core
import usb.util

# Define your RFID reader's vendor and product IDs
vendor_id = 0x1234 # Replace with your RFID reader's vendor ID
product_id = 0x5678 # Replace with your RFID reader's product ID

# Authorized card IDs
authorized_cards = ["YourCardID1", "YourCardID2"]

# Initialize the USB RFID reader
dev = usb.core.find(idVendor=vendor_id, idProduct=product_id)
if dev is None:
    raise ValueError("RFID reader not found")

# Detach kernel driver if it's attached
if dev.is_kernel_driver_active(0):
    dev.detach_kernel_driver(0)

# Set the configuration of the device
dev.set_configuration()

# Define the endpoint
endpoint = dev[0][(0, 0)][0]

while True:
    try:
                        
        data = dev.read(endpoint.bEndpointAddress, endpoint.wMaxPacketSize)
        card_id = ''.join([chr(byte) for byte in data])
        
        if card_id in authorized_cards:
            print("Access granted. You can now control the servo.")
            # Send the 'A' signal to the Arduino
            ser.write(b'A')
        else:
            print("Access denied. Unauthorized card.")
            ser.write(b'D')
    except usb.core.USBError:
        pass