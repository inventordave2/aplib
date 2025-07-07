/* DAVELIB_APLIB_C */

#include "./../aplib.h"

static const struct ap SimpleAP( char* val );
static const struct ap NewAP( char* wholepart, char* fractpart, char sign, unsigned long long Base );

static void FreeAP( ap A );

/*
The core virtual arithmetic operators. When the aplib library is deployed with a C++ wrapper, these are the functions that will be mapped
to the native operator primitives '+', '-', '*', and '/' via the C++ scope operator overloading semantics.


operator+( ap A, ap B )	{ return aplib.add( A,B ); }
operator-( ap A, ap B ) { return aplib.sub( A,B ); }
operator*( ap A, ap B ) { return aplib.mul( A,B ); }
operator/( ap A, ap B ) {

	if( (!aplib.cmpdigitstr( "2", B.wholepart )) && (!aplib.cmpdigitstr( "0", B.fractpart )) )
		return aplib.divby2( A );
	else
		return aplib.div( A,B );
}

static const signed short cmpdigitstr( char* dstr1, char* dstr2 )	{

	unsigned long long x = 0;
	unsigned long long y = 0;
	uint8_t flag = 0;

	char d1 = 0;
	char d2 = 0;

	while( 1 )	{
	
		d1 = dstr1[x];
		if( d1=='0' )
			x++;
		else
			break;
	}

	while( 1 ) 	{
		
		d2 = dstr2[y];
		if( d2=='0' )
			y++;
		else
			break;
	}

	loop:

	if( d1 != d2 )	{

		// if a) one string is shorter than the other, then that is the smaller digitstring in scalar value.
		// else, the number with the earliest (from highest-order digit, nearest the beginning of the dstring ) bigger number is the
		// bigger scalar value.

		if( d1==0 )
			return -1;
		
		if( d2==0 )
			return +1;
		
		if( flag==0 )
			(d1>d2)?flag=1:flag=2;
	}

	if( d1 != 0 )	{
		
		x++;
		y++;
		
		d1 = dstr1[x];
		d2 = dstr2[y];
		goto loop;
	}
	
	if( flag==2 )
		return -1;
	if( flag==1 )
		return +1;
	
	return 0;
}


*/
static const struct ap add( const struct ap A, const struct ap B );
static const struct ap sub( const struct ap A, const struct ap B );
static const struct ap mul( const struct ap A, const struct ap B );
static const struct ap div( const struct ap A, const struct ap B );
static const struct ap divby2( const struct ap A );


static const struct ap reciprocal( const struct ap A );
static const struct ap power( const struct ap A, const struct ap E );

/*
The Newton's Method root approximation functions. Only 'ap sqroot(ap)' is exposed via the aplib_t interface. The others are used by the
algorithm internally (by the core 'sqroot' function), they are implementation details that do not need to be directly-accessed by the User. 
*/
static const struct ap sqrt_f( const struct ap );
static const struct ap sqrt_deriv_f( const struct ap );
static const struct ap sqroot( const struct ap A );


static const struct ap log( const struct ap A, unsigned long long Base );
static const struct ap log10( const struct ap A );
static const struct ap log2( const struct ap A );
static const struct ap log16( const struct ap A );
static const struct ap getMaxPrecision( const struct ap A );
static const struct ap setMaxPrecision( const struct ap A, const struct ap P );



static const signed short cmpdigitstr( char* dstr1, char* dstr2 );
static const signed short cmpap( ap A, ap B );
static const struct ap diff( ap A, ap B );

static const struct vector_t vec( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k );
static const struct point_t point( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k )

static const struct circle_t (*circle)( struct point_t* origin, struct ap* radius, struct vector_t* momentum, struct ap** segments );

static const struct circle_t (*circle)( struct point_t* origin, struct ap* radius, struct vector_t* momentum, struct ap** segments )	{
	
	struct circle_t c;
	
	if( radius==NULL )
		radius = *AP1;
	else
		c.radius = *radius;
	
	if( momentum==NULL )
		c.momentum = vec( NULL, NULL, NULL, NULL, NULL );
	else
		c.momentum = *momentum;
	
	c.segments = segments;
}

static const struct vector_t vec( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k )	{
	
	// simply pass AP0, or NULL, for any param that is targeted to be 0.
	struct vector_t new_vector;

	if( t==NULL )
		t = &AP0;
	
	if( x==NULL )
		x = &AP0;
	
	if( y==NULL )
		y = &AP0;
	
	if( z==NULL )
		z = &AP0;
	
	if( k==NULL )
		k = &AP0;
	
	new_vector.t = *t;
	new_vector.x = *x;
	new_vector.y = *y;
	new_vector.z = *z;
	new_vector.k = *k;
	
	return (const struct vector_t) new_vector;
}

static const struct point_t point( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k )	{
	
	// simply pass AP0, or NULL, for any param that is targeted to be 0.
	struct point_t new_point;

	if( t==NULL )
		t = &AP0;
	
	if( x==NULL )
		x = &AP0;
	
	if( y==NULL )
		y = &AP0;
	
	if( z==NULL )
		z = &AP0;
	
	if( k==NULL )
		k = &AP0;
	
	new_point.t = *t;
	new_point.x = *x;
	new_point.y = *y;
	new_point.z = *z;
	new_point.k = *k;
	
	return (const struct point_t) new_point;
}



A = -4, B = -3	== -7 (-1)
A = -4, B = +3	== +1 (-7)
A = +4, B = -3	== +1 (+7)
A = +4, B = +3	== +7 (+1)
// c = sub( a,b )
// the abs(diff) is sign(+)
// if( signed(A)<signed(B), sign(C)=='-'

static const struct ap diff( ap A, ap B )	{

	return sub( A,B );
}


static const struct ap setMaxPrecision( const struct ap A, const struct ap P )	{

	struct ap* Aref = &((struct ap)A);

	if( Aref->maxPrecision != NULL )
		free( Aref->maxPrecision ), Aref->maxPrecision = NULL;
	
	struct ap* prec = (struct ap*) malloc( sizeof( struct ap ) );
	memcpy( prec, &P, sizeof( struct ap ) );
	
	Aref->maxPrecision = (void*) prec;

	#include <assert.h>
	assert( !aplib.cmpInt( A.maxPrecision.wholepart, P.wholepart ) );
	
	return A;
}

static const struct ap getMaxPrecision( const struct ap A )	{

	return *((const struct ap*)A.maxPrecision); 
}

static const struct ap getPrecision( const struct ap A )	{
	
	const struct ap* P = (const struct ap*) A.precison;
	
	return *P;
}

static void FreeAP( const struct ap* A )	{

	struct ap* Acopy = (struct ap*) A;
	 
	if( Acopy->wholepart != NULL )
		free( Acopy->wholepart );
	
	if( Acopy->fractpart != NULL )
		free( Acopy->fractpart );
	
	char* _ = (char*)Acopy;
	
	unsigned int x = 0;
	for( ; x < sizeof( ap ); x++ )
		_[x] = 0;
}

static const char* ull2digitstr( unsigned long long num )	{

	char* dstr = NULL;
	
	// .....
	// I will check C stdlib for an itoa that's type-compatible with an input ULL operand.
	// .....
	
	return dstr;
}

static const void* GetPrecisionObject( unsigned long long len )	{

	const struct ap* P = (const struct ap*) malloc( sizeof( struct ap ) );
	P->sign = POSITIVE;
	P->base = 10;
	P->fractpart = stringy.getstring( "0" );
	P->wholepart = stringy.ull2digitstr( len );
	P->len = stringy.strlen( P.wholepart );
	P->maxPrecision = NULL;
	P->precision = NULL;
	P->desc = "Precision specifier. Precision specifiers do not have precision specifiers. To get that information, (struct ap*)P->len has an unsigned long long representation of the Precision specifier's precision. Specify!"
	return (void*) P;
}

/* 

*/
#include <math.h>
signed long long cmpULL2ap( unsigned long long ull, ap Len )	{

	uint8_t d;
	uint8_t sd;
	unsigned order = 0;
	signed long long diff = 0;
	unsigned i = strlen( Len.wholepart );
	
	
	loop:
	
	d = Len.wholepart[ i ];
	d = d - '0';
	
	sd = ull % 10;

	if( d!=sd )	{

		
		diff += (( sd - d ) * pow( 10, order );
	}

	order += 1;
	ull -= sd;
	ull /= 10;
	i++;
	
	if( ull < 10 )	{
		
		// final highest-order digit of input value 'ull'.
		d = Len.wholepart[ i ];
		d = d - '0';
		sd = ull % 10;

		if( d!=sd )	{

			diff += (( sd - d ) * pow( 10, order );
		}
	}
	else
		goto loop;
	
	
	return diff;
}


static char* clipstring( char* str, ap Len )	{

	char* r;
	
	unsigned long long strlen_str = strlen( str );
	signed long long diff = 0;
	if( (diff=cmpULL2ap( strlen_str, Len ))>0 )	{
		
		r = substring( str, 0, diff );
	}
	else
		r = str;
	
	return r;
}

static const struct ap SimpleAP( char* val )	{
	
	struct ap A;
	A.maxPrecision = aplib.precision;
	
	char* clippedval = aplib.clipstring( val, A.maxPrecision );
	A.wholepart = clippedval;
	A.fractpart = getstring( "0" );
	A.sign = '+';
	A.base = 10;
	A.precision = GetPrecisionObject( strlen( clippedval ) );


	unsigned long long wp = strlen( A.wholepart );
	A.len = wp + strlen( A.fractpart );
	A.fp = wp - 1;
	
	return A;
}

static const struct ap NewAP( char* wp, char* fractpart, char sign, unsigned long long Base )	{
	
	struct ap A;
	A.wp = wp;
	A.fractpart = fractpart;
	A.sign = sign;
	A.base = Base;
	
	return A;
}

static const signed short cmpap( ap A, ap B )	{
	
	const signed short t = cmpdstr( A.wholepart, B.wholepart );
	
	if( t )
		return t;
	
	t = cmpdstr( A.fractpart, B.fractpart );
	
	return t;
}


#define NEGATIVE '-'
#define POSITIVE '+'
void InitAPLIB()	{

	aplib.simpleap = SimpleAP;

	aplib.add = add;
	aplib.sub = sub;
	aplib.mul = mul;
	aplib.div = div;
	aplib.divby2 = divby2;
	
	aplib.reciprocal = reciprocal;
	aplib.power = power;
	aplib.sqrt = sqroot;
	aplib.log10 = log10;
	aplib.log2 = log2;
	aplib.log16 = log16;
	aplib.log = log;
	
	aplib.cmpdstr = cmpdstr;
	aplib.diff = diff;
	aplib.cmpap = cmpap;
	
	aplib.vec = vec;
	aplib.circle = circle;
	aplib.triangulate = triangulate;

	AP0.wholepart = getstring( "0" );
	AP0.fractpart = getstring( "0" );
	AP0.sign = POSITIVE;
	AP0.base = 10;
	
	AP1.wholepart = getstring( "1" );
	AP1.fractpart = getstring( "0" );
	AP1.sign = POSITIVE;
	AP1.base = 10;

	AP2.wholepart = getstring( "2" );
	AP2.fractpart = getstring( "0" );
	AP2.sign = POSITIVE;
	AP2.base = 10;

	AP10.wholepart = getstring( "10" );
	AP10.fractpart = getstring( "0" );
	AP10.sign = POSITIVE;
	AP10.base = 10;
	
	AP16.wholepart = getstring( "16" );
	AP16.fractpart = getstring( "0" );
	AP16.sign = POSITIVE;
	AP16.base = 10;
	
	void* prec_copy = aplib.precision;
	aplib.precision = (void*)&(SimpleAP( "1024" ));
	APSQRT2 = aplib.sqrt( AP2 );
	aplib.FreeAP( (struct ap*)aplib.precision );
	aplib.precision = prec_copy;
}

/*
The SQROOT functions, implementing Isaac Newton's Method for converging-approximations of roots.
Not fully implemented yet.
*/

static const struct ap sqrt_f( const struct ap A )	{
	
	const struct ap r = mul( A,A );
	const struct ap r2 = sub( r, AP2 );
	
	aplib.FreeAP( &r );
	
	return r2;
}

static const struct ap sqrt_deriv_f( const struct ap A )	{

	const struct ap result;
	// ....
	
	return result;
}

static const struct ap sqroot( const struct ap A )	{
	
	const struct ap A;
	
	// ....
	
	return A;
}
