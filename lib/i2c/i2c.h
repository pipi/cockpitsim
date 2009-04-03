
#ifndef _I2C_H_
#define _I2C_H_

#include <clib.h>
//initialiser le bus i2c
void i2c_init(void);
//detruire i2c
void i2c_destroy(void);
/*ecrire un block de données
slave-- I2C adresse
buffer--ou garder les donnees à envoyer
length--nombre de donnees à envoyer*/ 
int i2c_write(unsigned char slave, const char far *buffer, int length);
/*lire un block de données
slave-- I2C adresse
buffer--ou va enregistrer les donnees reçues
length--nombre de donnees reçues*/ 
int i2c_read(unsigned char slave, char far *buffer, int length);

#endif
