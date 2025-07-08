// APLIB_CPP


operator+( ap A, ap B )	{ return aplib.add( A,B ); }
operator-( ap A, ap B ) { return aplib.sub( A,B ); }
operator*( ap A, ap B ) { return aplib.mul( A,B ); }
operator/( ap A, ap B ) {

	if( (!aplib.cmpdigitstr( "2", B.wholepart )) && (!aplib.cmpdigitstr( "0", B.fractpart )) )
		return aplib.divby2( A );
	else
		return aplib.div( A,B );
}

