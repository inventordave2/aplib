/* UNIT TESTS */

#include <stdio.h>
#include "./../stringy/stringy.h"

#define APLIB_MAX_NUM_TESTS	512
static int totalNumTests = 0;
static char* testmatrix[ 512 ];
static void* tests[ 512 ];

static int initialise();


int getTestNumber( char* testname )	{

	int i = 0;
	
	int x = 0;
	for( x; x < totalNumTests; x++ )
		if( !strcmp( testmatrix[x],testname ) )
			return x;

	// ....

	return i;
}


// signed long long cmpULL2ap( unsigned long long ull, ap Len )
void cmpULL2ap_test(void)	{

	signed long long r = 0;
	
	unsigned long long ull = 123456789;
	ap l = SimpleAP( "123456789" );
	
	r = cmpULL2ap( ull, l );
	
	printf( "Comparing %ll with %s resulted in code: %ll\n", ull, l.wholepart, r );
	
	return;
}


static int initialise()	{
	
	unsigned int x = 0;
	
	testmatrix[x] = getstring( "cmp_unsigned_long_long_to_ap" );
	tests[x] = cmpULL2ap_test;
	
	x++;
	
	// ...
	
	
	return x;
}

int dotests( int argc, char** argv )	{

	unsigned int x = initialise();
	
	printf( "Total number of test functions installed: %d\n", x );
	
	if( !x )
		exit( 1 );
	
	int testnumber = 0;

	char* testname = NULL;

	if( argc != 1 )	{

		testname = getstring( argv[1] );
		testnumber = getTestNumber( testname );
	}

	if( testnumber )	{

		if( test[testnumber] != NULL )
			test[testnumber]();
		else
			printf( "Test '%s' has not been registered.", testname );

		goto finish;
	}

	int i = 0;
	for( i; i < totalNumTests; i++ )
		test[i]();

	// ....

	finish:

	#include <stdlib.h>
	if( testname != NULL )
		free( testname );

	printf( "All tests completed." );
	exit(0);
}
