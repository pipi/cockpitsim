/*
// Sample C code - customize as your Beck IPC application
*/

#include <i2c.h>

//initialization of I2C bus
int i2c_init()
{
	I2C_init();
   I2C_release();

   return 0;
}

int i2c_write(unsigned char slave, const char far *buffer, int length)
{
	int ret;
   ret = I2C_transmit_block(slave, buffer, length);
   I2C_release();
	return (ret < 0 ? -1 : 0);
}


int i2c_read(unsigned char slave, char far *buffer, int length)
{
   int ret;
   ret = I2C_receive_block(slave, buffer, length);
	I2C_release();
	return (ret < 0 ? -1 : 0);
}
void i2c_destroy()
{
    I2C_release();
}
