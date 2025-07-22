// APLIB_CPP

#include "./aplib.h"
#include "./aplibcpp.hpp"

namespace mathematical	{
	
	operator+( ap& A, ap& B ) { return aplib->add( A,B ); }
	
	operator-( ap& A, ap& B ) { return aplib->sub( A,B ); }
	operator--( ap&A ) { aplib->DEC( A ); }
	operator*( ap& A, ap& B ) { return aplib->mul( A,B ); }
	operator/( ap& A, ap& B ) { if(!aplib->cmpap( B,AP2 )) return aplib->divby2( A ); return aplib->div( A,B ); }

	operator=(ap& A, ap& B )	{
		
		aplib->FreeAP( A );
		A = aplib->CopyAP( B );
		
		return A;
	}

	operator==(ap &A, ap &B )	{
		
		return aplib->CmpAP( A,B );
	}

	operator>>( ap &A, unsigned long long d )	{
		
		struct ap& C = A;
		
		while( d>0 )	{
			
			aplib->FreeAP( C );
			C = aplib->div( A,AP2 );
			aplib->FreeAP( A );
			A = aplib->CopyAP( C );
			
			d--;
		}
		
		return C;
	}

	operator<<( ap& A, unsigned long long d )	{
		
		struct ap& C = aplib->CopyAP( A );
		
		while( d>0 )	{
			
			struct ap* D;
			D = aplib->mul( C,AP2 );
			FreeAP( C );
			C = D;
			
			d--;
		}
		
		return A;
	}

	#define type class
	
	type ap;
	type rational extends ap;
	type real extends ap;
	type integer extends ap;
	type unit extends ap;
	type order extends integer;
	type exponent extends ap;
	type logarithm extends exponent;
	
	typedef char* digitstring;
	typedef { char*,char* } fractional;
	
}

