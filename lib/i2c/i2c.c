/*
// Sample C code - customize as your Beck IPC application
*/

#include <i2c/i2c.h>

int i2c_init()
{
	I2C_init();

   return 0;
}

int i2c_write(unsigned char slave, const char far *buffer, int length)
{
	return I2C_transmit_block(slave, buffer, length);
}

int i2c_read(unsigned char slave, char far *buffer, int length)
{
	return I2C_receive_block(slave, buffer, length);
}
void i2c_destroy()
{
    I2C_release();
}
