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

DEVICE_FP = "/dev"

device_profiles = {
    "stlink-stm32g070rb": {
        'devices': ["ttyACM"],
        'baudrate': 921600,
        'bytesize': serial.EIGHTBITS,
        'timeout': 5,
        'stopbits': serial.STOPBITS_ONE,
    },
    "ft232_sam-m10q": {
        'devices': ["ttyUSB"],
        'baudrate': 9600,
        'bytesize': serial.EIGHTBITS,
        'timeout': 5,
        'parity': serial.PARITY_NONE,
        'stopbits': serial.STOPBITS_ONE,
    }
}

def stop():
    global ser
    ser.close()
    ser = None
    return

def start(profile=None):
    global ser
    if ser is not None:
        print("WARN: Serial already initialized, closing previous serial connection first")
        stop()
    if profile is None:
        for i, profile_name in enumerate(device_profiles.keys()):
            print(f"{i}: {profile_name}")
        user_input = input("Enter index (default=0): ")
        if user_input == "":
            index = 0
        else:
            index = int(user_input)
        profile = list(device_profiles.keys())[index]
    print(f"Device Profile: {profile}")
    conn_args = device_profiles[profile].copy()
    devices_requested = conn_args.pop("devices")
    ports_matched = [
         f"{DEVICE_FP}/{d_found}"
        for d_found in os.listdir(f"{DEVICE_FP}/")
        if any(d_found.startswith(d_req) for d_req in devices_requested)
    ]
    if len(ports_matched) == 0:
        print("ERROR: No ports found")
        return

    print(f"Found ports: {ports_matched}")
    for port in ports_matched:
        try:
            conn_args["port"] = port
            print(conn_args)
            ser = serial.Serial(**conn_args)
            print(f"Attached to serial port {port}")
            break
        except serial.SerialException:
            continue
        except:
            break
    if ser is None:
        print("ERROR: Couldn't find a port to connect to")
    return ser

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
