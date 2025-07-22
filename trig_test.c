#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

typedef struct vec2d	{

	double x;
	double y;

} vec2d;

struct vec2d sine( double angle );
struct vec2d sine_alt( double angle );
double magnitude( struct vec2d v );
struct vec2d normalize( struct vec2d v );
struct vec2d getvec( double angle );

double x = 123456789.01234567890;
double y = 0.0;
double sety( double angle );

double taylor_series( double angle )	{

	// x - x^3/3! + x^5/5! - x^7/7! + x^9/9!
	uint64_t xpe = 1;
	
	uint64_t pbd[128];
	
	uint8_t y = 0;
	for( y; y<128; y++ )
		xpe *= (xpe+=1) * (xpe+=1), pbd[y] = xpe;
	
	double x = angle;
	
	xpe = 3;
	signed int t = -1;
	for( y=0; y<80; y++ )
		x += ( (double)t * (pow(angle, (double)xpe)/(double)pbd[y]) ), xpe += 2, t = t * -1; 
	
	return x;
}

int main( int argc, char** argv )	{

	double angle;
	
	if( argc>1 )
		sscanf( argv[1], "%lf", &angle );
	else
		angle = 22.5;	

	double PI =3.141592653589793238462643;
	double x = taylor_series( angle*(PI/180.0) );
	double stdx = sin( angle*(PI/180.0) );
	
	printf( "approximation:\t%.20f\nstdlib_sine:\t%.20f\n", x, stdx );
	
	return 0;

	/*
		sin θ = Opposite Side/Hypotenuse.
		cos θ = Adjacent Side/Hypotenuse.
		tan θ = Opposite Side/Adjacent Side.
	*/

}

double magnitude( struct vec2d v )	{
	
	double u = (v.x*v.x) + (v.y*v.y);
	
	double ru = pow( u, 0.5000000000000000 );
	
	return ru;
}

struct vec2d normalize( struct vec2d v )	{
	
	double mag;
	
	if( !(mag=magnitude(v)) )
		return v;
	
	struct vec2d newv;
	
	newv.x = v.x / mag;
	newv.y = v.y / mag;
	
	return newv;
}

struct vec2d getvec( double angle )	{

	sety( angle );
	
	struct vec2d v;
	
	v.x = x;
	v.y = y;
	
	return v;
}

double sety( double angle )	{
	
	y = x*((180.0-angle)/180.0);
	
	return y;
}

struct vec2d sine( double angle )	{

	sety( angle );
	
	printf( "unmodified:\nx = %.20f\ny = %.20f\n", x, y );
	
	vec2d v;
	v.x = x;
	v.y = y;
	
	vec2d normal_v = normalize( v );
	
	printf( "normalized( v ) gives:\nx = %.20f\ny = %.20f\n", normal_v.x, normal_v.y );
	return normal_v;

}

