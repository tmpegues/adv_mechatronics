from machine import Pin, PWM
from utime import sleep

pin = Pin("LED", Pin.OUT)
servo_pin = Pin(16)
servo_pwm = PWM(servo_pin, freq = 50)

def servo_angle(servo: Pin, duty: float, freq=50):
    """
    Set the angle of the servo by providing duty percent as a float.

    Args:
    ----
    servo (Pin): The pin that the servo is on.
    duty (float): Duty percent.
    freq (int): PWM frequency in Hz. Defaults to 50.
    """
    PWM(servo, freq=freq, duty_u16=int((duty/100 * 65535)))


print("LED starts flashing...")
while True:
    try:
        pin.toggle()
        sleep(1) # sleep 1sec
    except KeyboardInterrupt:
        break
pin.off()
print("Finished.")
