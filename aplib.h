/* APLIB_H */

#ifndef DAVELIB_APLIB_H
#define DAVELIB_APLIB_H

typedef struct ap	{
	
	const char* wholepart;
	const char* fractpart;
	const char sign;
	const int base;
	const unsigned long long len;
	
} ap;

typedef struct aplib_t {

	const struct ap (*SimpleAP)( char* val );
	struct ap (*add)( struct ap A, struct ap B );
	struct ap (*sub)( struct ap A, struct ap B );
	struct ap (*mul)( struct ap A, struct ap B );
	struct ap (*div)( struct ap A, struct ap B );
	
	struct ap (*reciprocal)( struct ap A );
	struct ap (*sqrt)( struct ap A, struct ap B );
	struct ap (*log10)( struct ap A );
	struct ap (*log2)( struct ap A );
	struct ap (*log16( struct ap A );
	struct ap (*log)( struct ap A, unsigned long long Base );
	
} aplib_t;

extern struct aplib_t aplib; 
extern void InitAPLIB();

struct ap AP0;
struct ap AP1;
struct ap AP2;
struct ap AP10;
struct ap AP16;

#endif
