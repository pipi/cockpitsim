#include <clib.h>
#define SDA_PIN 1	//set PIO1 as SDA
#define SCL_PIN 0	//set PIO0 as SCL
void init_i2c(void);
int I2C_write(unsigned char slave, char char_out);
int I2C_read(unsigned char slave, char far *buffer, int length);