/* UNIT TESTS */


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



int main( int argc, char** argv )	{

	int testnumber = 0;

	char* testname = NULL;

	if( argc !=1 )	{

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
