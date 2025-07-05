/* UNIT TESTS */


#include "../stringy/stringy.h"

static int totalNumTests = 0;
static char* testmatrix[ 512 ];
void (*test)()[ 512 ];

static int initialise()
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

	if( argc !=1 )	{
		
		testnumber = getTestNumber( argv[1] );
	}

	if( testnumber )	{

		test[testnumber]();
		goto finish;
	}

	int i = 0;
	for( i; i < totalNumTests; i++ )
		test[i]();

	// ....

	finish:

	printf( "All tests completed." ), exit(0);

}
