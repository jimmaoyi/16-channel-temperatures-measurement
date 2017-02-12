#ifndef __SPI_H
#define __SPI_H
			  	    													  
void SPI1_Init(void);			 //³õÊ¼»¯SPI¿Ú
void MF_delay(unsigned int ns);		

/* Constants for thermocouple readings */
#define MAX31855_TC_SCALE 0.25F
#define MAX31855_TC_SIGN_NEG (1UL << 31)
#define MAX31855_TC_MASK 0xFFFC0000
#define MAX31855_TC_SHIFT 18

/* Constants for internal sensor readings */
#define MAX31855_INT_SCALE 0.0625F
#define MAX31855_INT_SIGN_NEG  (1UL << 15)
#define MAX31855_INT_MASK   0xFFF0
#define MAX31855_INT_SHIFT  4

/* Does all the shifting and casting to produce a thermocouple or
 * internal reading from the raw 32-bit value provided by the module.
 */
#define MAX31855_TEMP_TC(raw_reading) \
    ((int16_t)((raw_reading & MAX31855_TC_MASK) >> MAX31855_TC_SHIFT))

#define MAX31855_TEMP_INT(raw_reading) \
    ((int16_t)((raw_reading & MAX31855_INT_MASK) >> MAX31855_INT_SHIFT))

/* Represents a return value from the therocouple.  Zero is
 * success.  Nonzero means that one or more faults have
 * occured.
 */
typedef uint8_t max31855_status;

/* Takes a reading from the temperature sensor and returns it
 * in either celsius, fahrenheit, or kelvin.  The internal and
 * thermocouple readings are passed back via pointers.  If
 * you only care about one reading and not the other, you
 * can simply pass in a NULL pointer for the one you aren't
 * interested in.
 */
max31855_status max31855_temp_cel(float *t_int, float *t_tc,uint8 channal);
max31855_status max31855_temp_fah(float *t_int, float *t_tc,uint8 channal);
max31855_status max31855_temp_kel(float *t_int, float *t_tc,uint8 channal);

#endif

