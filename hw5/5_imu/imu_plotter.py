import matplotlib.pyplot as plt
import serial
import time


def get_line() -> str:
    """
    Get serial line from serial.

    """
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
        gz.append(z)

fig, (ax1, ax2) = plt.subplots(1,2)


ax1.plot(ax, label = "Ax")
ax1.plot(ay, label = "Ay")
ax1.plot(az, label = "Az")
ax1.legend()
ax1.set_title('Accel')

ax2.plot(gx, label = "Gx")
ax2.plot(gy, label = "Gy")
ax2.plot(gz, label = "Gz")
ax2.legend()
ax2.set_title("Gyro")


plt.show()