/* APLIB_TESTS_H */

#ifndef DAVELIB_TESTBED_H
#define DAVELIB_TESTBED_H

typedef struct Testbed	{

	const int (*addTest)( void* fncPtr, char* testname );

	const int (*run)( int* testnums );
	const int (*runall)( void );

	const int (*listAllTests)( void );
	const int (*getTestNumber)( char* testname );
	const char* (*getTestName)( int testnum );

} Testbed;

extern const struct Testbed testbed;
extern const void InitTestBed();

#endif
