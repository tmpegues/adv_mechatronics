import pygame
import serial

ser = False
while not ser:
   try:
         ser = serial.Serial('/dev/ttyACM0', 115200, timeout=10)
   except serial.SerialException:
         print("No connect")

def get_line() -> str:
    """
    Get serial line from serial.

    """
    print('<--')
    return ser.readline().decode('utf-8').strip()


print('Opening port: ' + str(ser.name))
WIDTH = 600
HEIGHT = 600
n1_int = 0
n2_int = 0
def update():
   s = get_line()
   print(s)
   if s[0] == '(':
      result1 = s[s.find('(')+1:s.find(',')] # find everything beween ( and ,
      result2 = s[s.find(',')+1:s.find(')')] # find everything between , and )
      global n1_int
      n1_int = int(result1) # convert str to int
      global n2_int
      n2_int = int(result2)
def draw():
   screen.fill((0, 0, 0))
   screen.draw.text('force: ' + str(n1_int)+ '\n' + 'angle: ' + str(n2_int),(0, 0))