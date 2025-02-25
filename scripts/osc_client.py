import serial
from pythonosc import udp_client


# Set up the serial port
serial_port = "COM3"  # Replace with your port name
baud_rate = 9600
ser = serial.Serial(serial_port, baud_rate)


# Continuously read from serial and send to REAPER via OSC
while True:
    data = ser.readline().decode('utf-8').strip()
    if data == "Event!":
        print("Sending event to REAPER")