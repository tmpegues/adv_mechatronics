#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../../pegues_lib/pegues_lib.h"
#include "ina219.h"
#include "as5600.h"
#include "hx711.h"

#define IN1 18
#define IN2 19

#define PWMDIV 7.5
#define WRAP 1000

void init_hbridge()
{
    gpio_set_function(IN1, GPIO_FUNC_PWM);
    gpio_set_function(IN2, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(IN1); // With my setup, they should be the same slice, but I'll act like they're not anyway
    uint slice_num2 = pwm_gpio_to_slice_num(IN2);
    pwm_set_clkdiv(slice_num1, PWMDIV);
    pwm_set_clkdiv(slice_num2, PWMDIV);
    pwm_set_wrap(slice_num1, WRAP);
    pwm_set_wrap(slice_num2, WRAP);
    pwm_set_enabled(slice_num1, true);
    pwm_set_enabled(slice_num2, true);
    pwm_set_gpio_level(IN1, WRAP);
    pwm_set_gpio_level(IN2, WRAP);
}

void set_bridge(int duty)
{
    // printf("%d\n", duty);
    if (duty > 0)
    {
        // printf("Positive");
        pwm_set_gpio_level(IN1, WRAP);
        pwm_set_gpio_level(IN2, WRAP - duty);
    }
    else if (duty < 0)
    {
        // printf("Negative");
        pwm_set_gpio_level(IN1, WRAP + duty);
        pwm_set_gpio_level(IN2, WRAP);
    }
    else
    {
        pwm_set_gpio_level(IN1, WRAP);
        pwm_set_gpio_level(IN2, WRAP);
    }
}

// PI current controller, runs in a 1kHz timer interrupt
#define NSAMP 400 // samples to log per test sweep

float kp = 0.1;  // tune
float ki = 0.05; // tune

volatile float desired_current = 0; // mA, the current loop setpoint
volatile float current_integral = 0;
volatile int ctrl_state = 0; // 0 idle, 1 running test sweep (not needed for final controller, but useful for testing/debugging)
volatile int ctrl_counter = 0;
volatile float desired_log[NSAMP];
volatile float actual_log[NSAMP];

// one PI step: given the measured current, return the duty to command
int pi_current_step(float actual)
{
    float error = desired_current - actual;
    current_integral += error;

    // anti-windup: keep the integral term inside the duty range
    float integral_max = WRAP / ki;
    if (current_integral > integral_max)
    {
        current_integral = integral_max;
    }
    else if (current_integral < -integral_max)
    {
        current_integral = -integral_max;
    }

    float u = kp * error + ki * current_integral;
    int duty = (int)u; // replaces the int duty = 0 line from previous version.
    // instead of manually setting duty, duty is now set by the PI controller output u (calculated from the error and integral terms). *Verify this logic is correct*
    // this allows the controller to adjust the duty cycle based on how far off the actual current is from the desired current, and how long it has been off (integral term).
    if (duty > WRAP)
    {
        duty = WRAP;
    }
    else if (duty < -WRAP)
    {
        duty = -WRAP;
    }
    return duty;
}

// 1kHz current loop, inspo from HW16 square-wave test sweep. Helpful for tuning kp, ki.
bool current_loop_cb(struct repeating_timer *t)
{
    if (ctrl_state == 1)
    {
        float actual = read_ina219();
        set_bridge(pi_current_step(actual));

        if (ctrl_counter < NSAMP)
        {
            desired_log[ctrl_counter] = desired_current;
            actual_log[ctrl_counter] = actual;
        }
        ctrl_counter++;

        if ((ctrl_counter % 100) == 0)
        {
            desired_current = -desired_current; // flip sign to make a square wave
        }
        if (ctrl_counter >= NSAMP)
        {
            ctrl_state = 0;
            set_bridge(0);
            ctrl_counter = 0;
            current_integral = 0;
        }
    }
    else if (ctrl_state == 2)
    {
        // haptic run: track desired_current set by the PD loop
        set_bridge(pi_current_step(read_ina219()));
    }
    return true; // keep repeating
}

// PD position/force controller -> desired current, runs at ~80Hz (load cell rate)
#define PD_HZ 80
#define POS_CENTER 2650    // neutral paddle position in encoder counts (0..4095), tune
#define MAX_CURRENT 500.0f // mA, hard clamp on the PI setpoint

float kp_pd = 1;     // tune: mA per encoder count of position error
float kd_pd = 0.000; // tune: mA per (count/s) of velocity (damping)
float kf = 0.0;      // tune: mA per raw HX711 count of user-applied force
                     //   kf > 0: motor amplifies user push (power assist)
                     //   kf < 0: motor opposes user push (renders stiffness / wall)
                     //   flip sign if load cell is mounted in the opposite direction

// map measured position, velocity, and user-applied force to a desired current.
// position spring + velocity damping hold a virtual equilibrium; the force term
// lets the load cell reading directly modulate the motor output so the device
// feels compliant (kf > 0) or resistive (kf < 0) to the user's touch.
float pd_controller(int position, float velocity, float force)
{
    float error = (float)(POS_CENTER - position);
    float desired = -kp_pd * error - kd_pd * velocity + kf * force;
    printf("pos error, desired: %f %f\n", error, desired);

    // clamp so the PI inner loop is never asked for more than MAX_CURRENT
    if (desired > MAX_CURRENT)
        desired = MAX_CURRENT;
    if (desired < -MAX_CURRENT)
        desired = -MAX_CURRENT;

    return desired; // mA, consumed by the PI current loop
}

int main()
{
    await_usb(0, true);
    breathe_led(0);

    init_ina219();
    init_as5600();
    init_hx711();
    init_hbridge();

    // run the current loop at 1kHz (negative period = fixed 1000us rate)
    struct repeating_timer timer;
    add_repeating_timer_us(-1000, current_loop_cb, NULL, &timer);

    char c = 0;
    while (true)
    {
        printf("send 'a' for PI current test, 'h' for haptic run\n");
        scanf(" %c", &c);
        if (c == 'a')
        {
            desired_current = 100; // mA target, tune as needed
            current_integral = 0;
            ctrl_counter = 0;
            ctrl_state = 1;
            while (ctrl_state == 1)
            {
                tight_loop_contents();
            }
            for (int i = 0; i < NSAMP; i++)
            {
                printf("%d %.2f %.2f\n", i, desired_log[i], actual_log[i]);
            }
        }
        else if (c == 'h')
        {
            // PD position loop sets desired_current at ~80Hz, PI loop tracks it
            int prev_position = read_as5600();
            absolute_time_t prev_time = get_absolute_time();
            int32_t force_offset = read_hx711(); // tare the load cell at rest
            current_integral = 0;
            ctrl_state = 2;
            printf("haptic run, send 's' to stop\n");
            while (true)
            {
                // the load cell read blocks until the next HX711 sample, pacing the loop at ~80Hz
                float force = (float)(read_hx711() - force_offset);
                int position = read_as5600();
                absolute_time_t now = get_absolute_time();
                float dt = absolute_time_diff_us(prev_time, now) / 1e6f;
                float velocity = (position - prev_position) / dt;
                desired_current = pd_controller(position, velocity, force);
                prev_position = position;
                prev_time = now;

                if (getchar_timeout_us(0) == 's')
                {
                    break;
                }
            }
            ctrl_state = 0;
            desired_current = 0;
            set_bridge(0);
        }
        else if (c == 'x')
        {
            int prev_position = read_as5600();
            absolute_time_t prev_time = get_absolute_time();
            int32_t force_offset = read_hx711(); // tare the load cell at rest
        }
    }
}
