import os
import serial

try:
    ser
except:
    ser = None

DEV_FP = "/dev"

def start():
    global ser
    ports = [f"{DEV_FP}/{d}" for d in os.listdir(f"{DEV_FP}/") if d.startswith("ttyUSB")]
    print(f"Found ports: {ports}")
    for port in ports:
        try:
            ser = serial.Serial(
                port=port,
                baudrate=9600,
                bytesize=serial.EIGHTBITS,
                timeout=5,
                stopbits=serial.STOPBITS_ONE
            )
            print(f"Attached to serial port {port}")
            break
        except SerialException:
            continue
        except:
            break
    return ser

def stop():
    global ser
    ser.close()
    return

def w(b):
    ser.write(b)

def r():
    return ser.read()

def rl():
    return ser.readline()

def loop_rl():
    while True:
        print(rl())

# w(b'H')
# r()
