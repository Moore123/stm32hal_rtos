#ifndef __TIPS_H__
#define __TIPS_H__

#define TRUE 1
#define FALSE 0

#include "common.h"

#define Calloc(n, t)   (t *) calloc( (size_t) (n), sizeof(t) )
#define asizeof(a)     (int)(sizeof (a) / sizeof ((a)[0]))

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

/* Is character a digit */
#define GPS_IS_DIGIT(x)	((x) >= '0' && (x) <= '9')
#define GPS_IS_NUM(x)	(GPS_IS_DIGIT(x) || ((x) == '.') )
#define GPS_IS_HEX(x)   (GPS_IS_DIGIT(x) || ((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F'))
#define GPS_IS_ALPHA(x)	(((x) >= 'A' && (x) <= 'Z')||((x) >= 'a' && (x) <= 'z'))
#define GPS_IS_PRINT(x) ((GPS_IS_NUM(x)) || GPS_IS_ALPHA(x) )
#define GPS_IS_XPRINT(x)	((x) >= 0x20 && (x) <= 0x7F )

/* Char 2 digit conversions */
#define GPS_C2N(a)				(((a) - 48))
#define GPS_C2NM(a, x)			C2N(a) * (x)
#define GPS_CONCAT(x, y)		((x) << 5 | (y))

typedef double timestamp_t;	/* Unix time in seconds with fractional part */
typedef struct __gps_fix_t {
	timestamp_t time;	/* Time of update */
	int mode;		/* Mode of fix */
#define MODE_NOT_SEEN	0	/* mode update not seen yet */
#define MODE_NO_FIX	1	/* none */
#define MODE_2D  	2	/* good for latitude/longitude */
#define MODE_3D  	3	/* good for altitude/climb too */
	double ept;		/* Expected time uncertainty */
	double latitude;	/* Latitude in degrees (valid if mode >= 2) */
	double epy;		/* Latitude position uncertainty, meters */
	double longitude;	/* Longitude in degrees (valid if mode >= 2) */
	double epx;		/* Longitude position uncertainty, meters */
	double altitude;	/* Altitude in meters (valid if mode == 3) */
	double epv;		/* Vertical position uncertainty, meters */
	double track;		/* Course made good (relative to true north) */
	double epd;		/* Track uncertainty, degrees */
	double speed;		/* Speed over ground, meters/sec */
	double eps;		/* Speed uncertainty, meters/sec */
	double climb;		/* Vertical speed, meters/sec */
	double epc;		/* Vertical speed uncertainty */
} gps_fix_t;

typedef struct __s_tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;		//0 - 11
	int tm_year;		/*since 1900 */
	int tm_wday;
	int tm_yday;
} s_tm;

extern uint8_t verify_validly(const char *src);
extern uint8_t TM_GPS_INT_Atoi(char *str, uint32_t * val);
extern uint8_t TM_GPS_INT_Hex2Dec(char c);
extern uint8_t get_rmc_data(char *src, gps_fix_t *gv) ;

extern void timer_setup(void) ;
extern void delay_100us(unsigned int) ;

#endif
