/* DAVELIB_APLIB_C */

#include <math.h>
#include <stdlib.h>
#include "./aplib.h"
#define apheapblock() (struct ap*)malloc( sizeof(struct ap) )
#define apscrub(v) {char* _;uint16_t x=0;_=(char*)v;while(x<sizeof(ap))_[x]=0;}
#define APFLAG_CLEAN 128
#define VECTOR 0
#define POINT 1

static struct ap* GET_FRESH_APSTRUCT( );
static void SCRUB_INVALID_APSTRUCT( struct ap* a );
static const struct PRECISION CALC_PRECISION( struct ap* a )
static const struct ap* LOCK_APSTRUCT( struct ap* a );
	
static struct ap* SimpleAP( char* val );
static struct ap* QuickAP( char* val );
static struct ap* NewAP( char* wholepart, char* fractpart, char* sign, int* base );
static void FreeAP( struct ap* A );
static int integrity_check( struct ap* a );
static struct ap* createap( char* wp, char* fp, char* sign, int* base, struct ap* precision, char* desc, unsigned* type );
static struct ap* add( struct ap* A, struct ap* B ); 
static struct ap* sub( struct ap* A, struct ap* B );
static struct ap* mul( struct ap* A, struct ap* B );
static struct ap* div( struct ap* A, struct ap* B );
static struct ap* divby2( struct ap* A );
static struct ap* reciprocal( struct ap* A );
static struct ap* power( struct ap* A, struct ap* E );
static signed short cmpdigitstr( char* dstr1, char* dstr2 );
static struct ap* sqroot( struct ap* A );
static struct ap* aplog( struct ap* A, int base );
static struct ap* aplog10( struct ap* A );
static struct ap* aplog2( struct ap* A );
static struct ap* aplog16( struct ap* A );
static struct ap* getMaxPrecision( struct ap* A );
static struct ap* setMaxPrecision( struct ap* A, struct ap* P );
static signed short cmpdigitstr( char* dstr1, char* dstr2 );
static signed short cmpap( struct ap* A, struct ap* B );
static struct ap* diff( struct ap* A, struct ap* B );
static struct vector_t vec( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k );
static struct point_t point( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k );
static struct circle_t circle( struct point_t* origin, struct ap* radius, struct vector_t* momentum, struct ap** segments );

static struct ap* createap( char* wp, char* fp, char* sign, int* base, struct ap* precision, char* desc, unsigned* type )	{

	struct ap* a = (struct ap*) malloc( sizeof( struct ap ) );
	
	unsigned long long x = 0;
	while( wp[x]=='0' )
		x++;
	
	a->wholepart = stringy->substring( wp, x, stringy->strlen(wp) );
	
	x = stringy->strlen( fp )-1;
	while( fp[x]=='0' )
		x--;
	
	a->fractpart = stringy->substring( fp, 0, x );
	
	a->sign = *sign;
	
	if( *base==2 ||
		*base==8 ||
		*base==10 ||
		*base==16 )
		a->base = *base;
	else
		a->base = 0;
	
	a->desc = desc;
	
	if( type!=NULL )
		a->type = *type;
	
	struct ap* p;
	if( precision!=NULL )
		p = (struct ap*) precision;
	
	a->precision = (void*) p;
	
	a->c = 0 | APFLAG_CLEAN;
	
	return a;
}

static int integrity_check( struct ap* a )	{

	if( a->c && APFLAG_CLEAN )
		return 1;

	unsigned long long w = 0;
	while( a->wp[w]=='0' )
		w++;

	unsigned long long wp_len = stringy->strlen( a->wp );
	unsigned long long fp_len = stringy->strlen( a->fp );
	unsigned long long f = fp_len - 1;
	while( a->fp[f]=='0' )
		f++;

	a->w = w;
	a->f = f;

	if( a->sign != '-' && a->sign != '+' )
		a->sign = '+';
	
	if( 	a->base != 2 &&
		a->base != 8 &&
		a->base != 10 &&
		a->base != 16 )
		a->base = 10;

	a->len = wp_len+fp_len;
	a->fp  = wp_len - 1;

	if( a->precision )
		integrity_check( (struct ap*) a->precision );

	a->c = a->c & APFLAG_CLEAN;

	return 1;
}

static signed short cmpdigitstr( char* dstr1, char* dstr2 )	{

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
/**

When generating a result row array, if MSD(A) + MSD(B) + 1 would not equal more than 9,
no need for extra overflow digit slot. If MSD(A) + MSD(B), or that and an additional +1 would be >= 10,
need the additional digit.

Length(resultrow[]) ADD: length( max(A,B) ) (might also need to +1
					MUL: length( A ) + length( B )
					
					DIV: length( A ) + nonstatic amount for fractional digits. Rserving DIV fractional blocks in a block of length(A) digits at a time
*/

/*
The Newton's Method root approximation functions. Only 'ap sqroot(ap)' is exposed via the aplib_t interface. The others are used by the
algorithm internally (by the core 'sqroot' function), they are implementation details that do not need to be directly-accessed by the User. 
*/
static struct ap* sqrt_f( struct ap* v )	{
	
	ap x2 = mul( v,v );
	ap x2_2 = sub( x2, AP2 );
	
	FreeAP( x2 );
	return x2_2;
}
static struct ap* sqrt_deriv_f( struct ap* v )	{

	return mul( v,AP2 );
}

static signed short cmpap( struct ap* A, struct ap* B )	{
	
	return 0;
}

static const struct ap* sqroot( struct ap* A )	{
	
	struct ap* x0 = div( A,AP2 );
	
	struct ap* epsilon = QuickAP( "0.0000000001" );
	struct ap* tolerance = QuickAP( "0.000001" );

	uint64_t max_iterations = 20;
	
	while( max_iterations-- )	{
		
		struct ap* y = sqrt_f( x0 );
		struct ap* yprime = sqrt_deriv_f( x0 );
		
		if( cmpap( yprime,epsilon ) < 0 )
			break;
		
		struct ap* x1 = div( y,yprime );
		struct ap* x1_b = sub( x0,x1 );

		struct ap* x1_c = sub( x1_b,x0 );
		
		FreeAP( x1 );
		FreeAP( x1_b );
		FreeAP( y );
		FreeAP( yprime );

		if( cmpap( x1_c,tolerance ) < 1 )
			return (const struct ap*) x1_c;
		
		FreeAP( x0 );
		x0 = x1_c;
	}
	
	return (const struct ap*) copyap( &AP0 );
}

static struct circle_t circle( struct point_t* origin, struct ap* radius, struct vector_t* momentum, struct ap** segments )	{
	
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
static struct vector_t vec( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k )	{
	
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
	
	return (struct vector_t) new_vector;
}

static struct vector_t point( struct ap* t, struct ap* x, struct ap* y, struct ap* z, struct ap* k )	{
	
	// simply pass AP0, or NULL, for any param that is targeted to be 0.
	struct vector_t new_point;

	if( t==NULL )
		t = (struct ap*)&AP0;
	
	if( x==NULL )
		x = (struct ap*)&AP0;
	
	if( y==NULL )
		y = (struct ap*)&AP0;
	
	if( z==NULL )
		z = (struct ap*)&AP0;
	
	if( k==NULL )
		k = (struct ap*)&AP0;
	
	new_point.t = *t;
	new_point.x = *x;
	new_point.y = *y;
	new_point.z = *z;
	new_point.k = *k;
	new_point.type = POINT;
	return (struct vector_t) new_point;
}

static struct ap diff( ap A, ap B )	{

	return sub( A,B );
}

static struct ap setMaxPrecision( struct ap A, struct ap P )	{

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

static struct ap getMaxPrecision( struct ap A )	{

	return *((struct ap*)A.maxPrecision); 
}

static struct ap getPrecision( struct ap A )	{
	
	struct ap* P = (struct ap*) A.precison;
	
	return *P;
}

static void FreeAP( struct ap* A )	{

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

static char* ull2digitstr( unsigned long long num )	{

	char* dstr = NULL;
	
	// .....
	// I will check C stdlib for an itoa that's type-compatible with an input ULL operand.
	// .....
	
	return dstr;
}

static void* GetPrecisionObject( unsigned long long len )	{

	struct ap* P = (struct ap*) malloc( sizeof( struct ap ) );
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

		
		diff += (( sd - d ) * pow( 10, order ));
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

			diff += (( sd - d ) * pow( 10, order ));
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

static struct ap* QuickAP( char* val )	{

	struct ap* A = (struct ap*) malloc( sizeof( struct ap ) );
	A->sign = '+'';
	A->base = 10;
	A->wholepart = NULL;
	A->fractpart = NULL;
	
	unsigned long long x = 0;
	unsigned long long y = 0;
	unsigned long long z = 0;
	
	while( val[x] )	{
		
		if( val[x]=='.' )
			A->wholepart = (char*)malloc( x + 1 ), z=x;
		
		x++;
	}
	
	if( A->wholepart==NULL )
		A->wholepart = getstring( val ), A->fractpart = getstring( "0" );
	else	{
		
		while( y<z )
			A->wholepart[y] = val[y++];
		
		A->wholepart[y] = '\0';

		A->fractpart = (char*)malloc( (x-z) + 1 );
		z=0;
		while( val[y] )
			A->fractpart[z++] = val[y++];
		
		A->fractpart[z] = '\0';
	}
		
	return A;
}

static struct ap* SimpleAP( char* val )	{
	
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

static struct ap* NewAP( char* wp, char* fractpart, char* sign, int* base, struct ap* precision ) {
	
	struct ap* a = (struct ap*) malloc( sizeof( struct ap ) );
	
	a->wholepart = wp;
	a->fractpart = fractpart;
	
	a->sign = *sign;
	a->base = *base;
	a->precision = precision;
	
	return a;
};

static struct ap* add( struct ap* A, struct ap* B )	{

}

const struct ap* COPY_APSTRUCT( struct ap* a )	{

	struct ap* b;
	
	b->wholepart 	= a->wholepart;
	b->fractpart 	= a->fractpart;
	b->wp_len    	= a->wp_len;
	b->fp_len		= a->fp_len;
	b->base			= a->base;
	b->sign			= a->sign;
	
	
	return (const struct ap*) b;
}

static struct ap* GET_FRESH_APSTRUCT( )	{

	return apheapblock();
}

static void SCRUB_INVALID_APSTRUCT( struct ap* a )	{

	uint16_t size_ap = sizeof( struct ap );
	uint16_t x = 0;
	
	char* _ = (char*) a;
	
	while( x<size_ap )
		_[a] = 0, x++;
	
	return;
}

static const struct ap* LOCK_APSTRUCT( struct ap* a )	{

	struct ap* b = GET_FRESH_APSTRUCT();

	uint8_t t = 0;
	
	
	if( a->wholepart != NULL )	{
		
		t += 1;
		b->wholepart = a->wholepart;
		b->wp_len = GET_DS_LENGTH( a->wholepart );
	}
	
	if( a->fractpart != NULL )	{
		
		t += 1;
		b->fractpart = a->fractpart;
		b->fp_len = GET_DS_LENGTH( b->fractpart );
	}
	
	if( t==0 )	{
		
		SCRUB_INVALID_APSTRUCT( a );
		return (const struct ap*)NULL;
	}


	if( a->sign != '-' )
		b->sign = '+';
	else
		b->sign = '-';
	
	if( a->base == 0 )
		b->base = 10;
	else
		b->base = a->base;
	
	if( a->precision==0 )
		b->precision = CALC_PRECISION( a );
	else
		b->precision = a->precision;
	
	SCRUB_INVALID_APSTRUCT( a );
	
	return (const struct ap*) b;
}

static const struct PRECISION CALC_PRECISION( struct ap* a )	{

	struct PRECISION P;
	
	P.WP_PREC = a->wp_len;
	P.FP_PREC = a->fp_len;
	
	return (const struct PRECISION) P;
}	

static struct ap* verify_ap( struct ap* A )	{
	
	unsigned long long len_A_wp, len_A_fp, len_B_wp, len_B_fp;
	len_A_wp = stringy->strlen( A->wholepart );
	len_A_fp = stringy->strlen( A->fractpart );
	len_B_wp = stringy->strlen( B->wholepart );
	len_B_fp = stringy->strlen( B->fractpart );

	unsigned long long A_fp, B_fp;
	A_fp = A->fp; B_fp = B->fp;

	unsigned long long wA = 0;
	unsigned long long fA = 0;
	
	while( A->wholepart[wA]=='0' )
		wA++;
	
	fA = len_A_fp - 1;
	while( A->fractpart[fA]=='0' )
		fA--;
	
	if( wA>0 )
		len_A_wp -= wA, A->w = w, A->fp = len_A_wp-1;
	
	if( fA>0 )
		len_A_fp -= fA, A->f = f;
}

static signed short cmpap( ap* A, ap* B )	{
	
	signed short t = cmpdstr( A.wholepart, B.wholepart );
	
	if( t )
		return t;
	
	t = cmpdstr( A.fractpart, B.fractpart );
	
	return t;
}

#define NEGATIVE '-'
#define POSITIVE '+'
void InitAPLIB()	{

	aplib = (struct aplib_t*) malloc( sizeof(struct aplib_t) );
	aplib->simpleap = SimpleAP;

	aplib->add = add;
	aplib->sub = sub;
	aplib->mul = mul;
	aplib->div = div;
	aplib->divby2 = divby2;
	
	aplib->reciprocal = reciprocal;
	aplib->power = power;
	aplib->sqrt = sqroot;
	aplib->log10 = log10;
	aplib->log2 = log2;
	aplib->log16 = log16;
	aplib->log = log;
	
	aplib->cmpdstr = cmpdstr;
	aplib->diff = diff;
	aplib->cmpap = cmpap;
	
	aplib->vec = vec;
	aplib->circle = circle;
	aplib->triangulate = triangulate;

	AP0 = (struct ap*)malloc( sizeof(struct ap) );
	AP0->wholepart = getstring( "0" );
	AP0->fractpart = getstring( "0" );
	AP0->sign = POSITIVE;
	AP0->base = 10;
	
	AP1 = (struct ap*)malloc( sizeof(struct ap) );
	AP1->wholepart = getstring( "1" );
	AP1->fractpart = getstring( "0" );
	AP1->sign = POSITIVE;
	AP1->base = 10;

	#define apheapblock() (struct ap*)malloc( sizeof(struct ap) )
	
	
	AP2 = (struct ap*)malloc( sizeof(struct ap) );
	AP2->wholepart = getstring( "2" );
	AP2->fractpart = getstring( "0" );
	AP2->sign = POSITIVE;
	AP2->base = 10;

	AP10 = apheapblock();
	AP10->wholepart = getstring( "10" );
	AP10->fractpart = getstring( "0" );
	AP10->sign = POSITIVE;
	AP10->base = 10;
	
	AP16 = apheapblock();
	AP16->wholepart = getstring( "16" );
	AP16->fractpart = getstring( "0" );
	AP16->sign = POSITIVE;
	AP16->base = 10;
	
	void* prec_copy = aplib->precision;
	
	aplib->precision = (void*) SimpleAP( "1024" );
	APSQRT2 = aplib->sqroot( AP2 );
	aplib->FreeAP( (struct ap*)aplib->precision );

	aplib->precision = prec_copy;
}

/*
The SQROOT functions, implementing Isaac Newton's Method for converging-approximations of roots.
Not fully implemented yet.
*/
static struct ap* sqrt_f( struct ap* A )	{
	
	struct ap* r = mul( A,A );
	struct ap* r2 = sub( r, AP2 );
	
	aplib->FreeAP( r );
	
	return r2;
}
static struct ap* sqrt_deriv_f( struct ap* A )	{

	struct ap* result = mul( A,AP2 );
	return result;
}

