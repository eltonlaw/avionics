import os
import serial
import sys
import threading

try:
    state
except:
    state = {}

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
    global state
    state["ser"].close()
    state["out"].close()
    state = {}
    if sys.stdout.name != "<stdout>":
        sys.stdout.close()
    return

def start(profile=None, stdout=None):
    global state
    if state != {}:
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
            state["ser"] = serial.Serial(**conn_args)
            state["out"] = open(stdout, "w") if stdout else sys.stdout
            print(f"Attached to serial port {port}")
            break
        except serial.SerialException:
            continue
        except Exception as e:
            print(e)
            break
    if state == {}:
        print("ERROR: Couldn't find a port to connect to")
        return

    return state

def w(b):
    state["ser"].write(b)

def r():
    return state["ser"].read()

def rl():
    return state["ser"].readline()

def loop_rl():
    if state["ser"] is None:
        print("ERROR: Need to init serial connection first, call start()")
        return
    print("starting loop_rl")
    while True:
        try:
            line = rl().decode()
            if line != b"":
                state["out"].write(line)
        except Exception as e:
            if state["ser"] is None:
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
