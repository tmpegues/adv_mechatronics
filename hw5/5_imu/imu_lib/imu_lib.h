#ifndef IMU_LIB
#define IMU_LIB
#include <stdbool.h>


bool init_imu(void);
void read_all(void);
void calc_acc(void);

float acc_x(void);
float acc_y(void);
float acc_z(void);

#endif