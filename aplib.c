/* DAVELIB_APLIB_C */

#include "aplib.h"

static struct ap add( ap A, ap B );
static struct ap sub( ap A, ap B );
static struct ap mul( ap A, ap B );
static struct ap div( ap A, ap B );

static struct ap reciprocal( ap A );
static struct ap sqroot( ap A );

static struct ap log	( ap A, unsigned long long Base );
static struct ap log10	( ap A );
static struct ap log2	( ap A );
static struct ap log16	( ap A );

unsigned long long strlen_( char* str )	{

	unsigned long long i = 0;
	
	while( *(str++) != 0 )
		i++;
	
	return i;
}
#define strlen strlen_

static struct ap SimpleAP( char* val )	{
	
	struct ap A;
	A.wholepart = getstring( val );
	A.fractpart = getstring( "0" );
	A.sign = '+';
	A.base = 10;
	
	unsigned long long wp = strlen( A.wholepart );
	A.len = wp + strlen( A.fractpart );
	A.fp = wp - 1;
	
	return A;
}

static struct ap NewAP( char* wp, char* fractpart, char sign, unsigned long long Base )	{
	
	struct ap A;
	A.wp = wp;
	A.fractpart = fractpart;
	A.sign = sign;
	A.base = Base;
	
	return A;
}

void InitAPLIB()	{
	
	aplib.simpleap = SimpleAP;
	
	
	aplib.add = add;
	aplib.sub = sub;
	aplib.mul = mul;
	aplib.div = div;
	
	aplib.reciprocal = reciprocal;
	aplib.sqrt = sqroot;
	aplib.log10 = log10;
	aplib.log2 = log2;
	aplib.log16 = log16;
	aplib.log = log;
	
	AP0.wholepart = getstring( "0" );
	
}



static void FreeAP( ap A )	{
	
	if( A.wholepart != NULL )
		free( A.wholepart );
	
	if( A.fractpart != NULL )
		free( A.fractpart );
	
	;
}


static ap sqrt_f( ap A )	{
	
	ap r = mul( A,A );
	ap r2 = sub( r, AP2 );
	
	FreeAP( r );
	
	return r2;
}


static ap sqroot( ap A )	{
	
	ap A;
	
	// ....
	
	return A;
}
