# aplib
Arbitray-precision maths library

In-development. Upon release of a production grade and fully tested major release version (v 1.x, will comprise:

A Core providing the mathematical operators, and an optional C++ wrapper, to take advantage of C++'s better modularity.


====================================
Devpad.
-------

The core operators as yet do not have full safety features. They are designed to check for interim result calculations being in a valid range (e.g., for addition, any given column-wise interim result should be in the range [-19, +19], with the 'tens' digiting carrying over to the left-wise column, which without overflow is logically required (for base10) to be in the range of [-18, +18], for a potential maximum of 2 operands of +9 before a potential digit-carry from an interim calculation comprising the right-wise adjacent column. For multiplication, a colum-wise range is [[-89, +89], although in multiplication the sign calculation is done seperately (the interim calculations are done unisgned) based on the multiplicationsign rule that says the sign of the result is based on the signs of the 2 input operands (++ and -- result in a + sign for result, opposite signs for the operands result in the negative sign (-) for the result).

Optional safety checkers:


typedef struct resultObject	{

	unsigned long long x;
	unsigned err;
	char* msg;
}


void freeResultObj( struct resultObject r )	{

	if( r.msg!=NULL )
	free( r.msg );
	
	r.x = 0;
	r.err = 0;
}

void freeResultObjRef( struct resultObject* r )	{

	if( r==NULL )
		return;
		
	if( r->msg != NULL )
		free( r->msg );
	
	if( isGCStored(r) )
		gcfree( r );
}

struct resultObject checkDigitString( char* dstr )	{
	
	struct resultObject r;
	r.msg = NULL;
	
	unsigned long long x = 0;
	unsigned long long dstr_len = stringy.strlen(dstr);
	
	for( ; x < dstr_len; x++ )
		if( dstr[x] < '0' || dstr[x] > '9' )
			r.err = 1, r.x = x, r.msg = stringy.getstring("Invalid digit in input decimal string. Check resultobj.x for offset."), return r;
			
	r.err = 0;
	r.x = 0;
	r.msg = getstring("All digits within (ASCII printable) numerical range of '0'to '9'.");
	return r;

}






