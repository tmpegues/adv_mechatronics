#ifndef PEGUES_I2C
#define PEGUES_I2C

void init_i2c();

void write_i2c(unsigned char address, unsigned char reg, unsigned char value);

unsigned char read_i2c(unsigned char address, unsigned char reg);
#endif