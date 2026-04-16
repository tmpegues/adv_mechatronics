import matplotlib.pyplot as plt
import serial
import time


def get_line() -> str:
    """
    Get serial line from serial.

    """
    print('<--')
    return ser.readline().decode('utf-8').strip()




while True:
    try:
        ser = serial.Serial('/dev/ttyACM0')
        break
    except:
        pass

print('Opening port: ')
print(ser.name)

print("Listening")
ax = []
ay = []
az = []

gx = []
gy = []
gz = []

ser.read_until(b'\r\n\r\n')
print("Reading")
while len(ax) < 200:
    print(len(ax))
    x = 0
    y = 0
    z = 0
    line = get_line().split()
    if line[0] == "A":
        x, y, z = list(map(float, get_line().split()))
        ax.append(x)
        ay.append(y)
        az.append(z)
    if line[0] == "G":
        x, y, z = list(map(float, get_line().split()))
        gx.append(x)
        gy.append(y)
        gz.append(x)

plt.plot(ax, label = "Ax")
plt.legend()
plt.show()