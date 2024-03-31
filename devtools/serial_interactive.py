import os
import serial
import threading

try:
    ser
except:
    ser = None

try:
    thread
except:
    thread = None

DEV_FP = "/dev"

def start(
    port=None,
    baudrate=921600,
    bytesize=serial.EIGHTBITS,
    timeout=5,
    stopbits=serial.STOPBITS_ONE
):
    global ser
    ports = [
         f"{DEV_FP}/{d}"
         for d in os.listdir(f"{DEV_FP}/")
         if d.startswith("ttyUSB") or d.startswith("ttyACM")
     ]
    if port is None:
        print(f"Found ports: {ports}")
        for port in ports:
            try:
                ser = serial.Serial(port=port, baudrate=baudrate, bytesize=bytesize, timeout=timeout, stopbits=stopbits)
                print(f"Attached to serial port {port}")
                break
            except serial.SerialException:
                continue
            except:
                break
        return ser
    else:
        ser = serial.Serial(port=port, baudrate=baudrate, bytesize=bytesize, timeout=timeout, stopbits=stopbits)
        return ser

def stop():
    global ser
    ser.close()
    ser = None
    return

def w(b):
    ser.write(b)

def r():
    return ser.read()

def rl():
    return ser.readline()

def loop_rl():
    if ser is None:
        print("ERROR: Need to init serial connection first, call start()")
        return
    print("starting loop_rl")
    while True:
        try:
            line = rl()
            if line != b"":
                print(line)
        except Exception as e:
            if ser is None:
                break
            else:
                raise e
    print("stopping loop_rl")

def loop_rl_bt():
    """Run loop_rl in a background thread"""
    thread = threading.Thread(target=loop_rl)
    thread.start()
    return thread

# w(b'H')
# r()
