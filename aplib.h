/* APLIB_H */

#ifndef DAVELIB_APLIB_H
#define DAVELIB_APLIB_H

#ifdef _cplusplus_
extern "C"	{
#endif

#include <stdint.h>

typedef struct ap	{

	unsigned type; // Integer, Real, Radians, Degrees, etc...
	char* wholepart;
	char* fractpart;

	unsigned long long fp; // strlen(wholepart)-1
	char sign;
	int base;
	unsigned long long len;
	void* precision;

	/* not */
	char desc[1024];
	void* maxPrecision;
} ap;


typedef struct angle	{
	
	struct ap v;
	int type; // radians or degrees, etc
	
	int clockwise;
	
} angle;


struct ap AP0;
struct ap AP1;
struct ap AP2;
struct ap AP3;

// AP1_2, AP1_3, AP1_4, AP 1_5, AP3_4, AP4_5, AP2_3, AP1_7, AP2_7, AP22_7
struct ap AP10;
struct ap AP16;
struct ap PI;
struct ap e; // Euler's Constant.
struct ap GR; // The "Golden Ratio".

typedef struct point_t	{

	struct ap t;
	struct ap x;
	struct ap y;
	struct ap z;
	struct ap k;
	
	uint8_t type;

} point_t;

typedef struct vector_t	{
	
	struct ap t;
	struct ap x;
	struct ap y;
	struct ap z;
	struct ap k;
	
	uint8_t type;

} vector_t;


typedef struct circle_t	{
	
	struct ap radius;
	struct point_t origin;
	struct ap** segments;
	
} circle_t;
	
typedef struct triangle_t {

	struct ap opp;
	struct ap adj;
	struct ap hyp;
	
	struct ap hyp_adj_angle;
	struct ap hyp_opp_angle;
	struct ap opp_adj_angle;
	
	uint8_t type; // isoscoles, equilateral, rightangle
} triangle_t;





typedef struct aplib_t {

	/* not */
	struct ap precision;
	char desc[1024];
	
	
	/* */
	struct ap base;
	
	void (*FreeAP)( struct ap* A );
	char (*getSign)( struct ap A );


	struct ap (*SimpleAP)( char* val );
	struct ap (*NewAP)( char* wholepart, char* fractpart, char sign, unsigned long long Base );
	struct ap (*getBase)( ap A );
	struct ap (*setSign)( struct ap* A, char s );
	
	struct ap (*setBase)( ap* A, unsigned long long Base );
	struct ap (*getMaxPrecision)( ap A );
	struct ap (*setMaxPrecision)( ap* A, ap P );
	struct ap (*getPrecision)( ap A );
	
	struct ap (*add)( struct ap A, struct ap B );
	struct ap (*sub)( struct ap A, struct ap B );
	struct ap (*mul)( struct ap A, struct ap B );
	struct ap (*div)( struct ap A, struct ap B );
	struct ap (*divby2)( struct ap A );
	struct ap (*reciprocal)( struct ap A );
	struct ap (*power)( struct ap A, struct ap E );
	struct ap (*sqrt)( struct ap A );
	struct ap (*log10)( struct ap A );
	struct ap (*log2)( struct ap A );
	struct ap (*log16)( struct ap A );
	struct ap (*log)( struct ap A, unsigned long long Base );
	
	signed short (*cmpdigitstr)( char* dstr1, char* dstr2 );
	signed short (*cmpap)( ap A, ap B );
	struct ap (*diff)( ap A, ap B );
	
	struct ap (*cos)( ap R );
	struct ap (*sine)( ap R );
	struct ap (*tan)( ap R );
	
	struct vector_t (*vec)( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k );
	struct point_t (*point)( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k );
	struct circle_t (*circle)( struct point_t* origin, struct ap* radius, struct vector_t* momentum, struct ap** segments );

	struct triangle_t (*triangulate)( struct ap* opp, struct ap* adj, struct ap* hyp, struct ap* hyp_adj_angle, struct ap* adj_opp_angle, struct ap* hyp_opp_angle );
	// any of these individual ptr args can be defined as NULL on a givn fnc invocation,
	// but at least 3 non-NULL operands need to be present, and correct.
	
	/*
	Library subsystems.
	*/
	struct ap (*PI)( ap P ); // if precision P==0 (e.g., AP0 ), the precision is based on the current setting for aplib.precision
	
} aplib_t;


extern struct aplib_t aplib; 
extern void InitAPLIB();

#ifdef _cplusplus_
}
#endif

#endif
