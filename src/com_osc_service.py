import asyncio
import serial_asyncio
from pythonosc.udp_client import SimpleUDPClient

# Set up the OSC client (Reaper listens on 127.0.0.1, port 8000 by default)
OSC_IP = "127.0.0.1"
OSC_PORT = 8000
osc_client = SimpleUDPClient(OSC_IP, OSC_PORT)

# Define your serial port and baud rate
SERIAL_PORT = "COM5"  # Change for Linux/Mac (e.g., "/dev/ttyUSB0")
BAUD_RATE = 115200

fx_index = 1
track_index = 1
wahPowerParam = 56
wahValueParam = 57
cmd = ""

async def read_serial(loop):
    """ Asynchronously reads from the serial port and sends OSC messages. """
    reader, _ = await serial_asyncio.open_serial_connection(url=SERIAL_PORT, baudrate=BAUD_RATE)
    
    while True:
        data = await reader.readuntil(b"\n")  # Read until newline
        message = data.decode().strip()
        #print(f"Received from Serial: {message}")

        if message == "Event!":
            osc_client.send_message("/reaper/midi_cc", [74, int(1)])  # CC 74 example
        elif "Pedal mode:" in message:
            pedalMode = float(message.split()[-1])
            print(f"Wah Pedal Mode: {pedalMode}")

            cmd = f"/track/{track_index}/fx/{fx_index}/fxparam/{wahPowerParam}/value"
            print(cmd)
            osc_client.send_message(cmd, pedalMode)
        elif "Wah" in message:
            wahValue = float(message.split()[-1])
            if wahValue > 0.0:
                setValue = (wahValue) / 100
            else:
                setValue = 0.0
            print(f"Wah Value: {setValue}")
            cmd = f"/track/{track_index}/fx/{fx_index}/fxparam/{wahValueParam}/value"
            print(cmd)
            osc_client.send_message(cmd, setValue)
            


# Run the asyncio event loop
loop = asyncio.get_event_loop()
loop.run_until_complete(read_serial(loop))
loop.run_forever()
