from machine import Pin, PWM
from utime import sleep

pin = Pin("LED", Pin.OUT)

duty_100 = 65535
duty_min = 0.024 * duty_100
duty_max = 0.128 * duty_100

servo_pin = Pin(16)


def servo_angle(servo: Pin, angle: float, freq=50):
    """
    Set the angle of the servo.

    Args:
    ----
    servo (Pin): The pin that the servo is on.
    angle (float): Desired angle to rotate to.
    freq (int): PWM frequency in Hz. Defaults to 50.
    """
    duty = int((angle/180) * (duty_max-duty_min) + duty_min)
    print(angle)
    PWM(servo, freq=freq, duty_u16=duty)


angle = 0
sign = 1
while True:
    try:
        if sign > 0:
            actual_angle = angle
        elif sign < 0:
            actual_angle = 180 - angle
        servo_angle(servo_pin, actual_angle)
        sleep(.01) # sleep 1sec
    except KeyboardInterrupt:
        break
    if angle <= 0:
        sign *= -1
    elif angle >= 180:
        sign *= -1
        angle = 0
    angle += 1




pin.off()
print("Finished.")
