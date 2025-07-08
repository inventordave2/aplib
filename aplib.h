/* APLIB_H */

#ifndef DAVELIB_APLIB_H
#define DAVELIB_APLIB_H

#ifdef _cplusplus_
extern "C"	{
#endif



typedef struct ap	{

	const unsigned type; // Integer, Real, Radians, Degrees, etc...
	const char* wholepart;
	const char* fractpart;

	const unsigned long long fp; // strlen(wholepart)-1
	const char sign;
	const int base;
	const unsigned long long len;
	const void* precision;

	/* not const */
	char desc[1024] = { 0 };
	void* maxPrecision;
} ap;


typedef struct angle	{
	
	const struct ap v;
	const int type; // radians or degrees, etc
	
	bool clockwise = 1;
	
} angle;


const struct ap AP0;
const struct ap AP1;
const struct ap AP2;
const struct ap AP3;

// AP1_2, AP1_3, AP1_4, AP 1_5, AP3_4, AP4_5, AP2_3, AP1_7, AP2_7, AP22_7
const struct ap AP10;
const struct ap AP16;
const struct ap PI;
const struct ap e; // Euler's Constant.
const struct ap GR; // The "Golden Ratio".



typedef struct circle_t	{
	
	const struct ap radius;
	const struct point_t origin;
	const struct ap** segments;
	
} circle_t;
	
typedef struct triangle_t {

	struct ap opp;
	struct ap adj;
	struct ap hyp;
	
	struct ap hyp_adj_angle;
	struct ap hyp_opp_angle;
	struct ap opp_adj_angle;
	
	uint8t type; // isoscoles, equilateral, rightangle
} triangle_t;

typedef struct point_t	{

	const struct ap t;
	const struct ap x;
	const struct ap y;
	const struct ap z;
	const struct ap k;
	
	const uint8_t type = 1;

} point_t;

typedef struct vector_t	{
	
	const struct ap t;
	const struct ap x;
	const struct ap y;
	const struct ap z;
	const struct ap k;
	
	const uint8_t type = 0;

} vector_t;


typedef struct aplib_t {

	/* not const */
	struct ap precision;
	char desc[1024] = { 0 };
	
	
	/* const */
	const struct ap base;
	
	void (*FreeAP)( const struct ap* A );
	char (*getSign)( const struct ap A );


	const struct ap (*SimpleAP)( char* val );
	const struct ap (*NewAP)( char* wholepart, char* fractpart, char sign, unsigned long long Base );
	const struct ap (*getBase)( ap A );
	const struct ap (*setSign)( struct ap* A, char s );
	
	const struct ap (*setBase)( ap* A, unsigned long long Base );
	const struct ap (*getMaxPrecision)( ap A );
	const struct ap (*setMaxPrecision)( ap* A, ap P );
	const struct ap (*getPrecision)( ap A );
	
	const struct ap (*add)( const struct ap A, const struct ap B );
	const struct ap (*sub)( const struct ap A, const struct ap B );
	const struct ap (*mul)( const struct ap A, const struct ap B );
	const struct ap (*div)( const struct ap A, const struct ap B );
	const struct ap (*divby2)( const struct ap A );
	const struct ap (*reciprocal)( const struct ap A );
	const struct ap (*power)( const struct ap A, const struct ap E );
	const struct ap (*sqrt)( const struct ap A );
	const struct ap (*log10)( const struct ap A );
	const struct ap (*log2)( const struct ap A );
	const struct ap (*log16)( const struct ap A );
	const struct ap (*log)( const struct ap A, unsigned long long Base );
	
	const signed short (*cmpdigitstr)( char* dstr1, char* dstr2 );
	const signed short (*cmpap)( ap A, ap B );
	const struct ap (*diff)( ap A, ap B );
	
	const struct ap (*cos)( ap R );
	const struct ap (*sine)( ap R );
	const struct ap (*tan)( ap R );
	
	const struct vector_t (*vec)( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k );
	const struct point_t (*point)( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k );
	const struct circle_t (*circle)( struct point_t* origin, struct ap* radius, struct vector_t* momentum, struct ap** segments );

	const struct triangle_t (*triangulate)( struct ap* opp, struct ap* adj, struct ap* hyp, struct ap* hyp_adj_angle, struct ap* adj_opp_angle, struct ap* hyp_opp_angle );
	// any of these individual ptr args can be defined as NULL on a givn fnc invocation,
	// but at least 3 non-NULL operands need to be present, and correct.
	
	const struct circle_t (*circle)( struct point_t* origin, struct ap* radius, struct vector_t* momentum, struct ap** segments );
	
	/*
	Library subsystems.
	*/
	const struct ap (*PI)( ap P ); // if precision P==0 (e.g., AP0 ), the precision is based on the current setting for aplib.precision
	
} aplib_t;


extern struct aplib_t aplib; 
extern void InitAPLIB();

#ifdef _cplusplus_
}
#endif

#endif
