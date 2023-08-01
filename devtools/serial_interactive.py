import serial

try:
    ser
except:
    ser = None

def start():
    global ser
    ser =  serial.Serial(
        port="/dev/ttyUSB0",
        baudrate=9600,
        bytesize=serial.EIGHTBITS,
        timeout=5,
        stopbits=serial.STOPBITS_ONE
    )
    return ser

def stop():
    global ser
    ser.close()
    return

def w(b):
    ser.write(b)

def r():
    return ser.read()

# w(b'H')
# r()
